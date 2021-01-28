/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION14_WINSPMBT_H
#define	SECTION14_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION14 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC14];
		struct s_d {
			char	data[WINSPMBT_SIZESEC14];
		} d;
	} u;
} WINSPMBT_SECTION14;

#endif	/* SECTION14_WINSPMBT_H */
