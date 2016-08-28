/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT47_H
#define	INTERNAL_STRUCT47_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT47 {
	union {
		char	raw[SIZESEC47];
		struct {
			char	data[SIZESEC47];
		} d;
	} u;
} STRUCT47;

#endif	/* INTERNAL_STRUCT47_H */