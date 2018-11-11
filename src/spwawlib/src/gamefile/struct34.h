/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2017 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT34_H
#define	INTERNAL_STRUCT34_H	1

#include "gamefile/struct_common.h"

//typedef struct s_LEADER {
//	char	name[SPWAW_AZSNAME];	/* Leader name (ASCIIZ)				*/
//	USHORT	UID;			/* Unit record ID???				*/
//	char	__data00[1];
//	BYTE	rank;			/* Leader rank					*/
//	BYTE	rally;			/* Leader rally skill				*/
//	BYTE	skinf;			/* Leader infantry skill			*/
//	BYTE	skart;			/* Leader artillery skill			*/
//	BYTE	skarm;			/* Leader armor skill				*/
//	char	__data01[4];
//	BYTE	kills;			/* Leader kill count				*/
//	char	__data02[1];
//	BYTE	status;			/* Leader status				*/
//	char	__data03[1];
//} LEADER;

typedef struct s_LEADER {
	char	name[SPWAW_AZSNAME];	/* Leader name (ASCIIZ), includes trailing '\n'	*/
	USHORT	UID;			/* Unit record ID???				*/
	char	__data00[1];
	BYTE	rank;			/* Leader rank					*/
	BYTE	rally;			/* Leader rally skill				*/
	BYTE	skinf;			/* Leader infantry skill			*/
	BYTE	skart;			/* Leader artillery skill			*/
	BYTE	skarm;			/* Leader armor skill				*/
	char	__data01[3];
	USHORT	kills;			/* Leader kill count				*/
	BYTE	status;			/* Leader status				*/
	char	__data02[10];
} LEADER;

typedef struct s_STRUCT34 {
	union u_u {
		char	raw[SIZESEC34];
		struct s_d {
			LEADER	leaders[LEADERCOUNT];
		} d;
	} u;
} STRUCT34;

extern SPWAW_ERROR	sec34_save_snapshot	(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2);

#endif	/* INTERNAL_STRUCT34_H */
