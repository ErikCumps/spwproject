/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION09_SPWAW_H
#define	SECTION09_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_MAP_DATA0 {
	char	__data[8];
} MAP_DATA0;

typedef struct s_SECTION09 {
	union u_u {
		char	raw[SIZESEC09];
		struct s_d {
			MAP_DATA0	data[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} SECTION09;

#endif	/* SECTION09_SPWAW_H */
