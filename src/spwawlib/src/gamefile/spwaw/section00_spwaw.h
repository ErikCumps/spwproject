/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION00_SPWAW_H
#define	SECTION00_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_STARTSEC {
	char	info[20];	/* Savegame information string (ASCIIZ)	*/
} STARTSEC;

typedef struct s_SECTION00 {
	union u_u {
		char	raw[SPWAW_SIZESEC00];
		struct s_d {
			STARTSEC	info;
		} d;
	} u;
} SECTION00;

#endif	/* SECTION00_SPWAW_H */
