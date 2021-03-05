/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION32_WINSPMBT_H
#define	SECTION32_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION32 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC32];
		struct s_d {
			char	data[WINSPMBT_SIZESEC32];
		} d;
	} u;
} WINSPMBT_SECTION32;

#endif	/* SECTION32_WINSPMBT_H */
