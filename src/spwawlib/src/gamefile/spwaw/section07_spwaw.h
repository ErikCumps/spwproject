/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION07_SPWAW_H
#define	SECTION07_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION07 {
	union u_u {
		char	raw[SPWAW_SIZESEC07];
		struct s_d {
			MAP_ICON	icon[SPWAW_MAPWIDTH][SPWAW_MAPHEIGHT];
		} d;
	} u;
} SECTION07;

#endif	/* SECTION07_SPWAW_H */
