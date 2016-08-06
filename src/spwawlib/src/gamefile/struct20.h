/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT20_H
#define	INTERNAL_STRUCT20_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT20 {
	union {
		char	raw[SIZESEC20];
		struct {
			char	data[SIZESEC20];
		} d;
	} u;
} STRUCT20;

#endif	/* INTERNAL_STRUCT20_H */
