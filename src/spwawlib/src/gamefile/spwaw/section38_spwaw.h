/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION38_SPWAW_H
#define	SECTION38_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_MAP_WIDTH {
	DWORD	size;	/* Map width in hexes	*/
} MAP_WIDTH;

typedef struct s_SECTION38 {
	union u_u {
		char	raw[SIZESEC38];
		struct s_d {
			MAP_WIDTH	data;
		} d;
	} u;
} SECTION38;

#endif	/* SECTION38_SPWAW_H */
