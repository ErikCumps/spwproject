/** \file
 * The SPWaW Library - gamefile block compression handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	PACKING_H
#define	PACKING_H	1

#include "gamefile/gamedata.h"

typedef struct s_BLOCKHEAD {
	unsigned int	section;
	unsigned int	size;
	unsigned int	flag;
} BLOCKHEAD;

#define	FLAG_COMPRESSED	0x00000001

extern	bool	load_packed		(GAMEFILE *file, GAMEDATA *dst);
extern	bool	save_packed		(GAMEDATA *src, GAMEFILE *file);

extern	bool	load_packed_section	(GAMEFILE *file, DWORD sec, void **dst, unsigned long *len);

#endif	/* PACKING_H */
