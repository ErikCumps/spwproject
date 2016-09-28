/** \file
 * The SPWaW Library - dossier handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_DOSSIER_FILE_H
#define	INTERNAL_DOSSIER_FILE_H	1

#include <spwawlib_api.h>
#include <spwawlib_types.h>
#include <ad_hash.h>

#define	DOSS_MAGIC	"SPWAWLIB_DOSSIER"
#define	DOSS_MGCLEN	16
#define	DOSS_VERSION	10

#pragma pack(push, r1, 1)

typedef struct s_DOS_HEADER {
	char		magic[DOSS_MGCLEN];	/* Dossier magic string								*/
	ULONG		version;		/* Dossier file format version							*/
	ULONG		name;			/* Dossier name	symbol								*/
	ULONG		comment;		/* Dossier comment symbol							*/
	ULONG		oobdir;			/* Original OOB data directory symbol						*/
	ULONG		oobdat;			/* OOB data offset, relative to start of header					*/
	ULONG		OOB;			/* OOB ID									*/
	USHORT		fcnt;			/* Core formations count							*/
	USHORT		ucnt;			/* Core units count								*/
	USHORT		bcnt;			/* Battle list count								*/
	ULONG		blist;			/* Battle list offset, relative to start of header				*/
	ULONG		stab;			/* String table offset, relative to start of header				*/
} DOS_HEADER;

typedef struct s_DOS_BHEADER {
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
} DOS_BHEADER;

typedef struct s_DOS_THEADER {
	SPWAW_TIMESTAMP	date;			/* Battle turn date timestamp							*/
	ULONG		turn;			/* Battle turn number								*/
	ULONG		snap;			/* Battle turn snapshot offset, relative to start of DOS_THEADER table		*/
} DOS_THEADER;

#pragma pack(pop, r1)

#endif	/* INTERNAL_DOSSIER_FILE_H */
