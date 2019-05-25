/** \file
 * The SPWaW Library - winSPWW2 gamefile handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION48_WINSPWW2_H
#define	SECTION48_WINSPWW2_H	1

#include "gamefile/winspww2/defines_winspww2.h"

typedef struct s_WINSPWW2_RECHEAD {
	char	__data00[4];
	DWORD	size49;		/* Data size section #49 in bytes	*/
	char	__data01[14];
	DWORD	size50;		/* Data size section #50 in bytes	*/
	char	__data02[46];
} WINSPWW2_RECHEAD;

typedef struct s_WINSPWW2_SECTION48 {
	union u_u {
		char	raw[WINSPWW2_SIZESEC48];
		struct s_d {
			WINSPWW2_RECHEAD	data;
		} d;
	} u;
} WINSPWW2_SECTION48;

#endif	/* SECTION48_WINSPWW2_H */
