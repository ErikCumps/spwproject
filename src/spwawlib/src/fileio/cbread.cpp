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
cbread_core (int fd, CBIO cbio, const char *m1, const char *m2, const char *m3, const char *m4)
{
	unsigned long	clen;
	char		*cbuf = NULL;
	SPWAW_ERROR	rc = SPWERR_OK;

	if ((clen = *cbio.comp) != 0) {
		cbuf = safe_smalloc (char, clen); COOMGOTO (cbuf, m1, handle_error);

		if (!bread (fd, cbuf, clen, false)) FAILGOTO (SPWERR_FRFAILED, m2, handle_error);

		rc = zexpand (cbuf, clen, cbio.data, cbio.size);
		ERRORGOTO (m3, handle_error);

		safe_free (cbuf);
	} else {
		if (!bread (fd, cbio.data, cbio.size, false)) FAILGOTO (SPWERR_FRFAILED, m4, handle_error);
	}

	return (true);

handle_error:
	if (cbuf) safe_free (cbuf);
	return (false);
}