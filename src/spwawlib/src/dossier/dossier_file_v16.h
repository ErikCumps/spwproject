/** \file
 * The SPWaW Library - dossier handling - backwards compatibility with the V16 dossier.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_DOSSIER_FILE_V16_H
#define	INTERNAL_DOSSIER_FILE_V16_H	1

#include <spwawlib_api.h>
#include <spwawlib_types.h>
#include "dossier/dossier_file.h"

#define	DOSS_VERSION_V16	16

#pragma pack(push, r1, 1)

typedef struct s_DOS_BMHEADER_V16 {
	DWORD	width;				/* Battle map width								*/
	DWORD	height;				/* Battle map height								*/
	struct s_raw {				/* Battle map raw data:								*/
		ULONG	data;			/* data offset, relative to start of DOS_BHEADER table				*/
		ULONG	size;			/* data size									*/
		ULONG	comp;			/* compressed data size (0 if no compression)					*/
	}	raw;
	struct s_map {				/* Battle map data:								*/
		ULONG	data;			/* data offset, relative to start of DOS_BHEADER table				*/
		ULONG	size;			/* data size									*/
		ULONG	comp;			/* compressed data size (0 if no compression)					*/
	}	map;
} DOS_BMHEADER_V16;

typedef struct s_DOS_BHEADER_V16 {
	SPWAW_TIMESTAMP	date;			/* Battle date timestamp							*/
	ULONG		location;		/* Battle location symbol							*/
	USHORT		OOB_p1;			/* Player OOB ID								*/
	USHORT		OOB_p2;			/* Opponent OOB ID								*/
	ULONG		miss_p1;		/* Player mission symbol							*/
	ULONG		miss_p2;		/* Opponent mission symbol							*/
	BYTE		meeting;		/* Meeting engagement flag							*/
	USHORT		tcnt;			/* Battle turn list count							*/
	ULONG		tlist;			/* Battle turn list offset, relative to start of DOS_BHEADER table		*/
	struct s_ra {				/* Battle unit reassignment list:						*/
		ULONG	data;			/* data offset, relative to start of DOS_BHEADER table				*/
		ULONG	size;			/* data size									*/
		ULONG	comp;			/* compressed data size (0 if no compression)					*/
	}	ra;
	ULONG		oobdat;			/* Battle OOB data index in dossier OOB list					*/
	ULONG		name;			/* optional battle name symbol							*/
	USHORT		btlidx;			/* Campaign battle index							*/
	USHORT		racnt;			/* Battle unit reassignment list element count					*/
	DOS_BMHEADER	map;			/* Battle map data:								*/
} DOS_BHEADER_V16;

#pragma pack(pop, r1)

extern SPWAW_ERROR	dossier_load_v16_battle_headers	(int fd, DOS_BHEADER *hdrs, USHORT cnt);

#endif	/* INTERNAL_DOSSIER_FILE_V16_H */
