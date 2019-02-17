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
#include "spwoob/spwoob_file_v1.h"
#include "fileio/fileio.h"
#include "common/internal.h"
#include "utils/compression.h"
#include "spwoob/raw.h"

SPWAW_ERROR
SPWOOB_load (SPWOOB *oob, SPWAW_GAME_TYPE gametype, const char *dir)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CNULLARG (dir);

	oob->gametype = gametype;
	snprintf (oob->srcdir, sizeof (oob->srcdir) - 1, "%s", dir);

	rc = spwoob_load_raw_files (oob);

	return (rc);
}

static SPWAW_ERROR
spwoob_check_magic_version (SPWOOB_MV_HEADER &hdr)
{
	if (memcmp (hdr.magic, SPWOOB_MAGIC, SPWOOB_MGCLEN) != 0)
		RWE (SPWERR_BADSAVEDATA, "SPWOOB header check failed");

	/* We are now backwards compatible with version 1 */
	if ((hdr.version != SPWOOB_VERSION) && (hdr.version < SPWOOB_VERSION_V1))
		RWE (SPWERR_INCOMPATIBLE, "SPWOOB header version check failed");

	return (SPWERR_OK);
}

SPWAW_ERROR
SPWOOB_load (SPWOOB *oob, int fd)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	SPWOOB_MV_HEADER	mvhdr;
	SPWOOB_HEADER		hdr;
	SPWOOB_OOBHDR		*ohdr = NULL;
	long			p0;
	BYTE			idx;
	SPWOOB_DATA		*p;
	CBIO			cbio;

	CNULLARG (oob);

	memset (&mvhdr, 0, sizeof (mvhdr));

	if (!bread (fd, (char *)&mvhdr, sizeof (mvhdr), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(SPWOOB MV header) failed", handle_error);

	rc = spwoob_check_magic_version (mvhdr);
	ERRORGOTO ("spwoob_check_magic_version()", handle_error);

	/* We are now backwards compatible with version 1 */
	if (mvhdr.version == SPWOOB_VERSION_V1) {
		rc = spwoob_load_v1_header (fd, &hdr);
		ERRORGOTO ("spwoob_load_v1_header(SPWOOB header)", handle_error);
	} else {
		if (!bread (fd, (char *)&hdr, sizeof (hdr), false))
			FAILGOTO (SPWERR_FRFAILED, "bread(SPWOOB header) failed", handle_error);
	}

	oob->count = hdr.cnt;
	oob->gametype = (SPWAW_GAME_TYPE)hdr.gametype;

	ohdr = safe_nmalloc (SPWOOB_OOBHDR, oob->count); COOMGOTO (ohdr, "SPWOOB_OOBDATA list", handle_error);

	p0 = bseekget (fd);

	if (!bread (fd, (char *)ohdr, oob->count * sizeof (SPWOOB_OOBHDR), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(SPWOOB data headers) failed", handle_error);

	for (idx=0; idx<oob->count; idx++) {
                p = safe_malloc (SPWOOB_DATA); COOMGOTO (p, "SPWOOB_DATA", handle_error);

		p->id	 = ohdr[idx].id;
		p->rsize = ohdr[idx].size;
		p->rdata = safe_smalloc (char, p->rsize); COOMGOTO (p->rdata, "raw SPWOOB data", handle_error);

		bseekset (fd, p0 + ohdr[idx].data);

		cbio.data = (char *)(p->rdata); cbio.size = p->rsize; cbio.comp = &(ohdr[idx].comp);
		if (!cbread (fd, cbio, "raw SPWOOB data"))
			FAILGOTO (SPWERR_FRFAILED, "cbread(raw SPWOOB data) failed", handle_error);

		rc = spwoob_load_raw_data (p); ERRORGOTO ("spwoob_load_raw_data()", handle_error);

		if (p->crc32 != ohdr[idx].crc32)
			FAILGOTO (SPWERR_FRFAILED, "SPWOOB data CRC check failed", handle_error);

		oob->data[p->id] = p;
	}

	safe_free (ohdr);

	return (SPWERR_OK);

handle_error:
	if (ohdr) safe_free (ohdr);
	spwoob_free (oob);
	return (rc);
}
