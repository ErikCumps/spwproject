/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION13_SPWAW_H
#define	SECTION13_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION13 {
	union u_u {
		char	raw[SIZESEC13];
		struct s_d {
			char	data[SIZESEC13];
		} d;
	} u;
} SECTION13;

#endif	/* SECTION13_SPWAW_H */
