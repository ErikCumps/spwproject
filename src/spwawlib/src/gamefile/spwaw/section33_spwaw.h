/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION33_SPWAW_H
#define	SECTION33_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION33 {
	union u_u {
		char	raw[SIZESEC33];
		struct s_d {
			char	data[SIZESEC33];
		} d;
	} u;
} SECTION33;

#endif	/* SECTION33_SPWAW_H */
