/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT48_H
#define	INTERNAL_STRUCT48_H	1

#include "gamefile/struct_common.h"

typedef struct s_RECHEAD {
	char	__data00[4];
	DWORD	size;		/* Data size in bytes	*/
	char	__data01[72];
} RECHEAD;

typedef struct s_STRUCT48 {
	union u_u {
		char	raw[SIZESEC48];
		struct s_d {
			RECHEAD	data;
		} d;
	} u;
} STRUCT48;

#endif	/* INTERNAL_STRUCT48_H */
