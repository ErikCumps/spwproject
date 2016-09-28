/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT36_H
#define	INTERNAL_STRUCT36_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT36 {
	union u_u {
		char	raw[SIZESEC36];
		struct s_d {
			char	data[SIZESEC36];
		} d;
	} u;
} STRUCT36;

#endif	/* INTERNAL_STRUCT36_H */
