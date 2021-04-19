/** \file
 * The SPWaW Library - dossier handling - backwards compatibility with the V15 dossier.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "dossier/dossier_file_v15.h"
#include "fileio/fileio.h"
#include "common/internal.h"

SPWAW_ERROR
dossier_load_v15_battle_headers	(int fd, DOS_BHEADER *hdrs, USHORT cnt)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DOS_BHEADER_V15	*hdrs_v15;
	USHORT		i;

	if (!hdrs || !cnt) return (SPWERR_NULLARG);

	hdrs_v15 = safe_nmalloc (DOS_BHEADER_V15, cnt); COOMGOTO (hdrs_v15, "DOS_BHEADER_V12 list", handle_error);

	if (!bread (fd, (char *)hdrs_v15, cnt * sizeof (DOS_BHEADER_V15), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(battle hdrs v15)", handle_error);

	/* A V15 battle header:
	 * + lacks the battle map data (at the end)
	 *
	 * For V15 dossiers there is no separately saved battle map data.
	 *
	 * So a quick copy and fix up is all we need :)
	 */
	for (i=0; i<cnt; i++) {
		memcpy (&(hdrs[i]), &(hdrs_v15[i]), sizeof (DOS_BHEADER_V15));
		hdrs[i].map.width = hdrs[i].map.height = 0;
		hdrs[i].map.raw.data = hdrs[i].map.raw.size = hdrs[i].map.raw.comp = 0;
		hdrs[i].map.map.data = hdrs[i].map.map.size = hdrs[i].map.map.comp = 0;
	}

handle_error:
	if (hdrs_v15) safe_free (hdrs_v15);
	return (rc);
}
