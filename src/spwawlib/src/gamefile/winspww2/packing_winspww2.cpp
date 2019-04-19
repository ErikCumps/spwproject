/** \file
 * The SPWaW Library - winSPWW2 gamefile block handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/winspww2/packing_winspww2.h"
#include "fileio/fileio.h"

#pragma pack(push, r1, 1)
typedef struct s_BLOCKHEAD_WINSPWW2 {
	unsigned int	section;
	unsigned int	size;
	unsigned char	flag;
} BLOCKHEAD_WINSPWW2;
#pragma pack(pop, r1)

bool
load_winspww2_block (int fd, BLOCKHEAD &block)
{
	BLOCKHEAD_WINSPWW2	b;

	if (!bread (fd, (char *)&b, sizeof (b), true)) return (false);

	block.section = b.section;
	block.size    = b.size;
	block.flag    = b.flag;

	return (true);
}

bool
save_winspww2_block (int fd, BLOCKHEAD &block)
{
	BLOCKHEAD_WINSPWW2	b;

	b.section = block.section;
	b.size    = block.size;
	b.flag    = (unsigned char)(block.flag & 0xFF);

	return (bwrite (fd, (char *)&b, sizeof (b)));
}
