/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION12_SPWAW_H
#define	SECTION12_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SPWAW_SECTION12 {
	union u_u {
		char	raw[SPWAW_SIZESEC12];
		struct s_d {
			char	data[SPWAW_SIZESEC12];
		} d;
	} u;
} SPWAW_SECTION12;

#endif	/* SECTION12_SPWAW_H */
