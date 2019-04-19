/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION42_SPWAW_H
#define	SECTION42_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION42 {
	union u_u {
		char	raw[SIZESEC42];
		struct s_d {
			char	data[SIZESEC42];
		} d;
	} u;
} SECTION42;

#endif	/* SECTION42_SPWAW_H */
