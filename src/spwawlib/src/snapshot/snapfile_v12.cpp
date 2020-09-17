/** \file
 * The SPWaW Library - snapshot handling - backwards compatibility with the V12 snapshot.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "snapshot/snapfile_v12.h"
#include "fileio/fileio.h"
#include "common/internal.h"

SPWAW_ERROR
snapshot_load_v12_info_header (int fd, SNAP_INFO *hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SNAP_INFO_V12	hdr_v12;

	if (!hdr) return (SPWERR_NULLARG);

	if (!bread (fd, (char *)&hdr_v12, sizeof (SNAP_INFO_V12), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(snapshot info hdr v12) failed", handle_error);

	/* A V12 snapshot info header only lacks the save type at the end.
	 * The only supported save type for V12 snapshots is the SPWAW_SAVE_TYPE_REGULAR,
	 * so a quick copy and fix up is all we need :)
	 */
	memcpy (hdr, &hdr_v12, sizeof (SNAP_INFO_V12));
	hdr->savetype = SPWAW_SAVE_TYPE_REGULAR;

handle_error:
	return (rc);
}
