/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION25_WINSPMBT_H
#define	SECTION25_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION25 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC25];
		struct s_d {
			char	data[WINSPMBT_SIZESEC25];
		} d;
	} u;
} WINSPMBT_SECTION25;

#endif	/* SECTION25_WINSPMBT_H */
