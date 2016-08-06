/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT10_H
#define	INTERNAL_STRUCT10_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT10 {
	union {
		char	raw[SIZESEC10];
		struct {
			char	data[SIZESEC10];
		} d;
	} u;
} STRUCT10;

#endif	/* INTERNAL_STRUCT10_H */
