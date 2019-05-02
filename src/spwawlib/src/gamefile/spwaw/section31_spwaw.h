/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION31_SPWAW_H
#define	SECTION31_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION31 {
	union u_u {
		char	raw[SPWAW_SIZESEC31];
		struct s_d {
			char	data[SPWAW_SIZESEC31];
		} d;
	} u;
} SECTION31;

#endif	/* SECTION31_SPWAW_H */
