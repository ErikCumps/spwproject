/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
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

typedef struct s_SECTION48 {
	union u_u {
		char	raw[SIZESEC48];
		struct s_d {
			RECHEAD	data;
		} d;
	} u;
} SECTION48;

#endif	/* SECTION48_SPWAW_H */
