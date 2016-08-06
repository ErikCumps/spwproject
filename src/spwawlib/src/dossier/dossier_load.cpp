#include "stdafx.h"
#include <spwawlib_api.h>
#include "dossier/dossier.h"
#include "dossier/dossier_file.h"
#include "snapshot/snapshot.h"
#include "strtab/strtab.h"
#include "fileio/fileio.h"
#include "common/internal.h"
#include "utils/compression.h"

static SPWAW_BTURN *
link_bturn (SPWAW_BTURN *p, SPWAW_BTURN *pp)
{
	p->prev = pp;
	p->next = NULL;

	if (p->prev) p->prev->next = p;

	return (p);
}

static SPWAW_ERROR
dossier_load_battle_turns (int fd, SPWAW_BATTLE *dst, USHORT cnt, STRTAB *stab)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		pos;
	DOS_THEADER	*hdrs;
	SPWAW_BTURN	*p, *pp;
	USHORT		i;

	pos = bseekget (fd);

	hdrs = safe_nmalloc (DOS_THEADER, cnt); COOMGOTO (hdrs, "DOS_THEADER list", handle_error);

	dst->tlen = cnt;
	dst->tlist = safe_nmalloc (SPWAW_BTURN *, cnt); COOMGOTO (dst->tlist, "SPWAW_BTURN* list", handle_error);

	if (!bread (fd, (char *)hdrs, cnt * sizeof (DOS_THEADER), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(snap hdrs)", handle_error);

	pp = NULL;
	for (i=0, pp=NULL; i<dst->tlen; i++) {
		p = safe_malloc (SPWAW_BTURN); COOMGOTO (p, "SPWAW_BTURN", handle_error);

		p->battle = dst;

		rc = SPWAW_stamp2date (&(hdrs[i].date), &(p->date));
		ERRORGOTO ("SPWAW_stamp2date(snap hdr date)", handle_error);

		p->turn = hdrs[i].turn;

		rc = snapnew (&(p->snap), dst->dossier->oobdat, stab);
		ERRORGOTO ("snapnew()", handle_error);

		bseekset (fd, pos + hdrs[i].snap);
		rc = snapload (fd, p->snap, stab);
		ERRORGOTO ("snapload()", handle_error);

		rc = snapint (p->snap);
		ERRORGOTO ("snapint()", handle_error);

		p->info.pbir.fcnt = p->snap->OOBp1.battle.formations.cnt;
		p->info.pbir.ucnt = p->snap->OOBp1.battle.units.cnt;
		p->info.obir.fcnt = p->snap->OOBp2.battle.formations.cnt;
		p->info.obir.ucnt = p->snap->OOBp2.battle.units.cnt;

		rc = dossier_prep_bturn_info (p);
		ERRORGOTO ("dossier_prep_bturn_info()", handle_error);

		dst->tlist[dst->tcnt++] = pp = link_bturn (p, pp);

		rc = dossier_fix_battle_info (p, dst->tlist[0]);
		ERRORGOTO ("dossier_fix_battle_info()", handle_error);
	}
	dst->tfirst = dst->tlist[0];
	dst->tlast  = dst->tlist[dst->tcnt-1];

handle_error:
	// dossier cleanup done by caller
	if (hdrs) free (hdrs);
	return (rc);

}

static SPWAW_BATTLE *
link_battle (SPWAW_BATTLE *p, SPWAW_BATTLE *pp)
{
	p->prev = pp;
	p->next = NULL;

	if (p->prev) p->prev->next = p;

	return (p);
}

static SPWAW_ERROR
dossier_load_battles (int fd, SPWAW_DOSSIER *dst, USHORT cnt, STRTAB *stab)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		pos;
	DOS_BHEADER	*hdrs;
	SPWAW_BATTLE	*p, *pp;
	USHORT		i;
	CBIO		cbio;

	if (!cnt) return (SPWERR_OK);

	pos = bseekget (fd);

	hdrs = safe_nmalloc (DOS_BHEADER, cnt); COOMGOTO (hdrs, "DOS_BHEADER list", handle_error);

	dst->blen = cnt;
	dst->blist = safe_nmalloc (SPWAW_BATTLE *, cnt); COOMGOTO (dst->blist, "SPWAW_BATTLE* list", handle_error);

	if (!bread (fd, (char *)hdrs, cnt * sizeof (DOS_BHEADER), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(battle hdrs)", handle_error);

	pp = NULL;
	for (i=0; i<dst->blen; i++) {
		p = safe_malloc (SPWAW_BATTLE); COOMGOTO (p, "SPWAW_BATTLE", handle_error);

		p->dossier = dst;

		rc = SPWAW_stamp2date (&(hdrs[i].date), &(p->date));
		ERRORGOTO ("SPWAW_stamp2date(battle hdr date)", handle_error);

		p->location = STRTAB_getstr (stab, hdrs[i].location);
		p->OOB      = hdrs[i].OOB;
		p->miss_p1  = STRTAB_getstr (stab, hdrs[i].miss_p1);
		p->miss_p2  = STRTAB_getstr (stab, hdrs[i].miss_p2);
		p->meeting  = hdrs[i].meeting != 0;

		bseekset (fd, pos + hdrs[i].tlist);
		rc = dossier_load_battle_turns (fd, p, hdrs[i].tcnt, stab);
		ERRORGOTO ("dossier_load_battle_turns()", handle_error);

		if (p->tcnt) p->snap = p->tlist[0]->snap;

		p->ra = safe_nmalloc (SPWAW_DOSSIER_BURA, dst->ucnt); COOMGOTO (p->ra, "RA list", handle_error);

		bseekset (fd, pos + hdrs[i].ra.data);

		cbio.data = (char *)(p->ra); cbio.size = hdrs[i].ra.size; cbio.comp = &(hdrs[i].ra.comp);
		if (!cbread (fd, cbio, "compressed battle unit ra"))
			FAILGOTO (SPWERR_FRFAILED, "cbread(compressed battle unit ra) failed", handle_error);

		rc = dossier_update_battle_info (p);
		ERRORGOTO ("dossier_update_battle_info()", handle_error);

		dst->blist[dst->bcnt++] = pp = link_battle (p, pp);
	}
	dst->bfirst = dst->blist[0];
	dst->blast  = dst->blist[dst->bcnt-1];

handle_error:
	// dossier cleanup done by caller
	if (hdrs) free (hdrs);
	return (rc);
}

static SPWAW_ERROR
dossier_check_header (DOS_HEADER &hdr)
{
	if ((memcmp (hdr.magic, DOSS_MAGIC, DOSS_MGCLEN) != 0) || (hdr.version != DOSS_VERSION))
		RWE (SPWERR_BADSAVEDATA, "dossier header check failed");

	if (hdr.oobdat == 0)
		RWE (SPWERR_BADSAVEDATA, "dossier contains no OOB data");

	if (hdr.stab == 0)
		RWE (SPWERR_BADSAVEDATA, "dossier contains no STAB data");

	return (SPWERR_OK);
}

SPWAW_ERROR
dossier_loadinfo (int fd, SPWAW_DOSSIER_INFO *dst)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		pos;
	DOS_HEADER	hdr;
	STRTAB		*stab = NULL;

	CNULLARG (dst);
	
	pos = bseekget (fd);

	memset (&hdr, 0, sizeof (hdr));

	if (!bread (fd, (char *)&hdr, sizeof (hdr), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(hdr)", handle_error);

	rc = dossier_check_header (hdr);
	ERRORGOTO ("dossier_check_header()", handle_error);

	rc = STRTAB_new (&stab);
	ERRORGOTO ("STRTAB_new()", handle_error);

	bseekset (fd, pos + hdr.stab);
	rc = STRTAB_fdload (stab, fd);
	ERRORGOTO ("STRTAB_fdload()", handle_error);

	snprintf (dst->name, sizeof (dst->name) - 1, "%s", STRTAB_getstr (stab, hdr.name));
	snprintf (dst->comment, sizeof (dst->comment) - 1, "%s", STRTAB_getstr (stab, hdr.comment));
	dst->OOB = hdr.OOB;
	dst->bcnt = hdr.bcnt;

	STRTAB_free (&stab);

	return (SPWERR_OK);

handle_error:
	if (stab) STRTAB_free (&stab);
	return (rc);
}

SPWAW_ERROR
dossier_load (int fd, SPWAW_DOSSIER *dst)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		pos;
	DOS_HEADER	hdr;
	STRTAB		*stab = NULL;

	CNULLARG (dst);
	stab = (STRTAB *)dst->stab;

	pos = bseekget (fd);

	memset (&hdr, 0, sizeof (hdr));

	if (!bread (fd, (char *)&hdr, sizeof (hdr), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(hdr)", handle_error);

	rc = dossier_check_header (hdr);
	ERRORGOTO ("dossier_check_header()", handle_error);

	bseekset (fd, pos + hdr.oobdat);
	rc = SPWOOB_load (dst->oobdat, fd);
	ERRORGOTO ("spwoob_load()", handle_error);

	bseekset (fd, pos + hdr.stab);
	rc = STRTAB_fdload (stab, fd);
	ERRORGOTO ("STRTAB_fdload()", handle_error);

	dst->name = STRTAB_getstr (stab, hdr.name);
	dst->comment = STRTAB_getstr (stab, hdr.comment);
	dst->oobdir = STRTAB_getstr (stab, hdr.oobdir);
	dst->OOB = hdr.OOB;
	dst->fcnt = hdr.fcnt;
	dst->ucnt = hdr.ucnt;

	bseekset (fd, pos + hdr.blist);
	rc = dossier_load_battles (fd, dst, hdr.bcnt, stab);
	ERRORGOTO ("dossier_load_battles()", handle_error);

	return (SPWERR_OK);

handle_error:
	dossier_clean (dst);
	return (rc);
}
