/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT29_H
#define	INTERNAL_STRUCT29_H	1

#include "gamefile/struct_common.h"

typedef struct s_MAP_DATA2 {
	USHORT	vis;	/* Visibility?	*/
} MAP_DATA2;

typedef struct s_STRUCT29 {
	union u_u {
		char	raw[SIZESEC29];
		struct s_d {
			MAP_DATA2	data[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} STRUCT29;

#endif	/* INTERNAL_STRUCT29_H */
