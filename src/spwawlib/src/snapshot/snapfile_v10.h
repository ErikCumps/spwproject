/** \file
 * The SPWaW Library - snapshot handling - backwards compatibility with the V10 snapshot.
 *
 * Copyright (C) 2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_SNAPFILE_V10_H
#define	INTERNAL_SNAPFILE_V10_H	1

#include <spwawlib_api.h>
#include <spwawlib_types.h>
#include "snapshot/snapfile.h"

#define	SNAP_VERSION_V10	10

#pragma pack(push, r1, 1)

typedef struct s_SNAP_INFO_V10 {
	ULONG		title;			/* Title symbol								*/
	SPWAW_TIMESTAMP	start;			/* Battle start date/time stamp						*/
	SPWAW_TIMESTAMP	date;			/* Battle turn date/time stamp						*/
	BYTE		turn;			/* Battle turn number							*/
	ULONG		location;		/* Battle location symbol						*/
} SNAP_INFO_V10;

#pragma pack(pop, r1)

extern SPWAW_ERROR	snapshot_load_v10_info_header	(int fd, SNAP_INFO *hdr);

#endif	/* INTERNAL_SNAPFILE_V10_H */
