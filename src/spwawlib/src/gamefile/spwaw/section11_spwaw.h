/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION11_SPWAW_H
#define	SECTION11_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION11 {
	union u_u {
		char	raw[SIZESEC11];
		struct s_d {
			char	data[SIZESEC11];
		} d;
	} u;
} SECTION11;

#endif	/* SECTION11_SPWAW_H */