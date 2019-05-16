/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION23_SPWAW_H
#define	SECTION23_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SPWAW_SECTION23 {
	union u_u {
		char	raw[SPWAW_SIZESEC23];
		struct s_d {
			char	data[SPWAW_SIZESEC23];
		} d;
	} u;
} SPWAW_SECTION23;

#endif	/* SECTION23_SPWAW_H */
