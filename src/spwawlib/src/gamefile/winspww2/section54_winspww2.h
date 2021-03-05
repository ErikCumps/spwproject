/** \file
 * The SPWaW Library - winSPWW2 gamefile handling.
 *
 * Copyright (C) 2019-2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION54_WINSPWW2_H
#define	SECTION54_WINSPWW2_H	1

#include "gamefile/winspww2/defines_winspww2.h"

typedef struct s_WINSPWW2_MAPHEXNAME {
	char	name[64];
} WINSPWW2_MAPHEXNAME;

typedef struct s_WINSPWW2_SECTION54 {
	union u_u {
		char	raw[WINSPWW2_SIZESEC54];
		struct s_d {
			WINSPWW2_MAPHEXNAME	name[100];
		} d;
	} u;
} WINSPWW2_SECTION54;

#endif	/* SECTION54_WINSPWW2_H */
