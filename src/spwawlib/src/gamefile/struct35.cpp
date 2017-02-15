/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamefile.h"
#include "snapshot/index.h"
#include "utils/ud.h"
#include "common/internal.h"
#include <ad_list.h>

static SPWAW_ERROR
build_formations_list (FORMATION *src, BYTE player, USHORT start, USHORT end, FLIST &fl)
{
	int	seen[FORMCOUNT];
	USHORT	i;

	log ("find_formations: player #%u\n", player);

	memset (seen, 0, sizeof(seen));

	// Add all valid player formations
	for (i=start; i<end; i++)
	{
		if (src[i].leader == SPWAW_BADIDX) {
			// skipped: no leader
			log ("find_formations: [%3.3u] SKIPPED (no leader)\n", i);
			continue;
		}

		if (src[i].player != player) {
			// skipped: wrong player
			//log ("find_formations: [%3.3u] SKIPPED (wrong player ID %u)\n", i, src[i].player);
			continue;
		}


		if (seen[src[i].ID] != 0) {
			// skipped: duplicate formation ID
			//log ("find_formations: [%3.3u] SKIPPED (duplicate formation ID %u)\n", i, src[i].ID);
			continue;
		}

		FEL *fel = reserve_FEL (fl);
		if (!fel) RWE (SPWERR_FAILED, "reserve_FEL() failed");

		fel->d.RID    = i;
		fel->d.rawFID = src[i].ID;
		fel->d.player = src[i].player;
		fel->d.leader = src[i].leader;
		fel->d.OOBrid = src[i].OOBrid;
		memcpy (fel->d.name, src[i].name, SPWAW_AZSNAME);

		log ("find_formations: [%3.3u] FORMATION: P<%1.1u> ID<%3.3u> L<%5.5u> O<%3.3u>(%16.16s)\n",
			i, src[i].player, src[i].ID, src[i].leader, src[i].OOBrid, src[i].name);

		if (!commit_FEL (fl, fel)) {
			RWE (SPWERR_FAILED, "commit_fel() failed");
		}

		seen[src[i].ID] = 1;
	}
	log ("find_formations: cnt=%u\n", fl.cnt);

	return (SPWERR_OK);
}

static SPWAW_ERROR
build_formations (FORMATION *src, BYTE player, FLIST &fl)
{
	SPWAW_ERROR	rc;

	init_FLIST (fl);

	rc = build_formations_list (src, player, FORMP1START, FORMCOUNT, fl);
	ROE ("build_formations_list()");

	if (fl.cnt == 0) RWE (SPWERR_BADSAVEDATA, "no formations found");

	return (SPWERR_OK);
}

static SPWAW_ERROR
add_formation (USHORT rid, FORMATION *src, USHORT start, SPWAW_SNAP_OOB_FELRAW *dst, STRTAB *stab)
{
	SPWAW_UD		*UD;

	dst->RID	= rid;
	dst->FID	= src->ID - (BYTE)start;

	if ((dst->FID != 0) && (src->name[0] != '\0')) {
		dst->name = azstrstab (src->name, stab);
	} else {
		dst->name = NULL;
	}

	dst->leader	= src->leader;
	dst->hcmd	= src->hcmd;
	dst->OOBrid	= src->OOBrid;
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
add_formations (FORMATION *src, SPWAW_SNAP_OOB_FRAW *dst, FLIST &fl, STRTAB *stab)
{
	SPWAW_ERROR	rc;
	FEL		*p;
	USHORT		idx;
	USHORT		rid;

	if (fl.cnt == 0) RWE (SPWERR_FAILED, "unexpected empty formations list");

	dst->raw = safe_nmalloc (SPWAW_SNAP_OOB_FELRAW, fl.cnt); COOM (dst->raw, "SPWAW_SNAP_OOB_FELRAW list");
	dst->cnt   = fl.cnt;
	dst->start = src[fl.head->d.RID].ID;

	p = fl.head; idx = 0;
	while (p) {
		rid = p->d.RID;
		src[rid].OOBrid = p->d.OOBrid;
		rc = add_formation (rid, &src[rid], dst->start, &(dst->raw[idx]), stab);
		ERRORGOTO ("add_formation()", handle_error);

		p = p->l.next; idx++;
	}

	return (SPWERR_OK);

handle_error:
	safe_free (dst->raw);
	return (rc);
}

SPWAW_ERROR
sec35_detection (GAMEDATA *src, FULIST &ful1, FULIST &ful2)
{
	SPWAW_ERROR	rc;

	CNULLARG (src);

	rc = build_formations (src->sec35.u.d.formations, PLAYER1, ful1.fl);
	ROE ("build_formations_list(OOBp1)");

	rc = build_formations (src->sec35.u.d.formations, PLAYER2, ful2.fl);
	ROE ("build_formations_list(OOBp1)");

	return (SPWERR_OK);
}

static SPWAW_ERROR
sec35_save_formations (FORMATION *src, SPWAW_SNAP_OOB_FRAW *dst, STRTAB *stab, FLIST &fl)
{
	SPWAW_ERROR	rc;

	rc = add_formations (src, dst, fl, stab);
	ROE ("add_formations()");

	rc = build_fridx (dst);
	ROE ("build_fridx()");

	return (SPWERR_OK);
}

SPWAW_ERROR
sec35_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2)
{
	SPWAW_ERROR	rc;

	CNULLARG (src); CNULLARG (dst); CNULLARG (stab);

	rc = sec35_save_formations (src->sec35.u.d.formations, &(dst->raw.OOBp1.formations), stab, ful1.fl);
	ROE ("sec35_save_formations(OOBp1)");

	rc = sec35_save_formations (src->sec35.u.d.formations, &(dst->raw.OOBp2.formations), stab, ful2.fl);
	ROE ("sec35_save_formations(OOBp2)");

	return (SPWERR_OK);
}
