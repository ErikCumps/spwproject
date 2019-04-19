/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION26_SPWAW_H
#define	SECTION26_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION26 {
	union u_u {
		char	raw[SIZESEC26];
		struct s_d {
			char	data[SIZESEC26];
		} d;
	} u;
} SECTION26;

#endif	/* SECTION26_SPWAW_H */
