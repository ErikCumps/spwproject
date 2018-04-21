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
bread (int fd, char *b, int len, bool noerr)
{
	int	rc;

	rc = read (fd, b, len);

	IOLOG3 ("read(fd=%d, len=%d) = %d\n", fd, len, rc);

	if (rc == -1) {
		if (!noerr) ERROR1 ("failed to read from file (errno=%lu)", errno);
		return (false);
	}

	if (rc != len) {
		if (!noerr) ERROR2 ("failed to read from file (wanted=%lu, read=%lu)", len, rc);
		return (false);
	}
	return (rc == len);
}
