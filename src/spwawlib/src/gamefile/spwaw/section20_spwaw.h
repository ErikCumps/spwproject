/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION20_SPWAW_H
#define	SECTION20_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION20 {
	union u_u {
		char	raw[SPWAW_SIZESEC20];
		struct s_d {
			char	data[SPWAW_SIZESEC20];
		} d;
	} u;
} SECTION20;

#endif	/* SECTION20_SPWAW_H */
