/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT25_H
#define	INTERNAL_STRUCT25_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT25 {
	union u_u {
		char	raw[SIZESEC25];
		struct s_d {
			char	data[SIZESEC25];
		} d;
	} u;
} STRUCT25;

#endif	/* INTERNAL_STRUCT25_H */
