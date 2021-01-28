/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION11_WINSPMBT_H
#define	SECTION11_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION11 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC11];
		struct s_d {
			char	data[WINSPMBT_SIZESEC11];
		} d;
	} u;
} WINSPMBT_SECTION11;

#endif	/* SECTION11_WINSPMBT_H */
