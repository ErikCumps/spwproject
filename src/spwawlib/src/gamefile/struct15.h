/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT15_H
#define	INTERNAL_STRUCT15_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT15 {
	union u_u {
		char	raw[SIZESEC15];
		struct s_d {
			char	data[SIZESEC15];
		} d;
	} u;
} STRUCT15;

#endif	/* INTERNAL_STRUCT15_H */
