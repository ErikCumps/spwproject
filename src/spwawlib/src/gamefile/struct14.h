/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT14_H
#define	INTERNAL_STRUCT14_H	1

#include "gamefile/struct_common.h"

typedef struct s_CREDIT {
	DWORD	amount;	/* Amount of available purchase points	*/
} CREDIT;

typedef struct s_STRUCT14 {
	union {
		char	raw[SIZESEC14];
		struct {
			CREDIT	data;
		} d;
	} u;
} STRUCT14;

extern SPWAW_ERROR	sec14_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

#endif	/* INTERNAL_STRUCT14_H */
