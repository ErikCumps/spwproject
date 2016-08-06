/** \file
 * The SPWaW Library - file I/O handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "fileio/fileio.h"
#include "common/internal.h"

BBW *
bbwrite_init (void *ptr, long len)
{
	BBW	*bb = NULL;

	bb = safe_malloc (BBW); COOMRET (bb, "BBW", NULL);

	bb->dst  = (char *)ptr;
	bb->left = len;
	bb->idx  = 0;

	return (bb);
}

void
bbwrite (BBW *bb, char *b)
{
	if (!bb || !b) {
		ERROR0 ("unexpected NULL argument");
		return;
	}

	if (!bb->left) {
		ERROR0 ("buffer overflow");
		return;
	}
	bb->dst[bb->idx] = *b;
	bb->idx++;
	bb->left--;
}

void
bbwrite (BBW *bb, char *b, int len)
{
	if (!bb || !b) {
		ERROR0 ("unexpected NULL argument");
		return;
	}

	if (bb->left < len) {
		ERROR0 ("buffer overflow");
		return;
	}
	memcpy (bb->dst + bb->idx, b, len);
	bb->idx  += len;
	bb->left -= len;
}

void
bbwrite_stop (BBW *bb)
{
	if (!bb) return;

	bb->dst = NULL;
	bb->left = bb->idx = 0;
	free (bb);
}
