/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT21_H
#define	INTERNAL_STRUCT21_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT21 {
	union u_u {
		char	raw[SIZESEC21];
		struct s_d {
			char	data[SIZESEC21];
		} d;
	} u;
} STRUCT21;

#endif	/* INTERNAL_STRUCT21_H */
