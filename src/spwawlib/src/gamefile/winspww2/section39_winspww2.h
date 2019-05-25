/** \file
 * The SPWaW Library - winSPWW2 gamefile handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION39_WINSPWW2_H
#define	SECTION39_WINSPWW2_H	1

#include "gamefile/winspww2/defines_winspww2.h"

typedef struct s_WINSPWW2_MAP_HEIGHT {
	DWORD	size;	/* Map height in hexes	*/
} WINSPWW2_MAP_HEIGHT;

typedef struct s_WINSPWW2_SECTION39 {
	union u_u {
		char	raw[WINSPWW2_SIZESEC39];
		struct s_d {
			WINSPWW2_MAP_HEIGHT	data;
		} d;
	} u;
} WINSPWW2_SECTION39;

#endif	/* SECTION39_WINSPWW2_H */
