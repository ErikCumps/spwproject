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

	/* A V13 dossier header only lacks the savegame tracking info at the end.
	 *
	 * V13 dossiers do not support savegame tracking.
	 */
	memcpy (hdr, hdr_v13, sizeof (DOS_HEADER_V13));

	hdr->tracking.gametype	= SPWAW_GAME_TYPE_UNKNOWN;
	hdr->tracking.savetype	= SPWAW_SAVE_TYPE_UNKNOWN;
	hdr->tracking.path	= BADSTRIDX;
	hdr->tracking.base	= BADSTRIDX;
	hdr->tracking.filename	= BADSTRIDX;
	hdr->tracking.filedate	= 0;

handle_error:
	if (hdr_v13) safe_free (hdr_v13);
	return (rc);
}
