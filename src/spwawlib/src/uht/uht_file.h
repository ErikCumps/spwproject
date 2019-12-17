/** \file
 * The SPWaW Library - unit history tracking handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_UHT_FILE_H
#define	INTERNAL_UHT_FILE_H	1

#define	UHT_MAGIC	"SPWAWLIB_UHTDATA"
#define	UHT_MGCLEN	16
#define	UHT_VERSION	1

#pragma pack(push, r1, 1)

typedef struct s_UHT_HEADER {
	char		magic[UHT_MGCLEN];	/* UHT data magic string						*/
	ULONG		version;		/* UHT data file format version						*/
	ULONG		cnt;			/* element count							*/
	ULONG		data;			/* data offset, relative to start of header				*/
	ULONG		size;			/* data size								*/
	ULONG		comp;			/* compressed data size (0 if no compression)				*/
} UHT_HEADER;

typedef struct s_UHT_ELEMENT {
	ULONG		idx;			/* Element index							*/
	ULONG		lname;			/* Unit identity: name symbol						*/
	ULONG		UID;			/* Unit identity: UID symbol						*/
	ULONG		uname;			/* Unit identity: equipment symbol					*/
	BYTE		rank;			/* Unit identity: rank							*/
	USHORT		FMID;			/* Unit identity: formation major ID					*/
	BYTE		FSID;			/* Unit identity: formation sub ID					*/
	USHORT		FBIi;			/* Battle index for the battle the unit was first seen in		*/
	SPWAW_TIMESTAMP	FBId;			/* Battle date for the battle the unit was first seen in		*/
	USHORT		FUI;			/* Unit index for the battle the unit was first seen in			*/
	USHORT		LBIi;			/* Battle index for the battle the unit was last seen in		*/
	SPWAW_TIMESTAMP	LBId;			/* Battle date for the battle the unit was last seen in			*/
	USHORT		status;			/* Unit history status							*/
	ULONG		prev;			/* Index of previous element in unit history list			*/
	ULONG		next;			/* Index of next element in unit history list				*/
} UHT_ELEMENT;

#pragma pack(pop, r1)

#endif	/* INTERNAL_UHT_FILE_H */
