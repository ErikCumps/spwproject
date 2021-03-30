/** \file
 * The SPWaW Library - snapshot handling - backwards compatibility with the V14 snapshot.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "snapshot/snapfile_v14.h"
#include "common/internal.h"

SPWAW_ERROR
snapshot_load_v14_map_header (int fd, SNAP_MAPHDR *hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SNAP_MAPHDR_V14	hdr_v14;

	if (!hdr) return (SPWERR_NULLARG);

	if (!bread (fd, (char *)&hdr_v14, sizeof (SNAP_MAPHDR_V14), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(snapshot map data hdr v14) failed", handle_error);

	/* The V14 snapshot map data header lacks the reference field,
	 * because V14 snapshots always contain map data. */
	hdr->reference	= false;
	hdr->width	= hdr_v14.width;
	hdr->height	= hdr_v14.height;
	hdr->data	= hdr_v14.data;
	hdr->size	= hdr_v14.size;
	hdr->comp	= hdr_v14.comp;

handle_error:
	return (rc);
}
