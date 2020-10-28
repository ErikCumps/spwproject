/** \file
 * The SPWaW Library - SPWaW OOB handling.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_SPWOOBFILE_H
#define	INTERNAL_SPWOOBFILE_H	1

#include <spwawlib_api.h>
#include <spwawlib_types.h>
#include <spwawlib_defines.h>

#define	SPWOOB_MAGIC	"SPWAWLIB_SPWOOB"
#define	SPWOOB_MGCLEN	16
#define	SPWOOB_VERSION	2

#pragma pack(push, r1, 1)

typedef struct s_SPWOOB_MV_HEADER {
	char	magic[SPWOOB_MGCLEN];	/* SPWOOB magic string						*/
	USHORT	version;		/* SPWOOB file format version					*/
} SPWOOB_MV_HEADER;

typedef struct s_SPWOOB_HEADER {
	BYTE	cnt;			/* OOB list count						*/
	ULONG	gametype;		/* OOB game type						*/
} SPWOOB_HEADER;

typedef struct s_SPWOOB_OOBHDR {
	BYTE	id;			/* OOB ID							*/
	ULONG	crc32;			/* OOB data crc32						*/
	ULONG	data;			/* OOB data offset, relative to start of SPWOOB_OOBHDR table	*/
	ULONG	size;			/* OOB data size						*/
	ULONG	comp;			/* OOB compressed data size (0 if no compression)		*/
} SPWOOB_OOBHDR;

#pragma pack(pop, r1)

#endif	/* INTERNAL_SPWOOBFILE_H */
