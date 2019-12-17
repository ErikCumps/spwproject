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

static bool
load_uhte_data (UHT_ELEMENT &data, SPWAW_UHT *uht, STRTAB *stab)
{
	if (!uht || !stab) return (false);

	SPWAW_UHTE *uhte = uht_new_element (uht);
	if (!uhte) return (false);

	uhte->idx	 = data.idx	= uhte->idx;
	uhte->lname	 = STRTAB_getstr (stab, data.lname);
	uhte->UID	 = STRTAB_getstr (stab, data.UID);
	uhte->uname	 = STRTAB_getstr (stab, data.uname);
	uhte->rank	 = raw2rank (data.rank);
	uhte->FMID	 = data.FMID;
	uhte->FSID	 = data.FSID;
	uhte->FBI.btlidx = data.FBIi;
	SPWAW_stamp2date (&(data.FBId), &(uhte->FBI.date));
	uhte->FUI	 = data.FUI;
	uhte->LBI.btlidx = data.LBIi;
	SPWAW_stamp2date (&(data.LBId), &(uhte->LBI.date));
	uhte->status	 = data.status;
	uhte->prev	 = (SPWAW_UHTE *)data.prev;
	uhte->next	 = (SPWAW_UHTE *)data.next;

	return (true);
}

SPWAW_ERROR
UHT_load (int fd, SPWAW_UHT *dst)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	STRTAB		*stab = NULL;
	long		pos;
	UHT_HEADER	hdr;
	char		*data = NULL;
	CBIO		cbio;
	SBR		*sbr = NULL;
	UHT_ELEMENT	uhte;

	CNULLARG (dst);
	stab = (STRTAB *)dst->dossier->stab;

	/* Read header */
	pos = bseekget (fd);

	memset (&hdr, 0, sizeof (hdr));
	if (!bread (fd, (char *)&hdr, sizeof (hdr), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(uht hdr)", handle_error);

	/* Read uhte data buffer */
	data = safe_smalloc (char, hdr.size);
	COOMGOTO (data, "uhte data buffer", handle_error);

	cbio.data = data; cbio.size = hdr.size; cbio.comp = &(hdr.comp);
	if (!cbread (fd, cbio, "uhte data")) FAILGOTO (SPWERR_FRFAILED, "cbread(uhte data) failed", handle_error);

	/* Load uhte data from the uhte data buffer*/
	sbr = sbread_init (data, hdr.size);
	if (!sbr) FAILGOTO (SPWERR_FRFAILED, "sbread_init() failed", handle_error);
	for (ULONG i=0; i<hdr.cnt; i++) {
		if (sbread (sbr, (char *)&uhte, sizeof (uhte)) != sizeof (uhte))
			FAILGOTO (SPWERR_FRFAILED, "sbread(uhte data)", handle_error);
		if (!load_uhte_data (uhte, dst, stab))
			FAILGOTO (SPWERR_FRFAILED, "load_uhte_data()", handle_error);
	}

	/* Process loaded uhte data, replacing indexes with pointers */
	for (ULONG i=0; i<dst->cnt; i++) {
		ULONG	idx;

		idx = (ULONG)dst->list[i]->prev;
		if (idx == SPWAW_BADLONGIDX) {
			dst->list[i]->prev = NULL;
		} else {
			dst->list[i]->prev = dst->list[idx];
		}

		idx = (ULONG)dst->list[i]->next;
		if (idx == SPWAW_BADLONGIDX) {
			dst->list[i]->next = NULL;
		} else {
			dst->list[i]->next = dst->list[idx];
		}
	}

	/* Sort */
	uht_sort (dst);

	return (SPWERR_OK);

handle_error:
	if (sbr) sbread_stop (sbr);
	if (data) safe_free (data);
	UHT_clear (dst);
	return (rc);
}
