/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION18_SPWAW_H
#define	SECTION18_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION18 {
	union u_u {
		char	raw[SIZESEC18];
		struct s_d {
			char	data[SIZESEC18];
		} d;
	} u;
} SECTION18;

#endif	/* SECTION18_SPWAW_H */
