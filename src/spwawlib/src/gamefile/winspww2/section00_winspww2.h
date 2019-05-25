/** \file
 * The SPWaW Library - winSPWW2 gamefile handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION00_WINSPWW2_H
#define	SECTION00_WINSPWW2_H	1

#include "gamefile/winspww2/defines_winspww2.h"

typedef struct s_WINSPWW2_STARTSEC {
	char	info[20];	/* Savegame information string (ASCIIZ)	*/
} WINSPWW2_STARTSEC;

typedef struct s_WINSPWW2_SECTION00 {
	union u_u {
		char	raw[WINSPWW2_SIZESEC00];
		struct s_d {
			WINSPWW2_STARTSEC	info;
		} d;
	} u;
} WINSPWW2_SECTION00;

#endif	/* SECTION00_WINSPWW2_H */
