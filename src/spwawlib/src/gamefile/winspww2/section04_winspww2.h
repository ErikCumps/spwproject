/** \file
 * The SPWaW Library - winSPWW2 gamefile handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION04_WINSPWW2_H
#define	SECTION04_WINSPWW2_H	1

#include "gamefile/winspww2/defines_winspww2.h"
#include "gamefile/maptile.h"

typedef struct s_WINSPWW2_SECTION04 {
	union u_u {
		char	raw[WINSPWW2_SIZESEC04];
		struct s_d {
			MAP_TILE	tile[WINSPWW2_MAPWIDTH][WINSPWW2_MAPHEIGHT];
		} d;
	} u;
} WINSPWW2_SECTION04;

#endif	/* SECTION04_WINSPWW2_H */
