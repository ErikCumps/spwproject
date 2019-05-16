/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION05_SPWAW_H
#define	SECTION05_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SPWAW_SECTION05 {
	union u_u {
		char	raw[SPWAW_SIZESEC05];
		struct s_d {
			MAP_TILE	tile[SPWAW_MAPWIDTH][SPWAW_MAPHEIGHT];
		} d;
	} u;
} SPWAW_SECTION05;

#endif	/* SECTION05_SPWAW_H */
