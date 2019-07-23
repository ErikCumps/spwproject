/** \file
 * The SPWaW Library - winSPWW2 gamefile handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION33_WINSPWW2_H
#define	SECTION33_WINSPWW2_H	1

#include "gamefile/winspww2/defines_winspww2.h"

typedef struct s_WINSPWW2_SECTION33 {
	union u_u {
		char	raw[WINSPWW2_SIZESEC33];
		struct s_d {
			char	data[WINSPWW2_SIZESEC33];
		} d;
	} u;
} WINSPWW2_SECTION33;

#endif	/* SECTION33_WINSPWW2_H */