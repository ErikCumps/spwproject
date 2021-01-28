/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION22_WINSPMBT_H
#define	SECTION22_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION22 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC22];
		struct s_d {
			char	data[WINSPMBT_SIZESEC22];
		} d;
	} u;
} WINSPMBT_SECTION22;

#endif	/* SECTION22_WINSPMBT_H */
