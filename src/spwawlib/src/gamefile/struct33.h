/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT33_H
#define	INTERNAL_STRUCT33_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT33 {
	union {
		char	raw[SIZESEC33];
		struct {
			char	data[SIZESEC33];
		} d;
	} u;
} STRUCT33;

#endif	/* INTERNAL_STRUCT33_H */
