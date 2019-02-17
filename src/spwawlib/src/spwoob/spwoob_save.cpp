/** \file
 * The SPWaW Library - SPWaW OOB handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "spwoob/spwoob.h"
#include "spwoob/spwoob_file.h"
#include "fileio/fileio.h"
#include "common/internal.h"
#include "utils/compression.h"

SPWAW_ERROR
SPWOOB_save (SPWOOB *oob, int fd, bool compress)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	SPWOOB_MV_HEADER	mvhdr;
	SPWOOB_HEADER		hdr;
	SPWOOB_OOBHDR		*ohdr = NULL;
	long			p0, p1;
	int			i, idx;
	SPWOOB_DATA		*p;
	CBIO			cbio;

	CNULLARG (oob);

	ohdr = safe_nmalloc (SPWOOB_OOBHDR, oob->count); COOMGOTO (ohdr, "SPWOOB_OOBDATA list", handle_error);

	memset (&mvhdr, 0, sizeof (mvhdr));

	memcpy (mvhdr.magic, SPWOOB_MAGIC, strlen (SPWOOB_MAGIC));
	mvhdr.version = SPWOOB_VERSION;

	if (!bwrite (fd, (char *)&mvhdr, sizeof (mvhdr)))
		FAILGOTO (SPWERR_FWFAILED, "bwrite(SPWOOB MV header) failed", handle_error);

	memset (&hdr, 0, sizeof (hdr));

	hdr.cnt = oob->count;
	hdr.gametype = oob->gametype;

	if (!bwrite (fd, (char *)&hdr, sizeof (hdr)))
		FAILGOTO (SPWERR_FWFAILED, "bwrite(SPWOOB header) failed", handle_error);

	p0 = bseekget (fd);

	for (i=idx=0; i<SPWOOB_DCNT; i++) {
		p = oob->data[i]; if (!p) continue;

		ohdr[idx].id	= p->id;
		ohdr[idx].crc32	= p->crc32;
		ohdr[idx].size	= p->rsize;

		bseekmove (fd, sizeof (SPWOOB_OOBHDR));

		idx++;
	}

	for (i=idx=0; i<SPWOOB_DCNT; i++) {
		p = oob->data[i]; if (!p) continue;

		ohdr[idx].data = bseekget (fd) - p0;

		cbio.data = (char *)(p->rdata); cbio.size = p->rsize; cbio.comp = &(ohdr[idx].comp);
		if (!cbwrite (fd, cbio, "raw SPWOOB data", compress))
			FAILGOTO (SPWERR_FWFAILED, "cbwrite(raw SPWOOB data) failed", handle_error);

		idx++;
	}

	p1 = bseekget (fd); bseekset (fd, p0);
	if (!bwrite (fd, (char *)ohdr, hdr.cnt * sizeof (SPWOOB_OOBHDR)))
		FAILGOTO (SPWERR_FWFAILED, "bwrite(SPWOOB data headers) failed", handle_error);
	bseekset (fd, p1);

	safe_free (ohdr);

	return (SPWERR_OK);

handle_error:
	if (ohdr) safe_free (ohdr);
	return (rc);
}
