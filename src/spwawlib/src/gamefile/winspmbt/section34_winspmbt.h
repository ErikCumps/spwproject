/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION34_WINSPMBT_H
#define	SECTION34_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_LEADER {
	char	name[WINSPMBT_AZSNAME];	/* Leader name (ASCIIZ), includes trailing '\n'	*/
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
} WINSPMBT_LEADER;

typedef struct s_WINSPMBT_SECTION34 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC34];
		struct s_d {
			WINSPMBT_LEADER	leaders[WINSPMBT_LEADERCOUNT];
		} d;
	} u;
} WINSPMBT_SECTION34;

#endif	/* SECTION34_WINSPMBT_H */
