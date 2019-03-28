/** \file
 * The SPWaW Library - SPWaW OOB list handling.
 *
 * Copyright (C) 2018-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "spwoob/spwoob_list.h"
#include "spwoob/spwoob_list_file.h"
#include "fileio/fileio.h"
#include "common/internal.h"

SPWAW_ERROR
SPWOOB_LIST_load (SPWAW_SPWOOB_LIST *list, int fd)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	SPWOOBLIST_HEADER	hdr;
	SPWOOBLIST_SPWOOB	*ohdr = NULL;
	long			p0;
	unsigned long		i;
	SPWOOB			*spwoob = NULL;

	CNULLARG (list);

	memset (&hdr, 0, sizeof (hdr));

	if (!bread (fd, (char *)&hdr, sizeof (hdr), false))
		RWE (SPWERR_FRFAILED, "bread(SPWOOBLIST header) failed");
	if ((memcmp (hdr.magic, SPWOOBLIST_MAGIC, SPWOOBLIST_MGCLEN) != 0) || (hdr.version != SPWOOBLIST_VERSION))
		RWE (SPWERR_BADSAVEDATA, "SPWOOBLIST header check failed");

	ohdr = safe_nmalloc (SPWOOBLIST_SPWOOB, hdr.cnt); COOMGOTO (ohdr, "SPWOOBLIST_SPWOOB list", handle_error);

	p0 = bseekget (fd);

	if (!bread (fd, (char *)ohdr, hdr.cnt * sizeof (SPWOOBLIST_SPWOOB), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(SPWOOBLIST SPWOOB data headers) failed", handle_error);

	for (i=0; i<hdr.cnt; i++) {
		rc = SPWOOB_new (&spwoob); ERRORGOTO ("SPWOOB_new()", handle_error);

		bseekset (fd, p0 + ohdr[i].data);
		rc = SPWOOB_load (spwoob, fd); ERRORGOTO ("SPWOOB_load()", handle_error);

		rc = SPWOOB_LIST_add (list, spwoob); ERRORGOTO ("SPWOOB_LIST_add()", handle_error);

		SPWOOB_free (&spwoob);
	}

	safe_free (ohdr);

	return (SPWERR_OK);

handle_error:
	if (spwoob) SPWOOB_free (&spwoob);
	if (ohdr) safe_free (ohdr);
	SPWOOB_LIST_clear (list);
	return (rc);
}
