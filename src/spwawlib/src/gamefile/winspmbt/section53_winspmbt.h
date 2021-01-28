/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION53_WINSPMBT_H
#define	SECTION53_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION53 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC53];
		struct s_d {
			char	data[WINSPMBT_SIZESEC53];
		} d;
	} u;
} WINSPMBT_SECTION53;

#endif	/* SECTION53_WINSPMBT_H */
