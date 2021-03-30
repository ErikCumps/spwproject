/** \file
 * The SPWaW Library - snapshot handling - backwards compatibility with the V14 snapshot.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_SNAPFILE_V14_H
#define	INTERNAL_SNAPFILE_V14_H	1

#include <spwawlib_api.h>
#include "snapshot/snapfile.h"
#include "fileio/fileio.h"

#define	SNAP_VERSION_V14	14

#pragma pack(push, r1, 1)

typedef struct s_SNAP_MAPHDR_V14 {
	DWORD		width;			/* Battle map width							*/
	DWORD		height;			/* Battle map height							*/
	ULONG		data;			/* Map data offset, relative to start of header				*/
	ULONG		size;			/* data size								*/
	ULONG		comp;			/* compressed data size (0 if no compression)				*/
} SNAP_MAPHDR_V14;

#pragma pack(pop, r1)

extern SPWAW_ERROR	snapshot_load_v14_map_header	(int fd, SNAP_MAPHDR *hdr);

#endif	/* INTERNAL_SNAPFILE_V14_H */
