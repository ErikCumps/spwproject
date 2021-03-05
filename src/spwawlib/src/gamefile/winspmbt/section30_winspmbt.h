/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION30_WINSPMBT_H
#define	SECTION30_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION30 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC30];
		struct s_d {
			char	data[WINSPMBT_SIZESEC30];
		} d;
	} u;
} WINSPMBT_SECTION30;

#endif	/* SECTION30_WINSPMBT_H */
