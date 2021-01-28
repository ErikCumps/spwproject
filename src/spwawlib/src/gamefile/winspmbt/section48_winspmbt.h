/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION48_WINSPMBT_H
#define	SECTION48_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_RECHEAD {
	char	__data00[4];
	DWORD	size49;		/* Data size section #49 in bytes	*/
	char	__data01[14];
	DWORD	size50;		/* Data size section #50 in bytes	*/
	char	__data02[46];
} WINSPMBT_RECHEAD;

typedef struct s_WINSPMBT_SECTION48 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC48];
		struct s_d {
			WINSPMBT_RECHEAD	data;
		} d;
	} u;
} WINSPMBT_SECTION48;

#endif	/* SECTION48_WINSPMBT_H */
