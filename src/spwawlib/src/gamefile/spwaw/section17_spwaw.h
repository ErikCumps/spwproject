/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION17_SPWAW_H
#define	SECTION17_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_UNIT_POS {
	USHORT	x;		/* Hex x-coordinate	*/
	USHORT	y;		/* Hex y-coordinate	*/
	char	__data00[10];
	BYTE	seen;		/* Spotted flag	0=seen, 1=hidden	*/
	char	__data01[5];
} UNIT_POS;

typedef struct s_SECTION17 {
	union u_u {
		char	raw[SPWAW_SIZESEC17];
		struct s_d {
			UNIT_POS	pos[SPWAW_UNITPOSCOUNT];
		} d;
	} u;
} SECTION17;

#endif	/* SECTION17_SPWAW_H */
