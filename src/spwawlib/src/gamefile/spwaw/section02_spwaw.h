/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION02_SPWAW_H
#define	SECTION02_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"
#include "gamefile/maptile.h"

typedef struct s_SECTION02 {
	union u_u {
		char	raw[SIZESEC02];
		struct s_d {
			MAP_ICON	icon[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} SECTION02;

#endif	/* SECTION02_SPWAW_H */