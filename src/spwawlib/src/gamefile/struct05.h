/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT05_H
#define	INTERNAL_STRUCT05_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT05 {
	union u_u {
		char	raw[SIZESEC05];
		struct s_d {
			MAP_ICON	icon[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} STRUCT05;

#endif	/* INTERNAL_STRUCT05_H */
