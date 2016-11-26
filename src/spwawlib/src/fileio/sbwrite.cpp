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

#define	BLOCKSIZE	(64*1024)

SBW *
sbwrite_init (void *buf, long len)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SBW		*sb = NULL;

	sb = safe_malloc (SBW); COOMGOTO (sb, "SBW", handle_error);

	if (!buf) {
		sb->grow = true;
		sb->buf = safe_smalloc (char, BLOCKSIZE);
		sb->len = BLOCKSIZE;
		COOMGOTO (sb->buf, "initial SBW buffer", handle_error);
	} else {
		sb->grow = false;
		sb->buf = (char *)buf;
		sb->len = len;
	}
	sb->idx = 0;
	sb->lft = sb->len;

	return (sb);

handle_error:
	if (sb) {
		if (sb->grow && sb->buf) safe_free (sb->buf);
		safe_free (sb);
	}
	return (NULL);
}

static bool
sbw_grow (SBW *sb)
{
	long	nlen, nlft;
	char	*nbuf;

	if (!sb || !sb->grow) return (false);

	nlen = sb->len + BLOCKSIZE;
	nlft = sb->lft + BLOCKSIZE;
	nbuf = (char *)safe_realloc (sb->buf, nlen); COOMRET (nbuf, "growing SBW buffer", false);

	sb->buf = nbuf;
	sb->len = nlen;
	sb->lft = nlft;

	return (true);
}

int
sbwrite (SBW *sb, char *b, int len)
{
	int	done, left;

	if (!sb || !b) {
		ERROR0 ("unexpected NULL argument(s)");
		return (0);
	}

	left = len; done = 0;
	while (left) {
		if (sb->lft == 0) {
			if (!sb->grow) {
				ERROR0 ("buffer overflow detected");
				break;
			} else if (!sbw_grow (sb)) {
				ERROR0 ("failed to grow buffer");
				break;
			}
		}

		int todo = left; if (todo > sb->lft) todo = sb->lft;
		if (todo) memcpy (sb->buf + sb->idx, b, todo);
		b += todo; left -= todo; sb->idx += todo; sb->lft -= todo; done += todo;
	}

	return (done);
}

void
sbwrite_stop (SBW *sb)
{
	sbwrite_stop (sb, NULL, NULL);
}

void
sbwrite_stop (SBW *sb, char **buf, long *len)
{
	if (!sb) return;

	if (buf && len) {
		*buf = sb->buf; sb->buf = NULL;
		*len = sb->idx; sb->len = 0;
	}

	if (sb->grow && sb->buf) safe_free (sb->buf);

	safe_free (sb);
}
