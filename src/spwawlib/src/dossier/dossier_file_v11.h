/** \file
 * The SPWaW Library - dossier handling - backwards compatibility with the V11 dossier.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_DOSSIER_FILE_V11_H
#define	INTERNAL_DOSSIER_FILE_V11_H	1

#include <spwawlib_api.h>
#include <spwawlib_types.h>
#include "dossier/dossier_file.h"

#define	DOSS_VERSION_V11	11

#pragma pack(push, r1, 1)

typedef struct s_DOS_HEADER_V11 {
	ULONG		name;			/* Dossier name	symbol								*/
	ULONG		comment;		/* Dossier comment symbol							*/
	ULONG		oobdir;			/* Original OOB data directory symbol						*/
	ULONG		oobdata;		/* Dossier OOB data list offset, relative to start of header			*/
	ULONG		OOB;			/* OOB ID									*/
	USHORT		fcnt;			/* Core formations count							*/
	USHORT		ucnt;			/* Core units count								*/
	USHORT		bcnt;			/* Battle list count								*/
	ULONG		blist;			/* Battle list offset, relative to start of header				*/
	ULONG		stab;			/* String table offset, relative to start of header				*/
	ULONG		type;			/* Dossier type									*/
} DOS_HEADER_V11;

typedef struct s_DOS_BHEADER_V11 {
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
	ULONG		name;			/* optional battle name symbol								*/
} DOS_BHEADER_V11;

#pragma pack(pop, r1)

extern SPWAW_ERROR	dossier_load_v11_header		(int fd, DOS_HEADER *hdr);
extern SPWAW_ERROR	dossier_load_v11_battle_headers	(int fd, DOS_BHEADER *hdrs, USHORT cnt);

#endif	/* INTERNAL_DOSSIER_FILE_V11_H */