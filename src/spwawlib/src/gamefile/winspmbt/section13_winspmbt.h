/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION13_WINSPMBT_H
#define	SECTION13_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION13 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC13];
		struct s_d {
			char	data[WINSPMBT_SIZESEC13];
		} d;
	} u;
} WINSPMBT_SECTION13;

#endif	/* SECTION13_WINSPMBT_H */
