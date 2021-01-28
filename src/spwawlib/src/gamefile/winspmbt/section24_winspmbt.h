/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION24_WINSPMBT_H
#define	SECTION24_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION24 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC24];
		struct s_d {
			char	data[WINSPMBT_SIZESEC24];
		} d;
	} u;
} WINSPMBT_SECTION24;

#endif	/* SECTION24_WINSPMBT_H */
