/** \file
 * The SPWaW Library - dossier handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
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
#define	DOSS_VERSION	12

#pragma pack(push, r1, 1)

typedef struct s_DOS_MV_HEADER {
	char		magic[DOSS_MGCLEN];	/* Dossier magic string								*/
	ULONG		version;		/* Dossier file format version							*/
} DOS_MV_HEADER;

typedef struct s_DOS_CMPPROPS {
	BYTE		OOB;			/* OOB ID									*/
	USHORT		fcnt;			/* Player core formations count							*/
	USHORT		ucnt;			/* Player core units count							*/
	SPWAW_TIMESTAMP	start;			/* Campaign start date								*/
	SPWAW_TIMESTAMP	end;			/* Campaign end date								*/
	USHORT		maxbtlcnt;		/* Campaign maximum battles count						*/
} DOS_CMPPROPS;

typedef struct s_DOS_HEADER {
	ULONG		name;			/* Dossier name	symbol								*/
	ULONG		comment;		/* Dossier comment symbol							*/
	ULONG		oobdir;			/* Original OOB data directory symbol						*/
	ULONG		oobdata;		/* Dossier OOB data list offset, relative to start of header			*/
	USHORT		bcnt;			/* Battle list count								*/
	ULONG		blist;			/* Battle list offset, relative to start of header				*/
	ULONG		stab;			/* String table offset, relative to start of header				*/
	ULONG		type;			/* Dossier type									*/
	ULONG		gametype;		/* Dossier game type								*/
	DOS_CMPPROPS	props;			/* Dossier campaign properties							*/
} DOS_HEADER;

typedef struct s_DOS_BHEADER {
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
} DOS_BHEADER;

typedef struct s_DOS_THEADER {
	SPWAW_TIMESTAMP	date;			/* Battle turn date timestamp							*/
	ULONG		turn;			/* Battle turn number								*/
	ULONG		snap;			/* Battle turn snapshot offset, relative to start of DOS_THEADER table		*/
} DOS_THEADER;

#pragma pack(pop, r1)

#endif	/* INTERNAL_DOSSIER_FILE_H */
