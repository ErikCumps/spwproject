/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT32_H
#define	INTERNAL_STRUCT32_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT32 {
	union {
		char	raw[SIZESEC32];
		struct {
			char	data[SIZESEC32];
		} d;
	} u;
} STRUCT32;

#endif	/* INTERNAL_STRUCT32_H */