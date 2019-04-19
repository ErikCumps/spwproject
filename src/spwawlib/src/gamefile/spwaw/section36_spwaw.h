/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION36_SPWAW_H
#define	SECTION36_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION36 {
	union u_u {
		char	raw[SIZESEC36];
		struct s_d {
			char	data[SIZESEC36];
		} d;
	} u;
} SECTION36;

#endif	/* SECTION36_SPWAW_H */
