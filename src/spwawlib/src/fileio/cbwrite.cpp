/** \file
 * The SPWaW Library - file I/O handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "common/internal.h"
#include "fileio/fileio.h"
#include "utils/compression.h"

bool
cbwrite_core (int fd, CBIO cbio, bool compress, const char *m1, const char *m2, const char *m3)
{
	long		p0;
	char		*cbuf = NULL;
	unsigned long	clen;
	SPWAW_ERROR	rc = SPWERR_OK;

	p0 = bseekget (fd);

	if (compress) {
		rc = zcompress (cbio.data, cbio.size, &cbuf, &clen);
		ERRORGOTO (m1, handle_error);

		if (!bwrite (fd, cbuf, clen)) FAILGOTO (SPWERR_FWFAILED, m2, handle_error);
		safe_free (cbuf);

		*cbio.comp = clen;
	} else {
		if (!bwrite (fd, cbio.data, cbio.size)) FAILGOTO (SPWERR_FWFAILED, m3, handle_error);

		*cbio.comp = 0;
	}

	return (true);

handle_error:
	if (cbuf) safe_free (cbuf);
	bseekset (fd, p0);
	return (false);
}
