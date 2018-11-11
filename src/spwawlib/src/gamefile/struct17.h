/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2017 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT17_H
#define	INTERNAL_STRUCT17_H	1

#include "gamefile/struct_common.h"

//typedef struct s_UNIT_POS {
//	USHORT	x;		/* Hex x-coordinate	*/
//	USHORT	y;		/* Hex y-coordinate	*/
//	char	__data00[10];
//	BYTE	seen;		/* Spotted flag	0=seen, 1=hidden	*/
//	char	__data01[5];
//} UNIT_POS;

typedef struct s_UNIT_POS {
	USHORT	x;		/* Hex x-coordinate	*/
	USHORT	y;		/* Hex y-coordinate	*/
	char	__data00[10];
	BYTE	seen;		/* Spotted flag	0=seen, 1=hidden	*/
	char	__data01[4];
} UNIT_POS;

typedef struct s_STRUCT17 {
	union u_u {
		char	raw[SIZESEC17];
		struct s_d {
			UNIT_POS	pos[UNITPOSCOUNT];
		} d;
	} u;
} STRUCT17;

extern SPWAW_ERROR	sec17_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2);

#endif	/* INTERNAL_STRUCT17_H */
