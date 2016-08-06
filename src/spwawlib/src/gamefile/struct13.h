/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT13_H
#define	INTERNAL_STRUCT13_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT13 {
	union {
		char	raw[SIZESEC13];
		struct {
			char	data[SIZESEC13];
		} d;
	} u;
} STRUCT13;

#endif	/* INTERNAL_STRUCT13_H */
