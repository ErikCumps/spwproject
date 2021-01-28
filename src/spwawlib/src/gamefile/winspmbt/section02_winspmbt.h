/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION02_WINSPMBT_H
#define	SECTION02_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"
#include "gamefile/maptile.h"

typedef struct s_WINSPMBT_SECTION02 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC02];
		struct s_d {
			MAP_TILE	tile[WINSPMBT_MAPWIDTH][WINSPMBT_MAPHEIGHT];
		} d;
	} u;
} WINSPMBT_SECTION02;

#endif	/* SECTION02_WINSPMBT_H */
