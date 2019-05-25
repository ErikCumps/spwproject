/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION34_SPWAW_H
#define	SECTION34_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SPWAW_LEADER {
	char	name[SPWAW_AZSNAME];	/* Leader name (ASCIIZ)				*/
	USHORT	UID;			/* Unit record ID???				*/
	char	__data00[1];
	BYTE	rank;			/* Leader rank					*/
	BYTE	rally;			/* Leader rally skill				*/
	BYTE	skinf;			/* Leader infantry skill			*/
	BYTE	skart;			/* Leader artillery skill			*/
	BYTE	skarm;			/* Leader armor skill				*/
	char	__data01[4];
	BYTE	kills;			/* Leader kill count				*/
	char	__data02[1];
	BYTE	status;			/* Leader status				*/
	char	__data03[1];
} SPWAW_LEADER;

typedef struct s_SPWAW_SECTION34 {
	union u_u {
		char	raw[SPWAW_SIZESEC34];
		struct s_d {
			SPWAW_LEADER	leaders[SPWAW_LEADERCOUNT];
		} d;
	} u;
} SPWAW_SECTION34;

#endif	/* SECTION34_SPWAW_H */
