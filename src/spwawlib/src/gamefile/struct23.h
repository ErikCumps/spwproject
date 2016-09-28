/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT23_H
#define	INTERNAL_STRUCT23_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT23 {
	union u_u {
		char	raw[SIZESEC23];
		struct s_d {
			char	data[SIZESEC23];
		} d;
	} u;
} STRUCT23;

#endif	/* INTERNAL_STRUCT23_H */
