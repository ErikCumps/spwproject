/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION04_SPWAW_H
#define	SECTION04_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SECTION04 {
	union u_u {
		char	raw[SPWAW_SIZESEC04];
		struct s_d {
			MAP_ICON	icon[SPWAW_MAPWIDTH][SPWAW_MAPHEIGHT];
		} d;
	} u;
} SECTION04;

#endif	/* SECTION04_SPWAW_H */
