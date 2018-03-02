/** \file
 * The SPWaW Library - dossier handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "dossier/dossier.h"
#include "dossier/dossier_file.h"
#include "snapshot/snapshot.h"
#include "fileio/fileio.h"
#include "utils/textconv.h"
#include "common/internal.h"

#define	ROF(b)	do { if (!b) { rc = SPWERR_FRFAILED; ERRORGOTO ("TC_v2t()", handle_error); } } while (0)

static SPWAW_ERROR
dossier_export_hdr (int src, FILE *dst, long &pos, DOS_HEADER &hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	bool		b;

	pos = bseekget (src);

	if (!bread (src, (char *)&hdr, sizeof (hdr), false)) rc = SPWERR_FRFAILED;
	ERRORGOTO ("bread(header)", handle_error);

	b = TC_v2t (dst, "dossier.header.magic",	TC_DATA_ASCIIZ, &(hdr.magic), sizeof (hdr.magic)); ROF(b);
	b = TC_v2t (dst, "dossier.header.version",	TC_DATA_ULONG, &(hdr.version));	ROF(b);
	b = TC_v2t (dst, "dossier.header.name",		TC_DATA_ULONG, &(hdr.name));	ROF(b);
	b = TC_v2t (dst, "dossier.header.comment",	TC_DATA_ULONG, &(hdr.comment));	ROF(b);
	b = TC_v2t (dst, "dossier.header.oobdir",	TC_DATA_ULONG, &(hdr.oobdir));	ROF(b);
	b = TC_v2t (dst, "dossier.header.OOB",		TC_DATA_BYTE, &(hdr.OOB));	ROF(b);
	b = TC_v2t (dst, "dossier.header.fcnt",		TC_DATA_USHORT, &(hdr.fcnt));	ROF(b);
	b = TC_v2t (dst, "dossier.header.ucnt",		TC_DATA_USHORT, &(hdr.ucnt));	ROF(b);
	b = TC_v2t (dst, "dossier.header.bcnt",		TC_DATA_USHORT, &(hdr.bcnt));	ROF(b);
	b = TC_v2t (dst, "//dossier.header.blist",	TC_DATA_ULONG, &(hdr.blist));	ROF(b);
	b = TC_v2t (dst, "//dossier.header.stab",	TC_DATA_ULONG, &(hdr.stab));	ROF(b);

	return (SPWERR_OK);

handle_error:
	return (rc);
}

static SPWAW_ERROR
dossier_export_tlist (int src, FILE *dst, long pos, USHORT cnt)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DOS_THEADER	*hdrs = NULL;
	USHORT		i;

	bseekset (src, pos);

	hdrs = safe_nmalloc (DOS_THEADER, cnt); COOMGOTO (hdrs, "DOS_THEADER list", handle_error);

	if (!bread (src, (char *)hdrs, cnt * sizeof (DOS_THEADER), false)) rc = SPWERR_FRFAILED;
	ERRORGOTO ("bread(turn headers)", handle_error);

	for (i=0; i<cnt; i++) {
		bool	b;

		b = TC_v2t (dst, "dossier.turn.header.date",	TC_DATA_IDX_TIMESTAMP, &(hdrs[i].date), 0, i);	ROF(b);
		b = TC_v2t (dst, "dossier.turn.header.turn",	TC_DATA_IDX_BYTE, &(hdrs[i].turn), 0, i);	ROF(b);
		b = TC_v2t (dst, "//dossier.turn.header.snap",	TC_DATA_IDX_ULONG, &(hdrs[i].snap), 0, i);	ROF(b);

		bseekset (src, pos + hdrs[i].snap);
		rc = snapexport (src, dst);
		ERRORGOTO ("snapexport()", handle_error);
	}

handle_error:
	if (hdrs) free (hdrs);
	return (rc);
}

static SPWAW_ERROR
dossier_export_blist (int src, FILE *dst, long pos, USHORT cnt, USHORT ucnt)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	DOS_BHEADER		*hdrs = NULL;
	SPWAW_DOSSIER_BURA	*ra = NULL;
	USHORT			i, j;

	bseekset (src, pos);

	hdrs = safe_nmalloc (DOS_BHEADER, cnt); COOMGOTO (hdrs, "DOS_BHEADER list", handle_error);

	if (!bread (src, (char *)hdrs, cnt * sizeof (DOS_BHEADER), false)) rc = SPWERR_FRFAILED;
	ERRORGOTO ("bread(battle headers)", handle_error);

	for (i=0; i<cnt; i++) {
		bool	b;

		b = TC_v2t (dst, "dossier.battle.header.date",		TC_DATA_IDX_TIMESTAMP, &(hdrs[i].date), 0, i);	ROF(b);
		b = TC_v2t (dst, "dossier.battle.header.location",	TC_DATA_IDX_ULONG, &(hdrs[i].location), 0, i);	ROF(b);
		b = TC_v2t (dst, "dossier.battle.header.OOB_p1",	TC_DATA_IDX_BYTE, &(hdrs[i].OOB_p1), 0, i);	ROF(b);
		b = TC_v2t (dst, "dossier.battle.header.OOB_p2",	TC_DATA_IDX_BYTE, &(hdrs[i].OOB_p2), 0, i);	ROF(b);
		b = TC_v2t (dst, "dossier.battle.header.miss_p1",	TC_DATA_IDX_ULONG, &(hdrs[i].miss_p1), 0, i);	ROF(b);
		b = TC_v2t (dst, "dossier.battle.header.miss_p2",	TC_DATA_IDX_ULONG, &(hdrs[i].miss_p2), 0, i);	ROF(b);
		b = TC_v2t (dst, "dossier.battle.header.meeting",	TC_DATA_IDX_BYTE, &(hdrs[i].meeting), 0, i);	ROF(b);
		b = TC_v2t (dst, "dossier.battle.header.tcnt",		TC_DATA_IDX_USHORT, &(hdrs[i].tcnt), 0, i);	ROF(b);
		b = TC_v2t (dst, "//dossier.battle.header.tlist",	TC_DATA_IDX_ULONG, &(hdrs[i].tlist), 0, i);	ROF(b);
		b = TC_v2t (dst, "//dossier.battle.header.ra",		TC_DATA_IDX_ULONG, &(hdrs[i].ra), 0, i);	ROF(b);

		rc = dossier_export_tlist (src, dst, pos + hdrs[i].tlist, hdrs[i].tcnt);
		ERRORGOTO ("dossier_export_tlist()", handle_error);

		ra = safe_nmalloc (SPWAW_DOSSIER_BURA, ucnt); COOMGOTO (ra, "RA list", handle_error);

		bseekset (src, pos + hdrs[i].ra.data);
		if (!bread (src, (char *)ra, ucnt * sizeof (SPWAW_DOSSIER_BURA), false)) rc = SPWERR_FRFAILED;
		ERRORGOTO ("bread(battle unit ra)", handle_error);

		for (j=0; j<ucnt; j++) {
			b = TC_v2t (dst, "dossier.battle.ra.src",	TC_DATA_IDX_USHORT, &(ra[j].src), 0, j);	ROF(b);
			b = TC_v2t (dst, "dossier.battle.ra.dst",	TC_DATA_IDX_USHORT, &(ra[j].dst), 0, j);	ROF(b);
			b = TC_v2t (dst, "dossier.battle.ra.rpl",	TC_DATA_IDX_BOOL, &(ra[j].rpl), 0, j);		ROF(b);
		}
	}

handle_error:
	if (ra) free (ra);
	if (hdrs) free (hdrs);
	return (rc);
}

static SPWAW_ERROR
dossier_export_stab (int src, FILE *dst, long pos)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	bseekset (src, pos);
	rc = STRTAB_export (src, dst);
	ERRORGOTO ("STRTAB_export()", handle_error);

	return (SPWERR_OK);

handle_error:
	return (rc);
}

SPWAW_ERROR
dossier_export (int src, FILE *dst)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		pos;
	DOS_HEADER	hdr;

	CNULLARG (dst);

	rc = dossier_export_hdr (src, dst, pos, hdr);
	ERRORGOTO ("dossier_export_hdr()", handle_error);

	rc = dossier_export_blist (src, dst, pos + hdr.blist, hdr.bcnt, hdr.ucnt);
	ERRORGOTO ("dossier_export_blist()", handle_error);

	rc = dossier_export_stab (src, dst, pos + hdr.stab);
	ERRORGOTO ("dossier_export_stab()", handle_error);

	return (SPWERR_OK);

handle_error:
	return (rc);
}
