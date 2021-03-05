/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION35_WINSPMBT_H
#define	SECTION35_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_FORMATION {
	char	name[WINSPMBT_AZSNAME];	/* Formation name (ASCIIZ)	*/
	USHORT	OOBrid;			/* Formation OOB record ID	*/
	USHORT	hcmd;			/* Higher command leader ID	*/
	USHORT	leader;			/* Formation leader unit ID	*/
	char	__data00[6];
	USHORT	player;			/* Player ID			*/
	USHORT	ID;			/* Formation ID			*/
	BYTE	status:3;		/* Formation campaign status	*/
	BYTE	reserved:5;
	char	__data01[27];
} WINSPMBT_FORMATION;

typedef struct s_WINSPMBT_SECTION35 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC35];
		struct s_d {
			WINSPMBT_FORMATION	formations[WINSPMBT_FORMCOUNT];
		} d;
	} u;
} WINSPMBT_SECTION35;

#endif	/* SECTION35_WINSPMBT_H */
