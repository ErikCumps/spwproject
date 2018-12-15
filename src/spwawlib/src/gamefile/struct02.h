/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT02_H
#define	INTERNAL_STRUCT02_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT02 {
	union u_u {
		char	raw[SIZESEC02];
		struct s_d {
			MAP_TILE	tile[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} STRUCT02;

#endif	/* INTERNAL_STRUCT02_H */
