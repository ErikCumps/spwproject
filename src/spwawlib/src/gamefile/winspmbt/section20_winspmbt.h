/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION20_WINSPMBT_H
#define	SECTION20_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION20 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC20];
		struct s_d {
			char	data[WINSPMBT_SIZESEC20];
		} d;
	} u;
} WINSPMBT_SECTION20;

#endif	/* SECTION20_WINSPMBT_H */
