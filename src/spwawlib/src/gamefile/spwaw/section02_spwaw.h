/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION02_SPWAW_H
#define	SECTION02_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"
#include "gamefile/maptile.h"

typedef struct s_SECTION02 {
	union u_u {
		char	raw[SPWAW_SIZESEC02];
		struct s_d {
			MAP_TILE	tile[SPWAW_MAPWIDTH][SPWAW_MAPHEIGHT];
		} d;
	} u;
} SECTION02;

#endif	/* SECTION02_SPWAW_H */
