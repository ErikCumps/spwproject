/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION29_SPWAW_H
#define	SECTION29_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_SPWAW_MAP_DATA2 {
	BYTE	vis;	/* Visibility?	*/
} SPWAW_MAP_DATA2;

typedef struct s_SPWAW_SECTION29 {
	union u_u {
		char	raw[SPWAW_SIZESEC29];
		struct s_d {
			SPWAW_MAP_DATA2	data[SPWAW_MAPWIDTH][SPWAW_MAPHEIGHT];
		} d;
	} u;
} SPWAW_SECTION29;

#endif	/* SECTION29_SPWAW_H */
