/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT41_H
#define	INTERNAL_STRUCT41_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT41 {
	union u_u {
		char	raw[SIZESEC41];
		struct s_d {
			char	data[SIZESEC41];
		} d;
	} u;
} STRUCT41;

#endif	/* INTERNAL_STRUCT41_H */
