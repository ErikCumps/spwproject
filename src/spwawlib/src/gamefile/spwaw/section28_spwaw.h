/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION28_SPWAW_H
#define	SECTION28_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_MAP_DATA1 {
	USHORT	count;		/* Tile count		*/
	BYTE	ID;		/* Tile ID + 0xD6 ???)	*/
	BYTE	file;		/* File ID - 1 ???	*/
	char	__data00[10];
} MAP_DATA1;

typedef struct s_SECTION28 {
	union u_u {
		char	raw[SIZESEC28];
		struct s_d {
			MAP_DATA1	data[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} SECTION28;

#endif	/* SECTION28_SPWAW_H */
