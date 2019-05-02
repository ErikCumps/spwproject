/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION27_SPWAW_H
#define	SECTION27_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION27 {
	union u_u {
		char	raw[SPWAW_SIZESEC27];
		struct s_d {
			char	data[SPWAW_SIZESEC27];
		} d;
	} u;
} SECTION27;

#endif	/* SECTION27_SPWAW_H */
