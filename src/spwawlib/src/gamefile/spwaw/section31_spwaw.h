/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION31_SPWAW_H
#define	SECTION31_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION31 {
	union u_u {
		char	raw[SIZESEC31];
		struct s_d {
			char	data[SIZESEC31];
		} d;
	} u;
} SECTION31;

#endif	/* SECTION31_SPWAW_H */
