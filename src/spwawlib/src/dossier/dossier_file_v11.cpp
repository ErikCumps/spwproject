/** \file
 * The SPWaW Library - dossier handling - backwards compatibility with the V11 dossier.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
//#include "spwoob/spwoob_list.h"
#include "dossier/dossier_file_v11.h"
#include "fileio/fileio.h"
#include "common/internal.h"

SPWAW_ERROR
dossier_load_v11_header (int fd, DOS_HEADER *hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DOS_HEADER_V11	*hdr_v11;

	if (!hdr) return (SPWERR_NULLARG);

	hdr_v11 = safe_malloc (DOS_HEADER_V11); COOMGOTO (hdr_v11, "DOS_HEADER_V11", handle_error);

	if (!bread (fd, (char *)hdr_v11, sizeof (DOS_HEADER_V11), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(dossier hdr v11)", handle_error);

	/* A V11 dossier header:
	 * + lacks the dossier game type at the end
	 *
	 * V11 dossiers only support the SPWAW_GAME_TYPE_SPWAW game type.
	 *
	 * So a quick copy and fix up is all we need :)
	 */
	memcpy (hdr, hdr_v11, sizeof (DOS_HEADER_V11));
	hdr->gametype = SPWAW_GAME_TYPE_SPWAW;

handle_error:
	if (hdr_v11) safe_free (hdr_v11);
	return (rc);
}
