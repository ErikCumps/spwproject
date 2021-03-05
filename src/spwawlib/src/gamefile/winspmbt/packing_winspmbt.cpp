/** \file
 * The SPWaW Library - winSPMBT gamefile block handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/winspmbt/packing_winspmbt.h"
#include "fileio/fileio.h"

#pragma pack(push, r1, 1)
typedef struct s_BLOCKHEAD_WINSPMBT {
	unsigned int	section;
	unsigned int	size;
	unsigned char	flag;
} BLOCKHEAD_WINSPMBT;
#pragma pack(pop, r1)

bool
load_winspmbt_block (int fd, BLOCKHEAD &block)
{
	BLOCKHEAD_WINSPMBT	b;

	if (!bread (fd, (char *)&b, sizeof (b), true)) return (false);

	block.section = b.section;
	block.size    = b.size;
	block.flag    = b.flag;

	return (true);
}

bool
save_winspmbt_block (int fd, BLOCKHEAD &block)
{
	BLOCKHEAD_WINSPMBT	b;

	b.section = block.section;
	b.size    = block.size;
	b.flag    = (unsigned char)(block.flag & 0xFF);

	return (bwrite (fd, (char *)&b, sizeof (b)));
}
