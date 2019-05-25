/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION28_SPWAW_H
#define	SECTION28_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SPWAW_MAP_SLOPE_DATA {
	USHORT		count;		/* Tile count		*/
	MAP_TILE	tiles[6];	/* Tile #0 .. #5	*/
} SPWAW_MAP_SLOPE_DATA;

typedef struct s_SPWAW_SECTION28 {
	union u_u {
		char	raw[SPWAW_SIZESEC28];
		struct s_d {
			SPWAW_MAP_SLOPE_DATA	data[SPWAW_MAPWIDTH][SPWAW_MAPHEIGHT];
		} d;
	} u;
} SPWAW_SECTION28;

#endif	/* SECTION28_SPWAW_H */
