/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION19_WINSPMBT_H
#define	SECTION19_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION19 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC19];
		struct s_d {
			char	data[WINSPMBT_SIZESEC19];
		} d;
	} u;
} WINSPMBT_SECTION19;

#endif	/* SECTION19_WINSPMBT_H */
