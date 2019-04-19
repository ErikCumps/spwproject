/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION25_SPWAW_H
#define	SECTION25_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION25 {
	union u_u {
		char	raw[SIZESEC25];
		struct s_d {
			char	data[SIZESEC25];
		} d;
	} u;
} SECTION25;

#endif	/* SECTION25_SPWAW_H */
