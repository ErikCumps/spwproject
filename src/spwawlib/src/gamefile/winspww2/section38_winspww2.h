/** \file
 * The SPWaW Library - winSPWW2 gamefile handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION38_WINSPWW2_H
#define	SECTION38_WINSPWW2_H	1

#include "gamefile/winspww2/defines_winspww2.h"

typedef struct s_WINSPWW2_MAP_WIDTH {
	DWORD	size;	/* Map width in hexes	*/
} WINSPWW2_MAP_WIDTH;

typedef struct s_WINSPWW2_SECTION38 {
	union u_u {
		char	raw[WINSPWW2_SIZESEC38];
		struct s_d {
			WINSPWW2_MAP_WIDTH	data;
		} d;
	} u;
} WINSPWW2_SECTION38;

#endif	/* SECTION38_WINSPWW2_H */
