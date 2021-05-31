/** \file
 * The SPWaW Library - dossier handling - backwards compatibility with the V16 dossier.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "dossier/dossier_file_v16.h"
#include "fileio/fileio.h"
#include "common/internal.h"

SPWAW_ERROR
dossier_load_v16_battle_headers	(int fd, DOS_BHEADER *hdrs, USHORT cnt)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DOS_BHEADER_V16	*hdrs_v16;
	USHORT		i;

	if (!hdrs || !cnt) return (SPWERR_NULLARG);

	hdrs_v16 = safe_nmalloc (DOS_BHEADER_V16, cnt); COOMGOTO (hdrs_v16, "DOS_BHEADER_V16 list", handle_error);

	if (!bread (fd, (char *)hdrs_v16, cnt * sizeof (DOS_BHEADER_V16), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(battle hdrs v16)", handle_error);

	/* A V16 battle header:
	 * + lacks the battle location display name (at the end)
	 *
	 * For V16 dossiers the battle location display name is not stored.
	 *
	 * So a quick copy and fix up is all we need :)
	 */
	for (i=0; i<cnt; i++) {
		memcpy (&(hdrs[i]), &(hdrs_v16[i]), sizeof (DOS_BHEADER_V16));
		hdrs[i].location = BADSTRIDX;
	}

handle_error:
	if (hdrs_v16) safe_free (hdrs_v16);
	return (rc);
}
