/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION26_WINSPMBT_H
#define	SECTION26_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION26 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC26];
		struct s_d {
			char	data[WINSPMBT_SIZESEC26];
		} d;
	} u;
} WINSPMBT_SECTION26;

#endif	/* SECTION26_WINSPMBT_H */
