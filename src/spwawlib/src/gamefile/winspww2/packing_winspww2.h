/** \file
 * The SPWaW Library - winSPWW2 gamefile block handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	PACKING_WINSPWW2_H
#define	PACKING_WINSPWW2_H	1

#include "gamefile/packing.h"

extern	bool	load_winspww2_block (int fd, BLOCKHEAD &block);
extern	bool	save_winspww2_block (int fd, BLOCKHEAD &block);

#endif	/* PACKING_WINSPWW2_H */
