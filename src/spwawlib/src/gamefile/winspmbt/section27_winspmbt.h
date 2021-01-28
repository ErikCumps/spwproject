/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION27_WINSPMBT_H
#define	SECTION27_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION27 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC27];
		struct s_d {
			char	data[WINSPMBT_SIZESEC27];
		} d;
	} u;
} WINSPMBT_SECTION27;

#endif	/* SECTION27_WINSPMBT_H */
