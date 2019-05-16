/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION10_SPWAW_H
#define	SECTION10_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SPWAW_SECTION10 {
	union u_u {
		char	raw[SPWAW_SIZESEC10];
		struct s_d {
			char	data[SPWAW_SIZESEC10];
		} d;
	} u;
} SPWAW_SECTION10;

#endif	/* SECTION10_SPWAW_H */
