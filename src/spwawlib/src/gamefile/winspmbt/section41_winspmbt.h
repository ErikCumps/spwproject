/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION41_WINSPMBT_H
#define	SECTION41_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION41 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC41];
		struct s_d {
			char	data[WINSPMBT_SIZESEC41];
		} d;
	} u;
} WINSPMBT_SECTION41;

#endif	/* SECTION41_WINSPMBT_H */
