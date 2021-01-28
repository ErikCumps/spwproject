/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION28_WINSPMBT_H
#define	SECTION28_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_MAP_SLOPE_DATA {
	BYTE		count;		/* Tile count		*/
	MAP_TILE	tiles[6];	/* Tile #0 .. #5	*/
} WINSPMBT_MAP_SLOPE_DATA;

typedef struct s_WINSPMBT_SECTION28 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC28];
		struct s_d {
			WINSPMBT_MAP_SLOPE_DATA	data[WINSPMBT_MAPWIDTH][WINSPMBT_MAPHEIGHT];
		} d;
	} u;
} WINSPMBT_SECTION28;

#endif	/* SECTION28_WINSPMBT_H */
