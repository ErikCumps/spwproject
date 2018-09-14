/** \file
 * The SPWaW Library - dossier handling - backwards compatibility with the V10 dossier.
 *
 * Copyright (C) 2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "dossier/dossier_file_v10.h"
#include "fileio/fileio.h"
#include "common/internal.h"

SPWAW_ERROR
dossier_load_v10_header (int fd, DOS_HEADER *hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DOS_HEADER_V10	*hdr_v10;

	if (!hdr) return (SPWERR_NULLARG);

	hdr_v10 = safe_malloc (DOS_HEADER_V10); COOMGOTO (hdr_v10, "DOS_HEADER_V10", handle_error);

	if (!bread (fd, (char *)hdr_v10, sizeof (DOS_HEADER_V10), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(dossier hdr v10)", handle_error);

	/* A V10 dossier header only lacks the dossier type at the end.
	 * The only supported dossier type for V10 snapshots is the SPWAW_CAMPAIGN_DOSSIER,
	 * so a quick copy and fix up is all we need :)
	 */
	memcpy (hdr, hdr_v10, sizeof (DOS_HEADER_V10));
	hdr->type = SPWAW_CAMPAIGN_DOSSIER;

handle_error:
	if (hdr_v10) safe_free (hdr_v10);
	return (rc);
}

SPWAW_ERROR
dossier_load_v10_battle_headers	(int fd, DOS_BHEADER *hdrs, USHORT cnt)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DOS_BHEADER_V10	*hdrs_v10;
	USHORT		i;

	if (!hdrs || !cnt) return (SPWERR_NULLARG);

	hdrs_v10 = safe_nmalloc (DOS_BHEADER_V10, cnt); COOMGOTO (hdrs_v10, "DOS_BHEADER_V10 list", handle_error);

	if (!bread (fd, (char *)hdrs_v10, cnt * sizeof (DOS_BHEADER_V10), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(battle hdrs v10)", handle_error);

	/* A V10 battle header only has a slightly different OOB organization and it lacks
	 * the battle name symbol at the end.
	 * The only supported dossier type for V10 snapshots is the SPWAW_CAMPAIGN_DOSSIER
	 * that doesn't use battle names, so a quick copy and fix up is all we need :)
	 */
	for (i=0; i<cnt; i++) {
		memcpy (&(hdrs[i]), &(hdrs_v10[i]), sizeof (DOS_BHEADER_V10));
		hdrs[i].OOB_p1 = (BYTE)((hdrs_v10[i].OOB >> 16) & 0xFF);
		hdrs[i].OOB_p2 = (BYTE)((hdrs_v10[i].OOB >>  0) & 0xFF);
		hdrs[i].name = BADSTRIDX;
	}

handle_error:
	if (hdrs_v10) safe_free (hdrs_v10);
	return (rc);
}
