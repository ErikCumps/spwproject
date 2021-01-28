/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION38_WINSPMBT_H
#define	SECTION38_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_MAP_WIDTH {
	DWORD	size;	/* Map width in hexes	*/
} WINSPMBT_MAP_WIDTH;

typedef struct s_WINSPMBT_SECTION38 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC38];
		struct s_d {
			WINSPMBT_MAP_WIDTH	data;
		} d;
	} u;
} WINSPMBT_SECTION38;

#endif	/* SECTION38_WINSPMBT_H */
