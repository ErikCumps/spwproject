/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION51_SPWAW_H
#define	SECTION51_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION51 {
	union u_u {
		char	raw[SIZESEC51];
		struct s_d {
			char	data[SIZESEC51];
		} d;
	} u;
} SECTION51;

#endif	/* SECTION51_SPWAW_H */