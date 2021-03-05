/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION00_WINSPMBT_H
#define	SECTION00_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_STARTSEC {
	char	info[20];	/* Savegame information string (ASCIIZ)	*/
} WINSPMBT_STARTSEC;

typedef struct s_WINSPMBT_SECTION00 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC00];
		struct s_d {
			WINSPMBT_STARTSEC	info;
		} d;
	} u;
} WINSPMBT_SECTION00;

#endif	/* SECTION00_WINSPMBT_H */
