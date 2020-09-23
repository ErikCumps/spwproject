/** \file
 * The SPWaW Library - dossier handling - backwards compatibility with the V13 dossier.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_DOSSIER_FILE_V13_H
#define	INTERNAL_DOSSIER_FILE_V13_H	1

#include <spwawlib_api.h>
#include <spwawlib_types.h>
#include "dossier/dossier_file.h"

#define	DOSS_VERSION_V13	13

#pragma pack(push, r1, 1)

typedef struct s_DOS_CMPPROPS_V13 {
	BYTE			OOB;		/* OOB ID									*/
	SPWAW_TIMESTAMP		start;		/* Campaign start date								*/
	SPWAW_TIMESTAMP		end;		/* Campaign end date								*/
	USHORT			maxbtlcnt;	/* Campaign maximum battles count						*/
	USHORT			ifcnt;		/* Player initial core formations count						*/
	USHORT			iucnt;		/* Player initial core units count						*/
	USHORT			cfcnt;		/* Player current core formations count						*/
	USHORT			cucnt;		/* Player current core units count						*/
} DOS_CMPPROPS_V13;

typedef struct s_DOS_HEADER_V13 {
	ULONG			name;		/* Dossier name	symbol								*/
	ULONG			comment;	/* Dossier comment symbol							*/
	ULONG			oobdir;		/* Original OOB data directory symbol						*/
	ULONG			oobdata;	/* Dossier OOB data list offset, relative to start of header			*/
	USHORT			bcnt;		/* Battle list count								*/
	ULONG			blist;		/* Battle list offset, relative to start of header				*/
	ULONG			stab;		/* String table offset, relative to start of header				*/
	ULONG			type;		/* Dossier type									*/
	ULONG			gametype;	/* Dossier game type								*/
	DOS_CMPPROPS_V13	props;		/* Dossier campaign properties							*/
	ULONG			uht;		/* Dossier UHT data								*/
} DOS_HEADER_V13;

#pragma pack(pop, r1)

extern SPWAW_ERROR	dossier_load_v13_header		(int fd, DOS_HEADER *hdr);

#endif	/* INTERNAL_DOSSIER_FILE_V13_H */
