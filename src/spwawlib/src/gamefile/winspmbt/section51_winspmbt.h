/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION51_WINSPMBT_H
#define	SECTION51_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION51 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC51];
		struct s_d {
			char	data[WINSPMBT_SIZESEC51];
		} d;
	} u;
} WINSPMBT_SECTION51;

#endif	/* SECTION51_WINSPMBT_H */
