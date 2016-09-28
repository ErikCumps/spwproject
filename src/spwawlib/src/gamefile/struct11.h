/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT11_H
#define	INTERNAL_STRUCT11_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT11 {
	union u_u {
		char	raw[SIZESEC11];
		struct s_d {
			char	data[SIZESEC11];
		} d;
	} u;
} STRUCT11;

#endif	/* INTERNAL_STRUCT11_H */
