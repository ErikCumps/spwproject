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
SPWOOB_LIST_save (SPWAW_SPWOOB_LIST *list, int fd, bool compress)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	SPWOOBLIST_HEADER	hdr;
	SPWOOBLIST_SPWOOB	*ohdr = NULL;
	long			p0, p1;
	unsigned long		i;

	CNULLARG (list);

	ohdr = safe_nmalloc (SPWOOBLIST_SPWOOB, list->cnt); COOMGOTO (ohdr, "SPWOOBLIST_SPWOOB list", handle_error);

	memset (&hdr, 0, sizeof (hdr));

	memcpy (hdr.magic, SPWOOBLIST_MAGIC, strlen (SPWOOBLIST_MAGIC));
	hdr.version = SPWOOBLIST_VERSION;

	hdr.cnt = list->cnt;

	if (!bwrite (fd, (char *)&hdr, sizeof (hdr)))
		FAILGOTO (SPWERR_FWFAILED, "bwrite(SPWOOBLIST header) failed", handle_error);

	p0 = bseekget (fd);

	for (i=0; i<hdr.cnt; i++) {
		bseekmove (fd, sizeof (SPWOOBLIST_SPWOOB));
	}

	for (i=0; i<hdr.cnt; i++) {
		ohdr[i].data = bseekget (fd) - p0;

		rc = SPWOOB_save (list->list[i]->data, fd, compress);
		ERRORGOTO ("SPWOOB_save()", handle_error);
	}

	p1 = bseekget (fd); bseekset (fd, p0);
	if (!bwrite (fd, (char *)ohdr, hdr.cnt * sizeof (SPWOOBLIST_SPWOOB)))
		FAILGOTO (SPWERR_FWFAILED, "bwrite(SPWOOBLIST SPWOOB data headers) failed", handle_error);
	bseekset (fd, p1);

	safe_free (ohdr);

	return (SPWERR_OK);

handle_error:
	if (ohdr) safe_free (ohdr);
	return (rc);
}
