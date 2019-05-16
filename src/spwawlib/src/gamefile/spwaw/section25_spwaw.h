/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION25_SPWAW_H
#define	SECTION25_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SPWAW_SECTION25 {
	union u_u {
		char	raw[SPWAW_SIZESEC25];
		struct s_d {
			char	data[SPWAW_SIZESEC25];
		} d;
	} u;
} SPWAW_SECTION25;

#endif	/* SECTION25_SPWAW_H */
