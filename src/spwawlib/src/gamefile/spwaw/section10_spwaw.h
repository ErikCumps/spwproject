/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION10_SPWAW_H
#define	SECTION10_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION10 {
	union u_u {
		char	raw[SIZESEC10];
		struct s_d {
			char	data[SIZESEC10];
		} d;
	} u;
} SECTION10;

#endif	/* SECTION10_SPWAW_H */
