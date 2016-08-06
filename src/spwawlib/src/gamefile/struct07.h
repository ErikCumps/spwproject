/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT07_H
#define	INTERNAL_STRUCT07_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT07 {
	union {
		char	raw[SIZESEC07];
		struct {
			MAP_ICON	icon[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} STRUCT07;

#endif	/* INTERNAL_STRUCT07_H */
