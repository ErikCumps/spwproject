/** \file
 * The SPWaW Library - SPWaW gamefile block handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	PACKING_SPWAW_H
#define	PACKING_SPWAW_H	1

#include "gamefile/packing.h"

extern	bool	load_spwaw_block (int fd, BLOCKHEAD &block);
extern	bool	save_spwaw_block (int fd, BLOCKHEAD &block);

#endif	/* PACKING_SPWAW_H */
