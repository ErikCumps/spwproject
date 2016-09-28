/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT40_H
#define	INTERNAL_STRUCT40_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT40 {
	union u_u {
		char	raw[SIZESEC40];
		struct s_d {
			char	data[SIZESEC40];
		} d;
	} u;
} STRUCT40;

#endif	/* INTERNAL_STRUCT40_H */
