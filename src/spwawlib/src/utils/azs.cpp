/** \file
 * The SPWaW Library - utility code: handling AZS strings.
 * (these are max length, possibly zero-terminated, ascii strings)
 *
 * Copyright (C) 2007-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "common/internal.h"

DWORD
azslen (char *azs, DWORD size)
{
	DWORD	i;

	for (i=0; i<size; i++) if (azs[i] == '\0') break;
	return (i);
}

void
azsclr (char *azs, DWORD size)
{
	fill_ptr_core (azs, size, 0);
}

void
azsset (char *azs, DWORD size, char *string)
{
	azsclr (azs, size);

	if (size <= 1) return;

	snprintf (azs, size - 1, "%s", string);
}

char *
azsdup (char *azs, DWORD size)
{
	char	*dup;

	if (!azs || !size) return (NULL);

	if (azslen (azs, size) < size) {
		dup = safe_strdup (azs);
	} else {
		dup = safe_nmalloc (char, size+1);
		if (dup) {
			memcpy (dup, azs, size);
			dup[size] = '\0';
		}
	}

	return (dup);
}

void
azscpy (char *src, char *dst, DWORD size)
{
	azsclr (dst, size);

	if (size <= 1) return;

	memcpy (dst, src, size - 1);
}