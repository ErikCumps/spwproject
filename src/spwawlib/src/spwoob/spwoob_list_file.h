/** \file
 * The SPWaW Library - SPWaW OOB list handling.
 *
 * Copyright (C) 2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_SPWOOB_LIST_FILE_H
#define	INTERNAL_SPWOOB_LIST_FILE_H	1

#include <spwawlib_api.h>
#include <spwawlib_types.h>
#include <spwawlib_defines.h>
#include <ad_hash.h>

#define	SPWOOBLIST_MAGIC	"SPWAWLIB_SPWOOBLIST"
#define	SPWOOBLIST_MGCLEN	20
#define	SPWOOBLIST_VERSION	1

#pragma pack(push, r1, 1)

typedef struct s_SPWOOBLIST_HEADER {
	char	magic[SPWOOBLIST_MGCLEN];	/* SPWOOB_LIST magic string						*/
	USHORT	version;			/* SPWOOB_LIST file format version					*/
	ULONG	cnt;				/* SPWOOB data list count						*/
} SPWOOBLIST_HEADER;

typedef struct s_SPWOOBLIST_SPWOOB {
	ULONG	data;				/* SPWOOB data offset, relative to start of SPWOOBLIST_OOBHDR table	*/
} SPWOOBLIST_SPWOOB;

#pragma pack(pop, r1)

#endif	/* INTERNAL_SPWOOB_LIST_FILE_H */
