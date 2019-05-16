/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION16_SPWAW_H
#define	SECTION16_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SPWAW_SECTION16 {
	union u_u {
		char	raw[SPWAW_SIZESEC16];
		struct s_d {
			char	data[SPWAW_SIZESEC16];
		} d;
	} u;
} SPWAW_SECTION16;

#endif	/* SECTION16_SPWAW_H */
