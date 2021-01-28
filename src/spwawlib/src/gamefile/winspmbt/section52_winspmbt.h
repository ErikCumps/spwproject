/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION52_WINSPMBT_H
#define	SECTION52_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION52 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC52];
		struct s_d {
			char	data[WINSPMBT_SIZESEC52];
		} d;
	} u;
} WINSPMBT_SECTION52;

#endif	/* SECTION52_WINSPMBT_H */
