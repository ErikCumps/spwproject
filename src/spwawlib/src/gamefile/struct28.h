/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT28_H
#define	INTERNAL_STRUCT28_H	1

#include "gamefile/struct_common.h"

//typedef struct s_MAP_DATA1 {
//	USHORT	count;		/* Tile count		*/
//	BYTE	ID;		/* Tile ID + 0xD6 ???)	*/
//	BYTE	file;		/* File ID - 1 ???	*/
//	char	__data00[10];
//} MAP_DATA1;

typedef struct s_MAP_DATA1 {
	BYTE		count;		/* Tile count		*/
	MAP_ICON	tile0;		/* Tile #0		*/
	MAP_ICON	tile1;		/* Tile #1		*/
	MAP_ICON	tile2;		/* Tile #2		*/
	MAP_ICON	tile3;		/* Tile #3		*/
	MAP_ICON	tile4;		/* Tile #4		*/
	MAP_ICON	tile5;		/* Tile #5		*/
} MAP_DATA1;


typedef struct s_STRUCT28 {
	union u_u {
		char	raw[SIZESEC28];
		struct s_d {
			MAP_DATA1	data[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} STRUCT28;

#endif	/* INTERNAL_STRUCT28_H */
