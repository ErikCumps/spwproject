/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT18_H
#define	INTERNAL_STRUCT18_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT18 {
	union {
		char	raw[SIZESEC18];
		struct {
			char	data[SIZESEC18];
		} d;
	} u;
} STRUCT18;

#endif	/* INTERNAL_STRUCT18_H */
