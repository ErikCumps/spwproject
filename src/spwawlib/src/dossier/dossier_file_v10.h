/** \file
 * The SPWaW Library - dossier handling - backwards compatibility with the V10 dossier.
 *
 * Copyright (C) 2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_DOSSIER_FILE_V10_H
#define	INTERNAL_DOSSIER_FILE_V10_H	1

#include <spwawlib_api.h>
#include <spwawlib_types.h>
#include "dossier/dossier_file.h"

#define	DOSS_VERSION_V10	10

#pragma pack(push, r1, 1)

typedef struct s_DOS_BHEADER_V10 {
	SPWAW_TIMESTAMP	date;			/* Battle date timestamp							*/
	ULONG		location;		/* Battle location symbol							*/
	ULONG		OOB;			/* Opponent OOB ID								*/
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
} DOS_BHEADER_V10;

#pragma pack(pop, r1)

extern SPWAW_ERROR	dossier_load_v10_battle_headers	(int fd, DOS_BHEADER *hdrs, USHORT cnt);

#endif	/* INTERNAL_DOSSIER_FILE_V10_H */
