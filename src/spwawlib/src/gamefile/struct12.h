/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT12_H
#define	INTERNAL_STRUCT12_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT12 {
	union {
		char	raw[SIZESEC12];
		struct {
			char	data[SIZESEC12];
		} d;
	} u;
} STRUCT12;

#endif	/* INTERNAL_STRUCT12_H */
