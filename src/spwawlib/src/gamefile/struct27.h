/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT27_H
#define	INTERNAL_STRUCT27_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT27 {
	union u_u {
		char	raw[SIZESEC27];
		struct s_d {
			char	data[SIZESEC27];
		} d;
	} u;
} STRUCT27;

#endif	/* INTERNAL_STRUCT27_H */
