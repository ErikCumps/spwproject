/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION21_WINSPMBT_H
#define	SECTION21_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION21 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC21];
		struct s_d {
			char	data[WINSPMBT_SIZESEC21];
		} d;
	} u;
} WINSPMBT_SECTION21;

#endif	/* SECTION21_WINSPMBT_H */
