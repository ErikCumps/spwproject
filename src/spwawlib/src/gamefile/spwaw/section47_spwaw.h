/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION47_SPWAW_H
#define	SECTION47_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION47 {
	union u_u {
		char	raw[SIZESEC47];
		struct s_d {
			char	data[SIZESEC47];
		} d;
	} u;
} SECTION47;

#endif	/* SECTION47_SPWAW_H */
