/** \file
 * The SPWaW Library - file I/O handling.
 *
 * Copyright (C) 2007-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "common/internal.h"

bool
bwrite (int fd, char *b, int len)
{
	int	rc;

	rc = write (fd, b, len);

	IOLOG3 ("write(fd=%d, len=%d) = %d\n", fd, len, rc);

	if (rc == -1) ERROR1 ("failed to write to file (errno=%lu)", errno);

	if (rc != len) ERROR2 ("failed to write to file (wanted=%lu, written=%lu)", len, rc);
	return (true);
}
