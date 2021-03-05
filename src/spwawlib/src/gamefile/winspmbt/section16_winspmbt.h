/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION16_WINSPMBT_H
#define	SECTION16_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION16 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC16];
		struct s_d {
			char	data[WINSPMBT_SIZESEC16];
		} d;
	} u;
} WINSPMBT_SECTION16;

#endif	/* SECTION16_WINSPMBT_H */
