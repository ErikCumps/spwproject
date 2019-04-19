/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION35_SPWAW_H
#define	SECTION35_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_FORMATION {
	char	name[SPWAW_AZSNAME];	/* Formation name (ASCIIZ)	*/
	BYTE	OOBrid;			/* Formation OOB record ID?	*/
	char	__data000[1];
	USHORT	hcmd;			/* Higher command leader ID	*/
	USHORT	leader;			/* Formation leader unit ID	*/
	char	__data0010[6];
	BYTE	player;			/* Player ID			*/
	char	__data0011[1];
	BYTE	ID;			/* Formation ID			*/
	char	__data010[1];
	BYTE	status:3;		/* Formation campaign status	*/
	BYTE	reserved:5;
	char	__data011[13];
} FORMATION;

typedef struct s_SECTION35 {
	union u_u {
		char	raw[SIZESEC35];
		struct s_d {
			FORMATION	formations[FORMCOUNT];
		} d;
	} u;
} SECTION35;

#endif	/* SECTION35_SPWAW_H */
