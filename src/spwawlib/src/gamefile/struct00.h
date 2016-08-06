/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT00_H
#define	INTERNAL_STRUCT00_H	1

#include "gamefile/struct_common.h"

typedef struct s_STARTSEC {
	char	info[20];	/* Savegame information string (ASCIIZ)	*/
} STARTSEC;

typedef struct s_STRUCT00 {
	union {
		char	raw[SIZESEC00];
		struct {
			STARTSEC	info;
		} d;
	} u;
} STRUCT00;

#endif	/* INTERNAL_STRUCT00_H */
