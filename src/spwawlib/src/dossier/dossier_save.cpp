/** \file
 * The SPWaW Library - dossier handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "dossier/dossier.h"
#include "dossier/dossier_file.h"
#include "snapshot/snapshot.h"
#include "strtab/strtab.h"
#include "fileio/fileio.h"
#include "common/internal.h"
#include "utils/compression.h"

static SPWAW_ERROR
dossier_save_battle_turns (SPWAW_BATTLE *src, int fd, USHORT *cnt, STRTAB *stab, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		p0, p1;
	USHORT		i, idx;
	SPWAW_BTURN	*p = NULL;
	DOS_THEADER	*hdrs;

	CNULLARG (src); CNULLARG (cnt); CNULLARG (stab);
	*cnt = 0;

	p0 = bseekget (fd);

	hdrs = safe_nmalloc (DOS_THEADER, src->tcnt); COOMGOTO (hdrs, "DOS_THEADER list", handle_error);

	for (i=idx=0; i<src->tcnt; i++) {
		p = src->tlist[i]; if (!p) continue;

		rc = SPWAW_date2stamp (&(p->date), &(hdrs[idx].date));
		ERRORGOTO ("SPWAW_date2stamp(snap hdr date)", handle_error);

		hdrs[idx].turn = p->turn;

		bseekmove (fd, sizeof (DOS_THEADER));

		idx++;
	}

	*cnt = idx;

	for (i=idx=0; i<src->tcnt; i++) {
		p = src->tlist[i]; if (!p) continue;

		hdrs[idx].snap = bseekget (fd) - p0;
		snapsave (p->snap, fd, false, false, compress);

		idx++;
	}

	p1 = bseekget (fd); bseekset (fd, p0);
	if (!bwrite (fd, (char *)hdrs, (*cnt) * sizeof (DOS_THEADER)))
		FAILGOTO (SPWERR_FWFAILED, "bwrite(snap hdrs)", handle_error);
	bseekset (fd, p1);

	free (hdrs);

	return (SPWERR_OK);

handle_error:
	if (hdrs) free (hdrs);
	*cnt = 0;
	return (rc);
}

static SPWAW_ERROR
dossier_save_battles (SPWAW_DOSSIER *src, int fd, USHORT *cnt, STRTAB *stab, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		p0, p1;
	USHORT		i, idx;
	SPWAW_BATTLE	*p = NULL;
	DOS_BHEADER	*hdrs;
	CBIO		cbio;

	CNULLARG (src); CNULLARG (cnt); CNULLARG (stab);
	*cnt = 0;

	p0 = bseekget (fd);

	hdrs = safe_nmalloc (DOS_BHEADER, src->bcnt); COOMGOTO (hdrs, "DOS_BHEADER list", handle_error);

	for (i=idx=0; i<src->bcnt; i++) {
		p = src->blist[i]; if (!p) continue;

		rc = SPWAW_date2stamp (&(p->date), &(hdrs[idx].date));
		ERRORGOTO ("SPWAW_date2stamp(battle hdr date)", handle_error);

		hdrs[idx].location = STRTAB_getidx (stab, p->location);
		hdrs[idx].OOB      = p->OOB;
		hdrs[idx].miss_p1  = STRTAB_getidx (stab, p->miss_p1);
		hdrs[idx].miss_p2  = STRTAB_getidx (stab, p->miss_p2);
		hdrs[idx].meeting  = p->meeting;

		bseekmove (fd, sizeof (DOS_BHEADER));

		idx++;
	}

	*cnt = idx;

	for (i=idx=0; i<src->bcnt; i++) {
		p = src->blist[i]; if (!p) continue;

		hdrs[idx].tlist = bseekget (fd) - p0;
		rc = dossier_save_battle_turns (p, fd, &(hdrs[idx].tcnt), stab, compress);
		ERRORGOTO ("dossier_save_battle_turns()", handle_error);

		hdrs[idx].ra.data = bseekget (fd) - p0;
		hdrs[idx].ra.size = src->ucnt * sizeof (SPWAW_DOSSIER_BURA);

		cbio.data = (char *)(p->ra); cbio.size = hdrs[idx].ra.size; cbio.comp = &(hdrs[idx].ra.comp);
		if (!cbwrite (fd, cbio, "compressed battle unit ra", compress))
			FAILGOTO (SPWERR_FWFAILED, "cbwrite(compressed battle unit ra) failed", handle_error);

		idx++;
	}

	p1 = bseekget (fd); bseekset (fd, p0);
	if (!bwrite (fd, (char *)hdrs, (*cnt) * sizeof (DOS_BHEADER)))
		FAILGOTO (SPWERR_FWFAILED, "bwrite(battle headers)", handle_error);
	bseekset (fd, p1);

	free (hdrs);

	return (SPWERR_OK);

handle_error:
	if (hdrs) free (hdrs);
	*cnt = 0;
	return (rc);
}

SPWAW_ERROR
dossier_save (SPWAW_DOSSIER *src, int fd, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		p0, p1;
	DOS_HEADER	hdr;
	STRTAB		*stab = NULL;

	CNULLARG (src);
	stab = (STRTAB *)src->stab;

	STRTAB_compact (stab);

	p0 = bseekget (fd);

	memset (&hdr, 0, sizeof (hdr));

	memcpy (hdr.magic, DOSS_MAGIC, strlen (DOSS_MAGIC));
	hdr.version = DOSS_VERSION;

	hdr.name = STRTAB_getidx (stab, src->name);
	hdr.comment = STRTAB_getidx (stab, src->comment);
	hdr.oobdir = STRTAB_getidx (stab, src->oobdir);
	hdr.OOB = src->OOB;
	hdr.fcnt = src->fcnt;
	hdr.ucnt = src->ucnt;

	bseekmove (fd, sizeof (hdr));

	hdr.oobdat = bseekget (fd) - p0;
	rc = SPWOOB_save (src->oobdat, fd, compress);
	ROE ("spwoob_save()");

	hdr.blist = bseekget (fd) - p0;
	dossier_save_battles (src, fd, &(hdr.bcnt), stab, compress);

	hdr.stab = bseekget (fd) - p0;
	rc = STRTAB_fdsave (stab, fd, compress);
	ROE ("STRTAB_fdsave()");

	p1 = bseekget (fd); bseekset (fd, p0);
	if (!bwrite (fd, (char *)&hdr, sizeof (hdr)))
		RWE (SPWERR_FWFAILED, "bwrite(hdr) failed");
	bseekset (fd, p1);

	return (SPWERR_OK);
}
