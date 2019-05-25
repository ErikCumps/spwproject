/** \file
 * The SPWaW Library - winSPWW2 gamefile handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION17_WINSPWW2_H
#define	SECTION17_WINSPWW2_H	1

#include "gamefile/winspww2/defines_winspww2.h"

typedef struct s_WINSPWW2_UNIT_POS {
	USHORT	x;		/* Hex x-coordinate	*/
	USHORT	y;		/* Hex y-coordinate	*/
	char	__data00[10];
	BYTE	seen;		/* Spotted flag	0=seen, 1=hidden	*/
	char	__data01[4];
} WINSPWW2_UNIT_POS;

typedef struct s_WINSPWW2_SECTION17 {
	union u_u {
		char	raw[WINSPWW2_SIZESEC17];
		struct s_d {
			WINSPWW2_UNIT_POS	pos[WINSPWW2_UNITPOSCOUNT];
		} d;
	} u;
} WINSPWW2_SECTION17;

#endif	/* SECTION17_WINSPWW2_H */
