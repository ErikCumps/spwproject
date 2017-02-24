/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2017 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT35_H
#define	INTERNAL_STRUCT35_H	1

#include "gamefile/struct_common.h"

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

typedef struct s_STRUCT35 {
	union u_u {
		char	raw[SIZESEC35];
		struct s_d {
			FORMATION	formations[FORMCOUNT];
		} d;
	} u;
} STRUCT35;

extern SPWAW_ERROR	sec35_detection		(GAMEDATA *src, FULIST &ful1, FULIST &ful2);
extern SPWAW_ERROR	sec35_save_snapshot	(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2);


#endif	/* INTERNAL_STRUCT35_H */
