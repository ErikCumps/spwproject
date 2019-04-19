/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION39_SPWAW_H
#define	SECTION39_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_MAP_HEIGHT {
	DWORD	size;	/* Map height in hexes	*/
} MAP_HEIGHT;

typedef struct s_SECTION39 {
	union u_u {
		char	raw[SIZESEC39];
		struct s_d {
			MAP_HEIGHT	data;
		} d;
	} u;
} SECTION39;

#endif	/* SECTION39_SPWAW_H */
