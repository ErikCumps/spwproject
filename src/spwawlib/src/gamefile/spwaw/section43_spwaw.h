/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION43_SPWAW_H
#define	SECTION43_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION43 {
	union u_u {
		char	raw[SIZESEC43];
		struct s_d {
			char	data[SIZESEC43];
		} d;
	} u;
} SECTION43;

#endif	/* SECTION43_SPWAW_H */
