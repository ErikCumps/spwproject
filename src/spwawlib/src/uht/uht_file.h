/** \file
 * The SPWaW Library - unit history tracking handling.
 *
 * Copyright (C) 2019-2020 Erik Cumps <erik.cumps@gmail.com>
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
	ULONG		ucnt;			/* UHTE list element count						*/
	ULONG		udata;			/* UHTE list data offset, relative to start of header			*/
	ULONG		usize;			/* UHTE list data size							*/
	ULONG		ucomp;			/* UHTE list compressed data size (0 if no compression)			*/
	ULONG		bcnt;			/* UHT_BINFO list element count						*/
	ULONG		bdata;			/* UHT_BINFO list data offset, relative to start of header		*/
} UHT_HEADER;

typedef struct s_UHT_ELEMENT {
	ULONG		idx;			/* Element index							*/
	ULONG		lname;			/* Unit identity: name symbol						*/
	ULONG		UID;			/* Unit identity: UID symbol						*/
	ULONG		uname;			/* Unit identity: equipment symbol					*/
	BYTE		rank;			/* Unit identity: rank							*/
	USHORT		FMID;			/* Unit identity: formation major ID					*/
	BYTE		FSID;			/* Unit identity: formation sub ID					*/
	USHORT		FBDi;			/* Battle index for the battle the unit was first seen in		*/
	SPWAW_TIMESTAMP	FBDd;			/* Battle date for the battle the unit was first seen in		*/
	USHORT		FUI;			/* Unit index for the battle the unit was first seen in			*/
	USHORT		LBDi;			/* Battle index for the battle the unit was last seen in		*/
	SPWAW_TIMESTAMP	LBDd;			/* Battle date for the battle the unit was last seen in			*/
	USHORT		status;			/* Unit history status							*/
	ULONG		prev;			/* Index of previous element in unit history list			*/
	ULONG		next;			/* Index of next element in unit history list				*/
} UHT_ELEMENT;

typedef struct s_UHT_BATTLE_INFO {
	USHORT		bdi;			/* Battle index for the battle associated with this UHT_BINFO		*/
	SPWAW_TIMESTAMP	bdd;			/* Battle date for the battle associated with this UHT_BINFO		*/
	USHORT		cnt;			/* UHTE pointer list element count					*/
	ULONG		data;			/* UHTE pointer list data offset, relative to start of header		*/
	ULONG		size;			/* UHTE pointer list data size						*/
	ULONG		comp;			/* UHTE pointer list compressed data size (0 if no compression)		*/
} UHT_BATTLE_INFO;

typedef struct s_UHT_ELEMENT_PTR {
	ULONG		idx;			/* UHT element index							*/
} UHT_ELEMENT_PTR;

#pragma pack(pop, r1)

#endif	/* INTERNAL_UHT_FILE_H */
