/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION43_WINSPMBT_H
#define	SECTION43_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION43 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC43];
		struct s_d {
			char	data[WINSPMBT_SIZESEC43];
		} d;
	} u;
} WINSPMBT_SECTION43;

#endif	/* SECTION43_WINSPMBT_H */