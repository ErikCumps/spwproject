/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION21_SPWAW_H
#define	SECTION21_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION21 {
	union u_u {
		char	raw[SIZESEC21];
		struct s_d {
			char	data[SIZESEC21];
		} d;
	} u;
} SECTION21;

#endif	/* SECTION21_SPWAW_H */
