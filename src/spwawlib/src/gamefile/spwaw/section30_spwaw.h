/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION30_SPWAW_H
#define	SECTION30_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION30 {
	union u_u {
		char	raw[SPWAW_SIZESEC30];
		struct s_d {
			char	data[SPWAW_SIZESEC30];
		} d;
	} u;
} SECTION30;

#endif	/* SECTION30_SPWAW_H */
