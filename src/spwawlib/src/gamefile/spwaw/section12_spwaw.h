/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION12_SPWAW_H
#define	SECTION12_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION12 {
	union u_u {
		char	raw[SIZESEC12];
		struct s_d {
			char	data[SIZESEC12];
		} d;
	} u;
} SECTION12;

#endif	/* SECTION12_SPWAW_H */
