/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION57_WINSPMBT_H
#define	SECTION57_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_SECTION57 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC57];
		struct s_d {
			char	data[WINSPMBT_SIZESEC57];
		} d;
	} u;
} WINSPMBT_SECTION57;

#endif	/* SECTION57_WINSPMBT_H */
