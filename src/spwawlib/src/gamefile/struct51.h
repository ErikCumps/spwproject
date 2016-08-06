/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT51_H
#define	INTERNAL_STRUCT51_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT51 {
	union {
		char	raw[SIZESEC51];
		struct {
			char	data[SIZESEC51];
		} d;
	} u;
} STRUCT51;

#endif	/* INTERNAL_STRUCT51_H */
