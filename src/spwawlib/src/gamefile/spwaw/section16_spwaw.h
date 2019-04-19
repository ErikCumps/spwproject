/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION16_SPWAW_H
#define	SECTION16_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION16 {
	union u_u {
		char	raw[SIZESEC16];
		struct s_d {
			char	data[SIZESEC16];
		} d;
	} u;
} SECTION16;

#endif	/* SECTION16_SPWAW_H */
