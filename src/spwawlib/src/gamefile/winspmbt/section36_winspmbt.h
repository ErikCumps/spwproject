/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION36_WINSPMBT_H
#define	SECTION36_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION36 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC36];
		struct s_d {
			char	data[WINSPMBT_SIZESEC36];
		} d;
	} u;
} WINSPMBT_SECTION36;

#endif	/* SECTION36_WINSPMBT_H */
