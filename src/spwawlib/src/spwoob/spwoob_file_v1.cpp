/** \file
 * The SPWaW Library - SPWaW OOB handling - backwards comaptibility with the V1 SPWOOB.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "spwoob/spwoob_file_v1.h"
#include "fileio/fileio.h"
#include "common/internal.h"

SPWAW_ERROR
spwoob_load_v1_header (int fd, SPWOOB_HEADER *hdr)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	SPWOOB_HEADER_V1	*hdr_v1;

	if (!hdr) return (SPWERR_NULLARG);

	hdr_v1 = safe_malloc (SPWOOB_HEADER_V1); COOMGOTO (hdr_v1, "SPWOOB_HEADER_V1", handle_error);

	if (!bread (fd, (char *)hdr_v1, sizeof (SPWOOB_HEADER_V1), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(SPWOOB hdr v1)", handle_error);

	/* A V1 SPWOOB header lacks the game type at the end.
	 * V1 SPWOOB headers only support the SPWAW_GAME_TYPE_SPWAW game type.
	 *
	 * So a quick copy and fix up is all we need :)
	 */
	memcpy (hdr, hdr_v1, sizeof (SPWOOB_HEADER_V1));
	hdr->gametype = SPWAW_GAME_TYPE_SPWAW;

handle_error:
	if (hdr_v1) safe_free (hdr_v1);
	return (rc);
}
