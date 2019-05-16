/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION48_SPWAW_H
#define	SECTION48_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_RECHEAD {
	char	__data00[4];
	DWORD	size;		/* Data size in bytes	*/
	char	__data01[72];
} RECHEAD;

typedef struct s_SPWAW_SECTION48 {
	union u_u {
		char	raw[SPWAW_SIZESEC48];
		struct s_d {
			RECHEAD	data;
		} d;
	} u;
} SPWAW_SECTION48;

#endif	/* SECTION48_SPWAW_H */
