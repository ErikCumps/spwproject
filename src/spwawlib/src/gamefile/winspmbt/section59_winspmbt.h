/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION59_WINSPMBT_H
#define	SECTION59_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION59 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC59];
		struct s_d {
			char	data[WINSPMBT_SIZESEC59];
		} d;
	} u;
} WINSPMBT_SECTION59;

#endif	/* SECTION59_WINSPMBT_H */
