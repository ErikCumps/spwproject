/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION46_SPWAW_H
#define	SECTION46_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION46 {
	union u_u {
		char	raw[SPWAW_SIZESEC46];
		struct s_d {
			char	data[SPWAW_SIZESEC46];
		} d;
	} u;
} SECTION46;

#endif	/* SECTION46_SPWAW_H */
