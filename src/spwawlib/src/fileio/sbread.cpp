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

SBR *
sbread_init (void *buf, long len)
{
	SBR	*sb = NULL;

	sb = safe_malloc (SBR); COOMRET (sb, "SBR", NULL);

	sb->buf = (char *)buf;
	sb->len = len;
	sb->idx = 0;
	sb->lft = len;

	return (sb);
}

int
sbread (SBR *sb, char *b, int len)
{
	int	done = 0;

	if (!sb || !b) {
		ERROR0 ("unexpected NULL argument(s)");
		return (0);
	}

	if (!len || !sb->lft) return (0);

	done = len; if (done > sb->lft) done = sb->lft;
	memcpy (b, sb->buf + sb->idx, done);
	sb->idx += done; sb->lft -= done;

	return (done);
}

void
sbread_stop (SBR *sb)
{
	if (!sb) return;

	safe_free (sb);
}
