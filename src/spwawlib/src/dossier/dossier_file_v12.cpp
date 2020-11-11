/** \file
 * The SPWaW Library - dossier handling - backwards compatibility with the V12 dossier.
 *
 * Copyright (C) 2019-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "dossier/dossier_file_v12.h"
#include "fileio/fileio.h"
#include "common/internal.h"

SPWAW_ERROR
dossier_load_v12_header (int fd, DOS_HEADER *hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DOS_HEADER_V12	*hdr_v12;

	if (!hdr) return (SPWERR_NULLARG);

	hdr_v12 = safe_malloc (DOS_HEADER_V12); COOMGOTO (hdr_v12, "DOS_HEADER_V12", handle_error);

	if (!bread (fd, (char *)hdr_v12, sizeof (DOS_HEADER_V12), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(dossier hdr v12)", handle_error);

	/* A V12 dossier header:
	 * + lacks the savedir
	 * + lacks the initial and current core force campaign properties
	 * + lacks the UHT data
	 * + lacks the savegame tracking info
	 *
	 * V12 dossiers do not support savegame tracking.
	 */
	hdr->name		= hdr_v12->name;
	hdr->comment		= hdr_v12->comment;
	hdr->savedir		= BADSTRIDX;
	hdr->oobdir		= hdr_v12->oobdir;
	hdr->oobdata		= hdr_v12->oobdata;
	hdr->bcnt		= hdr_v12->bcnt;
	hdr->blist		= hdr_v12->blist;
	hdr->stab		= hdr_v12->stab;
	hdr->type		= hdr_v12->type;
	hdr->gametype		= hdr_v12->gametype;
	
	hdr->props.OOB		= hdr_v12->props.OOB;
	hdr->props.start	= hdr_v12->props.start;
	hdr->props.end		= hdr_v12->props.end;
	hdr->props.maxbtlcnt	= hdr_v12->props.maxbtlcnt;
	hdr->props.ifcnt	= hdr_v12->props.fcnt;
	hdr->props.iucnt	= hdr_v12->props.ucnt;
	hdr->props.cfcnt	= hdr->props.ifcnt;
	hdr->props.cucnt	= hdr->props.iucnt;

	hdr->uht		= 0;

	hdr->tracking.gametype	= SPWAW_GAME_TYPE_UNKNOWN;
	hdr->tracking.savetype	= SPWAW_SAVE_TYPE_UNKNOWN;
	hdr->tracking.oobdir	= BADSTRIDX;
	hdr->tracking.path	= BADSTRIDX;
	hdr->tracking.base	= BADSTRIDX;
	hdr->tracking.filename	= BADSTRIDX;
	hdr->tracking.filedate	= 0;

handle_error:
	if (hdr_v12) safe_free (hdr_v12);
	return (rc);
}

SPWAW_ERROR
dossier_load_v12_battle_headers	(int fd, DOS_BHEADER *hdrs, USHORT cnt)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DOS_BHEADER_V12	*hdrs_v12;
	USHORT		i;

	if (!hdrs || !cnt) return (SPWERR_NULLARG);

	hdrs_v12 = safe_nmalloc (DOS_BHEADER_V12, cnt); COOMGOTO (hdrs_v12, "DOS_BHEADER_V12 list", handle_error);

	if (!bread (fd, (char *)hdrs_v12, cnt * sizeof (DOS_BHEADER_V12), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(battle hdrs v12)", handle_error);

	/* A V12 battle header:
	 * + lacks the battle unit reassignment list element count (at the end)
	 *
	 * For V12 dossiers the RA info is no longer loaded.
	 *
	 * So a quick copy and fix up is all we need :)
	 */
	for (i=0; i<cnt; i++) {
		memcpy (&(hdrs[i]), &(hdrs_v12[i]), sizeof (DOS_BHEADER_V12));
		hdrs[i].racnt = 0;
	}

handle_error:
	if (hdrs_v12) safe_free (hdrs_v12);
	return (rc);
}