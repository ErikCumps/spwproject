/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION10_WINSPMBT_H
#define	SECTION10_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION10 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC10];
		struct s_d {
			char	data[WINSPMBT_SIZESEC10];
		} d;
	} u;
} WINSPMBT_SECTION10;

#endif	/* SECTION10_WINSPMBT_H */
