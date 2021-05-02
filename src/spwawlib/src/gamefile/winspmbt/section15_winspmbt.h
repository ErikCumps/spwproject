/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION15_WINSPMBT_H
#define	SECTION15_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION15 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC15];
		struct s_d {
			char	data[WINSPMBT_SIZESEC15];
		} d;
	} u;
} WINSPMBT_SECTION15;

#endif	/* SECTION15_WINSPMBT_H */