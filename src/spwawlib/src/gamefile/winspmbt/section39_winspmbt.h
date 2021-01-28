/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION39_WINSPMBT_H
#define	SECTION39_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_MAP_HEIGHT {
	DWORD	size;	/* Map height in hexes	*/
} WINSPMBT_MAP_HEIGHT;

typedef struct s_WINSPMBT_SECTION39 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC39];
		struct s_d {
			WINSPMBT_MAP_HEIGHT	data;
		} d;
	} u;
} WINSPMBT_SECTION39;

#endif	/* SECTION39_WINSPMBT_H */
