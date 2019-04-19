/** \file
 * The SPWaW Library - SPWaW gamefile block handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/spwaw/packing_spwaw.h"
#include "fileio/fileio.h"

#pragma pack(push, r1, 1)
typedef struct s_BLOCKHEAD_SPWAW {
	unsigned int	section;
	unsigned int	size;
	unsigned int	flag;
} BLOCKHEAD_SPWAW;
#pragma pack(pop, r1)

bool
load_spwaw_block (int fd, BLOCKHEAD &block)
{
	BLOCKHEAD_SPWAW	b;

	if (!bread (fd, (char *)&b, sizeof (b), true)) return (false);

	block.section = b.section;
	block.size    = b.size;
	block.flag    = b.flag;

	return (true);
}

bool
save_spwaw_block (int fd, BLOCKHEAD &block)
{
	BLOCKHEAD_SPWAW	b;

	b.section = block.section;
	b.size    = block.size;
	b.flag    = block.flag;

	return (bwrite (fd, (char *)&b, sizeof (b)));
}
