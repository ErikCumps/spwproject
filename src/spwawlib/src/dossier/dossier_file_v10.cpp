/** \file
 * The SPWaW Library - dossier handling - backwards compatibility with the V10 dossier.
 *
 * Copyright (C) 2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "dossier/dossier_file.h"
#include "dossier/dossier_file_v10.h"
#include "fileio/fileio.h"
#include "common/internal.h"

SPWAW_ERROR
dossier_load_v10_battle_headers	(int fd, DOS_BHEADER *hdrs, USHORT cnt)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DOS_BHEADER_V10	*hdrs_v10;
	USHORT		i;

	if (!cnt) return (SPWERR_OK);

	hdrs_v10 = safe_nmalloc (DOS_BHEADER_V10, cnt); COOMGOTO (hdrs_v10, "DOS_BHEADER_V10 list", handle_error);

	if (!bread (fd, (char *)hdrs_v10, cnt * sizeof (DOS_BHEADER_V10), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(battle hdrs v10)", handle_error);

	/* A V10 battle header only has a slightly different OOB organization,
	 * but it is just as long as the current battle header, so a quick copy
	 * and fix up is all we need :)
	 */
	memcpy (hdrs, hdrs_v10, cnt * sizeof (DOS_BHEADER));
	for (i=0; i<cnt; i++) {
		hdrs[i].OOB_p1 = (BYTE)((hdrs_v10[i].OOB >> 16) & 0xFF);
		hdrs[i].OOB_p2 = (BYTE)((hdrs_v10[i].OOB >>  0) & 0xFF);
	}

handle_error:
	if (hdrs_v10) safe_free (hdrs_v10);
	return (rc);
}
