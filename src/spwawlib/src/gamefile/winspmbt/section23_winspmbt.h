/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION23_WINSPMBT_H
#define	SECTION23_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION23 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC23];
		struct s_d {
			char	data[WINSPMBT_SIZESEC23];
		} d;
	} u;
} WINSPMBT_SECTION23;

#endif	/* SECTION23_WINSPMBT_H */
