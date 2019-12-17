/** \file
 * The SPWaW Library - unit history tracking handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "uht/uht.h"
#include "uht/uht_common.h"
#include "uht/uht_file.h"
#include "common/internal.h"
#include "fileio/fileio.h"
#include "utils/compression.h"
#include "snapshot/translate.h"

static void
prep_uhte_data (SPWAW_UHTE *uhte, UHT_ELEMENT &data, STRTAB *stab)
{
	memset (&data, 0, sizeof(data));

	data.idx	= uhte->idx;
	data.lname	= STRTAB_getidx (stab, uhte->lname);
	data.UID	= STRTAB_getidx (stab, uhte->UID);
	data.uname	= STRTAB_getidx (stab, uhte->uname);
	data.rank	= rank2raw (uhte->rank);
	data.FMID	= (USHORT)uhte->FMID;
	data.FSID	= (BYTE)uhte->FSID;
	data.FBIi	= uhte->FBI.btlidx;
	SPWAW_date2stamp (&(uhte->FBI.date), &(data.FBId));
	data.FUI	= uhte->FUI;
	data.LBIi	= uhte->LBI.btlidx;
	SPWAW_date2stamp (&(uhte->LBI.date), &(data.LBId));
	data.status	= uhte->status;
	data.prev	= uhte->prev?uhte->prev->idx:SPWAW_BADLONGIDX;
	data.next	= uhte->next?uhte->next->idx:SPWAW_BADLONGIDX;
}

SPWAW_ERROR
UHT_save (SPWAW_UHT *src, int fd, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	STRTAB		*stab = NULL;
	UHT_HEADER	hdr;
	long		p0, p1;
	SBW		*sbw = NULL;
	UHT_ELEMENT	uhte;
	char		*data = NULL;
	long		size;
	CBIO		cbio;

	CNULLARG (src);
	stab = (STRTAB *)src->dossier->stab;

	p0 = bseekget (fd);
	bseekmove (fd, sizeof (hdr));

	sbw = sbwrite_init (NULL, 0);
	if (!sbw) FAILGOTO (SPWERR_FWFAILED, "sbwrite_init() failed", handle_error);

	/* Prepare header */
	memset (&hdr, 0, sizeof (hdr));

	memcpy (hdr.magic, UHT_MAGIC, strlen (UHT_MAGIC));
	hdr.version = UHT_VERSION;
	hdr.cnt = src->cnt;

	/* Collect uhte data in a single buffer, replacing pointers by indexes */
	for (ULONG i=0; i<hdr.cnt; i++) {
		prep_uhte_data (src->list[i], uhte, stab);
		if (sbwrite (sbw, (char *)&uhte, sizeof (uhte)) != sizeof (uhte))
			FAILGOTO (SPWERR_FWFAILED, "sbwrite(uhte data)", handle_error);
	}
	sbwrite_stop (sbw, &data, &size); sbw = NULL;

	/* Update header and write uhte data buffer */
	hdr.size = size;
	hdr.data = bseekget (fd) - p0;

	cbio.data = data; cbio.size = size; cbio.comp = &(hdr.comp);
	if (!cbwrite (fd, cbio, "uhte data", compress)) FAILGOTO (SPWERR_FWFAILED, "cbwrite(uhte data)", handle_error);
	safe_free (data);

	/* Write header */
	p1 = bseekget (fd); bseekset (fd, p0);
	if (!bwrite (fd, (char *)&hdr, sizeof (hdr))) FAILGOTO (SPWERR_FWFAILED, "bwrite(uht hdr) failed", handle_error);
	bseekset (fd, p1);

	return (SPWERR_OK);

handle_error:
	if (data) safe_free (data);
	if (sbw) sbwrite_stop (sbw);
	bseekset (fd, p0);
	return (rc);
}
