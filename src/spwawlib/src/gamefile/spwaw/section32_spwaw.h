/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION32_SPWAW_H
#define	SECTION32_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION32 {
	union u_u {
		char	raw[SPWAW_SIZESEC32];
		struct s_d {
			char	data[SPWAW_SIZESEC32];
		} d;
	} u;
} SECTION32;

#endif	/* SECTION32_SPWAW_H */
