#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamefile.h"
#include "snapshot/index.h"
#include "utils/ud.h"
#include "common/internal.h"
#include <ad_list.h>

static SPWAW_ERROR
build_formations_list (FORMATION *src, BYTE player, USHORT start, FORMATION_LIST &list, char *loghdr)
{
	USHORT		i;
	int		seen[FORMCOUNT];
	char		name[17];

	memset (seen, 0, sizeof(seen));

	//log ("%s: start=%u\n", loghdr, start);

	// First skip all non-player formations
	i = start;
	while ((i < FORMCOUNT) && (src[i].player != player)) i++;

	// Now add all valid player formations
	while (i < FORMCOUNT) {

		if (src[i].leader == SPWAW_BADIDX) {
			// skipped: no leader
			log ("%s: [%3.3u] SKIPPED (no leader)\n", loghdr, i);
		} else 	if (src[i].player != player) {
			// skipped: wrong player
			log ("%s: [%3.3u] SKIPPED (wrong player ID %u)\n", loghdr, i, src[i].player);
		} else if (seen[src[i].ID] != 0) {
			// skipped: duplicate formation ID
			log ("%s: [%3.3u] SKIPPED (duplicate formation ID %u)\n", loghdr, i, src[i].ID);
		} else {
			snprintf (name, sizeof (name) - 1, "%s", src[i].name); name[16] = '\0';
			log ("%s: [%3.3u] FORMATION: P<%1.1u> ID<%3.3u> L<%5.5u> (%16.16s)\n",
				loghdr, i, src[i].player, src[i].ID, src[i].leader, src[i].name);

			list.list[list.cnt++] = i;

			seen[src[i].ID] = 1;
		}

		i++;
	}
	log ("%s: cnt=%u\n", loghdr, list.cnt);

	return (SPWERR_OK);
}

static SPWAW_ERROR
build_formations (FORMATION *src, BYTE player, USHORT start, USHORT alt_start, FORMATION_LIST &list)
{
	SPWAW_ERROR	rc;

	memset (&list, 0, sizeof(list));

	rc = build_formations_list (src, player, start, list, "formationcount");
	ROE ("build_formations_list()");

	if ((list.cnt == 0) && (alt_start != start)) {
		rc = build_formations_list (src, player, alt_start, list, "ALT~formationcount");
		ROE ("build_formations_list(alt)");
	}

	if (list.cnt == 0) RWE (SPWERR_BADSAVEDATA, "no formations found");

	// TODO: CHECK: the list of formations is not sorted?

	return (SPWERR_OK);
}

static SPWAW_ERROR
add_formation (USHORT idx, FORMATION *src, USHORT start, SPWAW_SNAP_OOB_FELRAW *dst, STRTAB *stab)
{
	SPWAW_UD		*UD;

	dst->RID	= idx;
	dst->FID	= src->ID - (BYTE)start;

	if ((dst->FID != 0) && (src->name[0] != '\0')) {
		dst->name = azstrstab (src->name, stab);
	} else {
		dst->name = NULL;
	}

	dst->leader	= src->leader;
	dst->hcmd	= src->hcmd;
	dst->OOBrid	= src->OOBid;
	dst->status	= src->status;
	dst->player	= src->player;

	if (cfg.withUD) {
		UD = &(dst->UD);
		UD_init (UD, sizeof (*src));
		UD_ADD (UD, src, __data000);
		UD_ADD (UD, src, __data0010);
		UD_ADD (UD, src, __data0011);
		UD_ADD (UD, src, __data010);
		UD_ADD (UD, src, __data011);
	}

	return (SPWERR_OK);
}

static SPWAW_ERROR
add_formations (FORMATION *src, SPWAW_SNAP_OOB_FRAW *dst, FORMATION_LIST &list, STRTAB *stab)
{
	SPWAW_ERROR		rc;
	USHORT			cnt;
	USHORT			start;
	SPWAW_SNAP_OOB_FELRAW	*p;
	USHORT			i;

	if ((cnt = list.cnt) == 0) RWE (SPWERR_FAILED, "internal error: unexpected empty formations list");

	start = src[list.list[0]].ID;

	dst->raw = safe_nmalloc (SPWAW_SNAP_OOB_FELRAW, cnt); COOM (dst->raw, "SPWAW_SNAP_OOB_FELRAW list");
	dst->cnt   = (USHORT)cnt;
	dst->start = start;

	for (i=0; i<cnt; i++) {
		p = &(dst->raw[i]);
		rc = add_formation (list.list[i], &src[list.list[i]], dst->start, p, stab);
		ERRORGOTO ("add_formation()", handle_error);
	}

	return (SPWERR_OK);

handle_error:
	safe_free (dst->raw);
	return (rc);
}

static SPWAW_ERROR
sec35_save_formations (FORMATION *src, BYTE player, SPWAW_SNAP_OOB_FRAW *dst, STRTAB *stab)
{
	SPWAW_ERROR	rc;
	USHORT		start, alt_start;
	FORMATION_LIST	fl;

	switch (player) {
		case PLAYER1:
			start = alt_start = FORMP1START;
			break;
		case PLAYER2:
			start = FORMP2START;
			alt_start = FORMP1START;
			break;
		default:
			// TODO: replace with some smarter RWE macro?
			ERROR1 ("internal error: unknown player ID %u", player);
			return (SPWERR_FAILED);
			break;
	}

	rc = build_formations (src, player, start, alt_start, fl);
	ROE ("build_formations_list()");

	rc = add_formations (src, dst, fl, stab);
	ROE ("add_formations()");

	rc = build_fridx (dst);
	ROE ("build_fridx()");

	return (SPWERR_OK);
}

SPWAW_ERROR
sec35_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab)
{
	SPWAW_ERROR	rc;

	CNULLARG (src); CNULLARG (dst); CNULLARG (stab);

	rc = sec35_save_formations (src->sec35.u.d.formations, PLAYER1, &(dst->raw.OOBp1.formations), stab);
	ROE ("sec35_save_formations(OOBp1)");

	rc = sec35_save_formations (src->sec35.u.d.formations, PLAYER2, &(dst->raw.OOBp2.formations), stab);
	ROE ("sec35_save_formations(OOBp2)");

	return (SPWERR_OK);
}
