/** \file
 * The SPWaW Library - winSPWW2 gamefile handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION35_WINSPWW2_H
#define	SECTION35_WINSPWW2_H	1

#include "gamefile/winspww2/defines_winspww2.h"

typedef struct s_WINSPWW2_FORMATION {
	char	name[WINSPWW2_AZSNAME];	/* Formation name (ASCIIZ)	*/
	USHORT	OOBrid;			/* Formation OOB record ID	*/
	USHORT	hcmd;			/* Higher command leader ID	*/
	USHORT	leader;			/* Formation leader unit ID	*/
	char	__data00[6];
	USHORT	player;			/* Player ID			*/
	USHORT	ID;			/* Formation ID			*/
	BYTE	status:3;		/* Formation campaign status	*/
	BYTE	reserved:5;
	char	__data01[27];
} WINSPWW2_FORMATION;

typedef struct s_WINSPWW2_SECTION35 {
	union u_u {
		char	raw[WINSPWW2_SIZESEC35];
		struct s_d {
			WINSPWW2_FORMATION	formations[WINSPWW2_FORMCOUNT];
		} d;
	} u;
} WINSPWW2_SECTION35;

#endif	/* SECTION35_WINSPWW2_H */
