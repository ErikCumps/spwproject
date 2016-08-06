/** \file
 * The SPWaW Library - utility code: error handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "common/internal.h"

void
error (const char *file, unsigned long line, const char *func, char *fmt, ...)
{
	va_list	AP;
	char	buf[1024];

	memset (buf, 0, sizeof (buf));
	va_start (AP, fmt);
	vsnprintf (buf, sizeof (buf) - 1, fmt, AP);
	va_end (AP);
	log ("*** %s:%lu %s: Error: %s!\n", file, line, func, buf);
}
