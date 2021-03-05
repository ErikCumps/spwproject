/** \file
 * The SPWaW Library - winSPMBT gamefile block handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	PACKING_WINSPMBT_H
#define	PACKING_WINSPMBT_H	1

#include "gamefile/packing.h"

extern	bool	load_winspmbt_block (int fd, BLOCKHEAD &block);
extern	bool	save_winspmbt_block (int fd, BLOCKHEAD &block);

#endif	/* PACKING_WINSPMBT_H */
