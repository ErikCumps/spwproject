/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION22_SPWAW_H
#define	SECTION22_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION22 {
	union u_u {
		char	raw[SIZESEC22];
		struct s_d {
			char	data[SIZESEC22];
		} d;
	} u;
} SECTION22;

#endif	/* SECTION22_SPWAW_H */
