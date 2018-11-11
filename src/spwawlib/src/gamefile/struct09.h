/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT09_H
#define	INTERNAL_STRUCT09_H	1

#include "gamefile/struct_common.h"

typedef struct s_MAP_DATA0 {
	char	__data[1];
} MAP_DATA0;

typedef struct s_STRUCT09 {
	union u_u {
		char	raw[SIZESEC09];
		struct s_d {
			MAP_DATA0	data[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} STRUCT09;

#endif	/* INTERNAL_STRUCT09_H */
