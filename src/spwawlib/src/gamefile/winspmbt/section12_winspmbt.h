/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION12_WINSPMBT_H
#define	SECTION12_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION12 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC12];
		struct s_d {
			char	data[WINSPMBT_SIZESEC12];
		} d;
	} u;
} WINSPMBT_SECTION12;

#endif	/* SECTION12_WINSPMBT_H */
