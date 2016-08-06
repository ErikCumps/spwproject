/** \file
 * The SPWaW Library - file I/O handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "common/internal.h"

void
bseekset (int fd, long pos)
{
	long	offs;

	offs = lseek (fd, pos, SEEK_SET);
	if (offs == -1) ERROR2 ("failed to seek to file offset %lu (errno=%lu)", pos, errno);
	if (offs != pos) ERROR2 ("failed to seek to file offset %lu (obtained %lu)", pos, offs);
}

void
bseekmove (int fd, long len)
{
	long	offs;

	offs = lseek (fd, len, SEEK_CUR);
	if (offs == -1) ERROR2 ("failed to move file offset with %lu (errno=%lu)", len, errno);
}

long
bseekget (int fd)
{
	long	offs;

	offs = lseek (fd, 0, SEEK_CUR);
	if (offs == -1) ERROR1 ("failed to get file offset (errno=%lu)", errno);
	return (offs);
}
