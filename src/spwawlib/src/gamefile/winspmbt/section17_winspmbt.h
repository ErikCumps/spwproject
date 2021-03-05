/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION17_WINSPMBT_H
#define	SECTION17_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_UNIT_POS {
	USHORT	x;		/* Hex x-coordinate	*/
	USHORT	y;		/* Hex y-coordinate	*/
	char	__data00[10];
	BYTE	seen;		/* Spotted flag	0=seen, 1=hidden	*/
	char	__data01[4];
} WINSPMBT_UNIT_POS;

typedef struct s_WINSPMBT_SECTION17 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC17];
		struct s_d {
			WINSPMBT_UNIT_POS	pos[WINSPMBT_UNITPOSCOUNT];
		} d;
	} u;
} WINSPMBT_SECTION17;

#endif	/* SECTION17_WINSPMBT_H */
