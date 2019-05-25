/** \file
 * The SPWaW Library - winSPWW2 gamefile handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION34_WINSPWW2_H
#define	SECTION34_WINSPWW2_H	1

#include "gamefile/winspww2/defines_winspww2.h"

typedef struct s_WINSPWW2_LEADER {
	char	name[WINSPWW2_AZSNAME];	/* Leader name (ASCIIZ), includes trailing '\n'	*/
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
} WINSPWW2_LEADER;

typedef struct s_WINSPWW2_SECTION34 {
	union u_u {
		char	raw[WINSPWW2_SIZESEC34];
		struct s_d {
			WINSPWW2_LEADER	leaders[WINSPWW2_LEADERCOUNT];
		} d;
	} u;
} WINSPWW2_SECTION34;

#endif	/* SECTION34_WINSPWW2_H */
