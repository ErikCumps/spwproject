/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION56_WINSPMBT_H
#define	SECTION56_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION56 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC56];
		struct s_d {
			char	data[WINSPMBT_SIZESEC56];
		} d;
	} u;
} WINSPMBT_SECTION56;

#endif	/* SECTION56_WINSPMBT_H */
