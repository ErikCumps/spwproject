/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION23_SPWAW_H
#define	SECTION23_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION23 {
	union u_u {
		char	raw[SIZESEC23];
		struct s_d {
			char	data[SIZESEC23];
		} d;
	} u;
} SECTION23;

#endif	/* SECTION23_SPWAW_H */