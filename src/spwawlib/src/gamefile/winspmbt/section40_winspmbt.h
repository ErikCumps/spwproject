/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION40_WINSPMBT_H
#define	SECTION40_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION40 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC40];
		struct s_d {
			char	data[WINSPMBT_SIZESEC40];
		} d;
	} u;
} WINSPMBT_SECTION40;

#endif	/* SECTION40_WINSPMBT_H */
