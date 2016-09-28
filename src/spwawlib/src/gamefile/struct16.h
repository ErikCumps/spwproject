/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT16_H
#define	INTERNAL_STRUCT16_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT16 {
	union u_u {
		char	raw[SIZESEC16];
		struct s_d {
			char	data[SIZESEC16];
		} d;
	} u;
} STRUCT16;

#endif	/* INTERNAL_STRUCT16_H */
