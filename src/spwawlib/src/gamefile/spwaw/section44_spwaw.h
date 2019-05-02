/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION44_SPWAW_H
#define	SECTION44_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION44 {
	union u_u {
		char	raw[SPWAW_SIZESEC44];
		struct s_d {
			char	data[SPWAW_SIZESEC44];
		} d;
	} u;
} SECTION44;

#endif	/* SECTION44_SPWAW_H */
