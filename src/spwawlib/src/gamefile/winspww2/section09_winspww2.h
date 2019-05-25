/** \file
 * The SPWaW Library - winSPWW2 gamefile handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION09_WINSPWW2_H
#define	SECTION09_WINSPWW2_H	1

#include "gamefile/winspww2/defines_winspww2.h"

typedef struct s_WINSPWW2_MAP_DATA0 {
	char	__data[1];
} WINSPWW2_MAP_DATA0;

typedef struct s_WINSPWW2_SECTION09 {
	union u_u {
		char	raw[WINSPWW2_SIZESEC09];
		struct s_d {
			WINSPWW2_MAP_DATA0	data[WINSPWW2_MAPWIDTH][WINSPWW2_MAPHEIGHT];
		} d;
	} u;
} WINSPWW2_SECTION09;

#endif	/* SECTION09_WINSPWW2_H */
