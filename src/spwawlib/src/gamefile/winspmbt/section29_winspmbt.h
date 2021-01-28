/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION29_WINSPMBT_H
#define	SECTION29_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_MAP_DATA2 {
	USHORT	vis;	/* Visibility?	*/
} WINSPMBT_MAP_DATA2;

typedef struct s_WINSPMBT_SECTION29 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC29];
		struct s_d {
			WINSPMBT_MAP_DATA2	data[WINSPMBT_MAPWIDTH][WINSPMBT_MAPHEIGHT];
		} d;
	} u;
} WINSPMBT_SECTION29;

#endif	/* SECTION29_WINSPMBT_H */
