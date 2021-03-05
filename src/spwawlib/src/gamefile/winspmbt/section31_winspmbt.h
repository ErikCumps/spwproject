/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION31_WINSPMBT_H
#define	SECTION31_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION31 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC31];
		struct s_d {
			char	data[WINSPMBT_SIZESEC31];
		} d;
	} u;
} WINSPMBT_SECTION31;

#endif	/* SECTION31_WINSPMBT_H */
