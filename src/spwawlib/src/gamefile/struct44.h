/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT44_H
#define	INTERNAL_STRUCT44_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT44 {
	union u_u {
		char	raw[SIZESEC44];
		struct s_d {
			char	data[SIZESEC44];
		} d;
	} u;
} STRUCT44;

#endif	/* INTERNAL_STRUCT44_H */
