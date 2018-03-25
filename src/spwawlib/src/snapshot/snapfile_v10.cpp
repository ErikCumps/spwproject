/** \file
 * The SPWaW Library - snapshot handling - backwards compatibility with the V10 snapshot.
 *
 * Copyright (C) 2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "snapshot/snapfile_v10.h"
#include "fileio/fileio.h"
#include "common/internal.h"

SPWAW_ERROR
snapshot_load_v10_info_header (int fd, SNAP_INFO *hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SNAP_INFO_V10	*hdr_v10;

	if (!hdr) return (SPWERR_NULLARG);

	hdr_v10 = safe_malloc (SNAP_INFO_V10); COOMGOTO (hdr_v10, "SNAP_INFO_V10", handle_error);

	if (!bread (fd, (char *)hdr_v10, sizeof (SNAP_INFO_V10), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(snapshot info hdr v10) failed", handle_error);

	/* A V10 snapshot info header only lacks the battle type at the end.
	 * The only supported battle type for V10 snapshots is the SPWAW_CAMPAIGN_BATTLE,
	 * so a quick copy and fix up is all we need :)
	 */
	memcpy (hdr, hdr_v10, sizeof (SNAP_INFO_V10));
	hdr->type = SPWAW_CAMPAIGN_BATTLE;

handle_error:
	if (hdr_v10) safe_free (hdr_v10);
	return (rc);

}