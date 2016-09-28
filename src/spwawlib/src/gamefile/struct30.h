/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT30_H
#define	INTERNAL_STRUCT30_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT30 {
	union u_u {
		char	raw[SIZESEC30];
		struct s_d {
			char	data[SIZESEC30];
		} d;
	} u;
} STRUCT30;

#endif	/* INTERNAL_STRUCT30_H */
