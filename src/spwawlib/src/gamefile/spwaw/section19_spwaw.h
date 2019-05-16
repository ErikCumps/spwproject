/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION19_SPWAW_H
#define	SECTION19_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SPWAW_SECTION19 {
	union u_u {
		char	raw[SPWAW_SIZESEC19];
		struct s_d {
			char	data[SPWAW_SIZESEC19];
		} d;
	} u;
} SPWAW_SECTION19;

#endif	/* SECTION19_SPWAW_H */
