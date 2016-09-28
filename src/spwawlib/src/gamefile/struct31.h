/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT31_H
#define	INTERNAL_STRUCT31_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT31 {
	union u_u {
		char	raw[SIZESEC31];
		struct s_d {
			char	data[SIZESEC31];
		} d;
	} u;
} STRUCT31;

#endif	/* INTERNAL_STRUCT31_H */
