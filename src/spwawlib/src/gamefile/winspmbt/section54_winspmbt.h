/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION54_WINSPMBT_H
#define	SECTION54_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_MAPHEXNAME {
	char	name[64];
} WINSPMBT_MAPHEXNAME;

typedef struct s_WINSPMBT_SECTION54 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC54];
		struct s_d {
			WINSPMBT_MAPHEXNAME	name[100];
		} d;
	} u;
} WINSPMBT_SECTION54;

#endif	/* SECTION54_WINSPMBT_H */
