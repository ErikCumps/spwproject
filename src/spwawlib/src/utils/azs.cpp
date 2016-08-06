/** \file
 * The SPWaW Library - utility code: handling AZS strings.
 * (these are max length, possibly zero-terminated, ascii strings)
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "common/internal.h"

char *
dupazs (char *string, DWORD len)
{
	DWORD	i;
	char	*dup;

	if (!string || !len) return (NULL);

	for (i=0; i<len; i++) if (string[i] == '\0') break;

	if (string[i] == '\0')
		dup = strdup ((const char *)string);
	else {
		dup = safe_nmalloc (char, len+1);
		if (dup) {
			memcpy (dup, string, len);
			dup[len] = '\0';
		}
	}

	return (dup);
}
