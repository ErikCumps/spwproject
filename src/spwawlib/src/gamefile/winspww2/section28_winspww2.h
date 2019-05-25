/** \file
 * The SPWaW Library - winSPWW2 gamefile handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION28_WINSPWW2_H
#define	SECTION28_WINSPWW2_H	1

#include "gamefile/winspww2/defines_winspww2.h"

typedef struct s_WINSPWW2_MAP_SLOPE_DATA {
	BYTE		count;		/* Tile count		*/
	MAP_TILE	tiles[6];	/* Tile #0 .. #5	*/
} WINSPWW2_MAP_SLOPE_DATA;

typedef struct s_WINSPWW2_SECTION28 {
	union u_u {
		char	raw[WINSPWW2_SIZESEC28];
		struct s_d {
			WINSPWW2_MAP_SLOPE_DATA	data[WINSPWW2_MAPWIDTH][WINSPWW2_MAPHEIGHT];
		} d;
	} u;
} WINSPWW2_SECTION28;

#endif	/* SECTION28_WINSPWW2_H */
