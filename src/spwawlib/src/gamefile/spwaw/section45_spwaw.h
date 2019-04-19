/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION45_SPWAW_H
#define	SECTION45_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION45 {
	union u_u {
		char	raw[SIZESEC45];
		struct s_d {
			char	data[SIZESEC45];
		} d;
	} u;
} SECTION45;

#endif	/* SECTION45_SPWAW_H */
