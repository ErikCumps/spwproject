/** \file
 * The SPWaW Library - dossier handling - backwards compatibility with the V14 dossier.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "dossier/dossier_file_v14.h"
#include "fileio/fileio.h"
#include "common/internal.h"

SPWAW_ERROR
dossier_load_v14_header (int fd, DOS_HEADER *hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DOS_HEADER_V14	*hdr_v14;

	if (!hdr) return (SPWERR_NULLARG);

	hdr_v14 = safe_malloc (DOS_HEADER_V14); COOMGOTO (hdr_v14, "DOS_HEADER_V14", handle_error);

	if (!bread (fd, (char *)hdr_v14, sizeof (DOS_HEADER_V14), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(dossier hdr V14)", handle_error);

	/* A V14 dossier header:
	 * + lacks the savedir
	 */
	hdr->name		= hdr_v14->name;
	hdr->comment		= hdr_v14->comment;
	hdr->savedir		= BADSTRIDX;
	hdr->oobdir		= hdr_v14->oobdir;
	hdr->oobdata		= hdr_v14->oobdata;
	hdr->bcnt		= hdr_v14->bcnt;
	hdr->blist		= hdr_v14->blist;
	hdr->stab		= hdr_v14->stab;
	hdr->type		= hdr_v14->type;
	hdr->gametype		= hdr_v14->gametype;

	hdr->props.OOB		= hdr_v14->props.OOB;
	hdr->props.start	= hdr_v14->props.start;
	hdr->props.end		= hdr_v14->props.end;
	hdr->props.maxbtlcnt	= hdr_v14->props.maxbtlcnt;
	hdr->props.ifcnt	= hdr_v14->props.ifcnt;
	hdr->props.iucnt	= hdr_v14->props.iucnt;
	hdr->props.cfcnt	= hdr_v14->props.cfcnt;
	hdr->props.cucnt	= hdr_v14->props.cucnt;

	hdr->uht		= hdr_v14->uht;

	hdr->tracking.gametype	= hdr_v14->tracking.gametype;
	hdr->tracking.savetype	= hdr_v14->tracking.savetype;
	hdr->tracking.oobdir	= hdr_v14->tracking.oobdir;
	hdr->tracking.path	= hdr_v14->tracking.path;
	hdr->tracking.base	= hdr_v14->tracking.base;
	hdr->tracking.filename	= hdr_v14->tracking.filename;
	hdr->tracking.filedate	= hdr_v14->tracking.filedate;

handle_error:
	if (hdr_v14) safe_free (hdr_v14);
	return (rc);
}
