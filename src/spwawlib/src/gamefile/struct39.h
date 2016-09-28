/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT39_H
#define	INTERNAL_STRUCT39_H	1

#include "gamefile/struct_common.h"

typedef struct s_MAP_HEIGHT {
	DWORD	size;	/* Map height in hexes	*/
} MAP_HEIGHT;

typedef struct s_STRUCT39 {
	union u_u {
		char	raw[SIZESEC39];
		struct s_d {
			MAP_HEIGHT	data;
		} d;
	} u;
} STRUCT39;

extern SPWAW_ERROR	sec39_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

#endif	/* INTERNAL_STRUCT39_H */
