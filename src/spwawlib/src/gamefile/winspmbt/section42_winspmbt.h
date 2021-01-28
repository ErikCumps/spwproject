/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION42_WINSPMBT_H
#define	SECTION42_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION42 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC42];
		struct s_d {
			char	data[WINSPMBT_SIZESEC42];
		} d;
	} u;
} WINSPMBT_SECTION42;

#endif	/* SECTION42_WINSPMBT_H */
