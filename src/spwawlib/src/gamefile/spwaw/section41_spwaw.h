/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION41_SPWAW_H
#define	SECTION41_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION41 {
	union u_u {
		char	raw[SIZESEC41];
		struct s_d {
			char	data[SIZESEC41];
		} d;
	} u;
} SECTION41;

#endif	/* SECTION41_SPWAW_H */
