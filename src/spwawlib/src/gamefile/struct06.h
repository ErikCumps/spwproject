/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT06_H
#define	INTERNAL_STRUCT06_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT06 {
	union u_u {
		char	raw[SIZESEC06];
		struct s_d {
			MAP_ICON	icon[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} STRUCT06;

#endif	/* INTERNAL_STRUCT06_H */
