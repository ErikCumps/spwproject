/** \file
 * The SPWaW Library - dossier handling - backwards compatibility with the V13 dossier.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "dossier/dossier_file_v13.h"
#include "fileio/fileio.h"
#include "common/internal.h"

SPWAW_ERROR
dossier_load_v13_header (int fd, DOS_HEADER *hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DOS_HEADER_V13	*hdr_v13;

	if (!hdr) return (SPWERR_NULLARG);

	hdr_v13 = safe_malloc (DOS_HEADER_V13); COOMGOTO (hdr_v13, "DOS_HEADER_V13", handle_error);

	if (!bread (fd, (char *)hdr_v13, sizeof (DOS_HEADER_V13), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(dossier hdr V13)", handle_error);

	/* A V13 dossier header:
	 * + lacks the savedir
	 * + lacks the savegame tracking info
	 *
	 * V13 dossiers do not support savegame tracking.
	 */
	hdr->name		= hdr_v13->name;
	hdr->comment		= hdr_v13->comment;
	hdr->savedir		= BADSTRIDX;
	hdr->oobdir		= hdr_v13->oobdir;
	hdr->oobdata		= hdr_v13->oobdata;
	hdr->bcnt		= hdr_v13->bcnt;
	hdr->blist		= hdr_v13->blist;
	hdr->stab		= hdr_v13->stab;
	hdr->type		= hdr_v13->type;
	hdr->gametype		= hdr_v13->gametype;

	hdr->props.OOB		= hdr_v13->props.OOB;
	hdr->props.start	= hdr_v13->props.start;
	hdr->props.end		= hdr_v13->props.end;
	hdr->props.maxbtlcnt	= hdr_v13->props.maxbtlcnt;
	hdr->props.ifcnt	= hdr_v13->props.ifcnt;
	hdr->props.iucnt	= hdr_v13->props.iucnt;
	hdr->props.cfcnt	= hdr_v13->props.cfcnt;
	hdr->props.cucnt	= hdr_v13->props.cucnt;

	hdr->uht		= hdr_v13->uht;

	hdr->tracking.gametype	= SPWAW_GAME_TYPE_UNKNOWN;
	hdr->tracking.savetype	= SPWAW_SAVE_TYPE_UNKNOWN;
	hdr->tracking.oobdir	= BADSTRIDX;
	hdr->tracking.path	= BADSTRIDX;
	hdr->tracking.base	= BADSTRIDX;
	hdr->tracking.filename	= BADSTRIDX;
	hdr->tracking.filedate	= 0;

handle_error:
	if (hdr_v13) safe_free (hdr_v13);
	return (rc);
}
