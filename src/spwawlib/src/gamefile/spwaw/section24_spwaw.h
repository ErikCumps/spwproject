/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION24_SPWAW_H
#define	SECTION24_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION24 {
	union u_u {
		char	raw[SIZESEC24];
		struct s_d {
			char	data[SIZESEC24];
		} d;
	} u;
} SECTION24;

#endif	/* SECTION24_SPWAW_H */
