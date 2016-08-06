/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT22_H
#define	INTERNAL_STRUCT22_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT22 {
	union {
		char	raw[SIZESEC22];
		struct {
			char	data[SIZESEC22];
		} d;
	} u;
} STRUCT22;

#endif	/* INTERNAL_STRUCT22_H */
