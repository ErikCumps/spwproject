/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION33_WINSPMBT_H
#define	SECTION33_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION33 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC33];
		struct s_d {
			char	data[WINSPMBT_SIZESEC33];
		} d;
	} u;
} WINSPMBT_SECTION33;

#endif	/* SECTION33_WINSPMBT_H */
