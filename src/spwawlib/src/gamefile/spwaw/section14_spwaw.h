/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION14_SPWAW_H
#define	SECTION14_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_CREDIT {
	DWORD	amount;	/* Amount of available purchase points	*/
} CREDIT;

typedef struct s_SECTION14 {
	union u_u {
		char	raw[SPWAW_SIZESEC14];
		struct s_d {
			CREDIT	data;
		} d;
	} u;
} SECTION14;

#endif	/* SECTION14_SPWAW_H */
