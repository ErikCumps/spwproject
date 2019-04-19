/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION40_SPWAW_H
#define	SECTION40_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION40 {
	union u_u {
		char	raw[SIZESEC40];
		struct s_d {
			char	data[SIZESEC40];
		} d;
	} u;
} SECTION40;

#endif	/* SECTION40_SPWAW_H */
