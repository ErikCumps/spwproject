/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION09_WINSPMBT_H
#define	SECTION09_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_MAP_DATA0 {
	char	__data[1];
} WINSPMBT_MAP_DATA0;

typedef struct s_WINSPMBT_SECTION09 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC09];
		struct s_d {
			WINSPMBT_MAP_DATA0	data[WINSPMBT_MAPWIDTH][WINSPMBT_MAPHEIGHT];
		} d;
	} u;
} WINSPMBT_SECTION09;

#endif	/* SECTION09_WINSPMBT_H */
