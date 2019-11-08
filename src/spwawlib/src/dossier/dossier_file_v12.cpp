/** \file
 * The SPWaW Library - dossier handling - backwards compatibility with the V12 dossier.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
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
	 * + lacks the initial and current core force campaign properties
	 */
	hdr->name	= hdr_v12->name;
	hdr->comment	= hdr_v12->comment;
	hdr->oobdir	= hdr_v12->oobdir;
	hdr->oobdata	= hdr_v12->oobdata;
	hdr->bcnt	= hdr_v12->bcnt;
	hdr->blist	= hdr_v12->blist;
	hdr->stab	= hdr_v12->stab;
	hdr->type	= hdr_v12->type;
	hdr->gametype	= hdr_v12->gametype;
	
	hdr->props.OOB		= hdr_v12->props.OOB;
	hdr->props.start	= hdr_v12->props.start;
	hdr->props.end		= hdr_v12->props.end;
	hdr->props.maxbtlcnt	= hdr_v12->props.maxbtlcnt;
	hdr->props.ifcnt	= hdr_v12->props.fcnt;
	hdr->props.iucnt	= hdr_v12->props.ucnt;
	hdr->props.cfcnt	= hdr->props.ifcnt;
	hdr->props.cucnt	= hdr->props.iucnt;

handle_error:
	if (hdr_v12) safe_free (hdr_v12);
	return (rc);
}
