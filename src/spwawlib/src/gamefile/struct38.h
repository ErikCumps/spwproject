/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT38_H
#define	INTERNAL_STRUCT38_H	1

#include "gamefile/struct_common.h"

typedef struct s_MAP_WIDTH {
	DWORD	size;	/* Map width in hexes	*/
} MAP_WIDTH;

typedef struct s_STRUCT38 {
	union u_u {
		char	raw[SIZESEC38];
		struct s_d {
			MAP_WIDTH	data;
		} d;
	} u;
} STRUCT38;

extern SPWAW_ERROR	sec38_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

#endif	/* INTERNAL_STRUCT38_H */
