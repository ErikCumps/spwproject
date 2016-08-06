/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT26_H
#define	INTERNAL_STRUCT26_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT26 {
	union {
		char	raw[SIZESEC26];
		struct {
			char	data[SIZESEC26];
		} d;
	} u;
} STRUCT26;

extern SPWAW_ERROR	sec26_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

#endif	/* INTERNAL_STRUCT26_H */
