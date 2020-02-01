/** \file
 * The SPWaW Library - unit history tracking handling.
 *
 * Copyright (C) 2019-2020 Erik Cumps <erik.cumps@gmail.com>
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
	uhte->FBD.btlidx = data.FBDi;
	SPWAW_stamp2date (&(data.FBDd), &(uhte->FBD.date));
	uhte->FUI	 = data.FUI;
	uhte->LBD.btlidx = data.LBDi;
	SPWAW_stamp2date (&(data.LBDd), &(uhte->LBD.date));
	uhte->status	 = data.status;
	uhte->prev	 = (SPWAW_UHTE *)data.prev;
	uhte->next	 = (SPWAW_UHTE *)data.next;

	return (true);
}

static SPWAW_ERROR
UHT_load_uhte_list (int fd, SPWAW_UHT *dst, UHT_HEADER &hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	STRTAB		*stab = NULL;
	char		*data = NULL;
	CBIO		cbio;
	SBR		*sbr = NULL;
	UHT_ELEMENT	uhte;

	stab = (STRTAB *)dst->dossier->stab;

	/* Read uhte data buffer */
	data = safe_smalloc (char, hdr.usize);
	COOMGOTO (data, "uhte data buffer", handle_error);

	cbio.data = data; cbio.size = hdr.usize; cbio.comp = &(hdr.ucomp);
	if (!cbread (fd, cbio, "uhte data")) FAILGOTO (SPWERR_FRFAILED, "cbread(uhte data) failed", handle_error);

	/* Load uhte data from the uhte data buffer*/
	sbr = sbread_init (data, hdr.usize);
	if (!sbr) FAILGOTO (SPWERR_FRFAILED, "sbread_init() failed", handle_error);
	for (ULONG i=0; i<hdr.ucnt; i++) {
		if (sbread (sbr, (char *)&uhte, sizeof (uhte)) != sizeof (uhte))
			FAILGOTO (SPWERR_FRFAILED, "sbread(uhte data)", handle_error);
		if (!load_uhte_data (uhte, dst, stab))
			FAILGOTO (SPWERR_FRFAILED, "load_uhte_data()", handle_error);
	}
	sbread_stop (sbr);
	safe_free (data);

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

static void
load_binfo_data (UHT_ELEMENT_PTR &data, SPWAW_UHTE **uhte, SPWAW_UHT *uht)
{
	if (data.idx == SPWAW_BADLONGIDX) {
		*uhte = NULL;
	} else {
		*uhte = uht->list[data.idx];
	}
}

static SPWAW_ERROR
UHT_load_binfo (int fd, SPWAW_UHT_BINFO *dst, UHT_BATTLE_INFO &hdr, SPWAW_UHT *uht)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	char		*data = NULL;
	CBIO		cbio;
	SBR		*sbr = NULL;
	UHT_ELEMENT_PTR	uep;

	/* Read BINFO data buffer */
	data = safe_smalloc (char, hdr.size);
	COOMGOTO (data, "BINFO data buffer", handle_error);

	cbio.data = data; cbio.size = hdr.size; cbio.comp = &(hdr.comp);
	if (!cbread (fd, cbio, "BINFO data")) FAILGOTO (SPWERR_FRFAILED, "cbread(BINFO data) failed", handle_error);

	/* Load BINFO data from the BINFO data buffer*/
	sbr = sbread_init (data, hdr.size);
	if (!sbr) FAILGOTO (SPWERR_FRFAILED, "sbread_init() failed", handle_error);
	for (USHORT i=0; i<hdr.cnt; i++) {
		if (sbread (sbr, (char *)&uep, sizeof (uep)) != sizeof (uep))
			FAILGOTO (SPWERR_FRFAILED, "sbread(BINFO data)", handle_error);
		load_binfo_data (uep, &(dst->list[i]), uht);
	}

	sbread_stop (sbr);
	safe_free (data);

	return (SPWERR_OK);

handle_error:
	if (sbr) sbread_stop (sbr);
	if (data) safe_free (data);
	return (rc);
}

static SPWAW_ERROR
UHT_load_binfo_list (int fd, SPWAW_UHT *dst, UHT_HEADER &hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		pos;
	unsigned long	isize;
	UHT_BATTLE_INFO	*ihdrs;
	SPWAW_UHT_BINFO *info;

	/* Read BINFO list */
	pos = bseekget (fd);

	isize = hdr.bcnt * sizeof (UHT_BATTLE_INFO);
	ihdrs = safe_nmalloc (UHT_BATTLE_INFO, hdr.bcnt); COOMGOTO (ihdrs, "BINFO list", handle_error);

	if (!bread (fd, (char *)ihdrs, isize, false))
		FAILGOTO (SPWERR_FRFAILED, "bread(BINFO list)", handle_error);

	/* Read BINFO data */
	for (ULONG i=0; i<hdr.bcnt; i++) {
		SPWAW_BATTLE_DATE bd; bd.btlidx = ihdrs[i].bdi; SPWAW_stamp2date (&(ihdrs[i].bdd), &(bd.date));
		SPWAW_BATTLE *b = dossier_find_battle (dst->dossier, &(bd));
		if (!b) FAILGOTO (SPWERR_CORRUPT, "dossier_find_battle()", handle_error);

		rc = UHT_battle_info (dst, b, &info); ERRORGOTO ("UHT_battle_info()", handle_error);
		if (info->cnt != ihdrs[i].cnt) FAILGOTO (SPWERR_CORRUPT, "battle racnt and BINFO list cnt do not match", handle_error);

		bseekset (fd, pos + ihdrs[i].data);
		rc = UHT_load_binfo (fd, info, ihdrs[i], dst); ERRORGOTO ("UHT_load_binfo()", handle_error);
	}

	safe_free (ihdrs);

	return (SPWERR_OK);

handle_error:
	if (ihdrs) safe_free (ihdrs);
	UHT_clear (dst);
	return (rc);
}

SPWAW_ERROR
UHT_load (int fd, SPWAW_UHT *dst)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	STRTAB		*stab = NULL;
	long		pos;
	UHT_HEADER	hdr;

	CNULLARG (dst);
	stab = (STRTAB *)dst->dossier->stab;

	/* Read header */
	pos = bseekget (fd);

	memset (&hdr, 0, sizeof (hdr));
	if (!bread (fd, (char *)&hdr, sizeof (hdr), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(uht hdr)", handle_error);

	/* Load UHTE list */
	rc = UHT_load_uhte_list (fd, dst, hdr); ERRORGOTO ("UHT_load_uhte_list()", handle_error);

	/* Load UHTE_BINFO list */
	rc = UHT_load_binfo_list (fd, dst, hdr); ERRORGOTO ("UHT_load_binfo_list()", handle_error);

	return (SPWERR_OK);

handle_error:
	return (rc);
}
