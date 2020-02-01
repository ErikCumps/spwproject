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
	data.FBDi	= uhte->FBD.btlidx;
	SPWAW_date2stamp (&(uhte->FBD.date), &(data.FBDd));
	data.FUI	= uhte->FUI;
	data.LBDi	= uhte->LBD.btlidx;
	SPWAW_date2stamp (&(uhte->LBD.date), &(data.LBDd));
	data.status	= uhte->status;
	data.prev	= uhte->prev?uhte->prev->idx:SPWAW_BADLONGIDX;
	data.next	= uhte->next?uhte->next->idx:SPWAW_BADLONGIDX;
}

static SPWAW_ERROR
UHT_save_uhte_list (SPWAW_UHT *src, int fd, bool compress, UHT_HEADER &hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	STRTAB		*stab = NULL;
	long		p0;
	SBW		*sbw = NULL;
	UHT_ELEMENT	uhte;
	char		*data = NULL;
	long		size;
	CBIO		cbio;

	stab = (STRTAB *)src->dossier->stab;
	p0 = bseekget (fd);

	/* Collect uhte data in a single buffer, replacing pointers by indexes */
	sbw = sbwrite_init (NULL, 0);
	if (!sbw) FAILGOTO (SPWERR_FWFAILED, "sbwrite_init() failed", handle_error);
	for (ULONG i=0; i<hdr.ucnt; i++) {
		prep_uhte_data (src->list[i], uhte, stab);
		if (sbwrite (sbw, (char *)&uhte, sizeof (uhte)) != sizeof (uhte))
			FAILGOTO (SPWERR_FWFAILED, "sbwrite(uhte data)", handle_error);
	}
	sbwrite_stop (sbw, &data, &size); sbw = NULL;

	/* Update header and write uhte data buffer */
	hdr.usize = size;

	cbio.data = data; cbio.size = size; cbio.comp = &(hdr.ucomp);
	if (!cbwrite (fd, cbio, "uhte data", compress)) FAILGOTO (SPWERR_FWFAILED, "cbwrite(uhte data)", handle_error);
	safe_free (data);

	return (SPWERR_OK);

handle_error:
	if (data) safe_free (data);
	if (sbw) sbwrite_stop (sbw);
	bseekset (fd, p0);
	return (rc);
}

static void
prep_binfo_data (SPWAW_UHTE *uhte, UHT_ELEMENT_PTR &data)
{
	memset (&data, 0, sizeof(data));

	data.idx	= uhte?uhte->idx:SPWAW_BADLONGIDX;
}

static SPWAW_ERROR
UHT_save_binfo (SPWAW_UHT_BINFO *info, int fd, bool compress, UHT_BATTLE_INFO &hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		p0;
	SBW		*sbw = NULL;
	UHT_ELEMENT_PTR	uep;
	char		*data = NULL;
	long		size;
	CBIO		cbio;

	p0 = bseekget (fd);

	/* Collect BINFO data in a single buffer, replacing pointers by indexes */
	sbw = sbwrite_init (NULL, 0);
	if (!sbw) FAILGOTO (SPWERR_FWFAILED, "sbwrite_init() failed", handle_error);
	for (ULONG i=0; i<hdr.cnt; i++) {
		prep_binfo_data (info->list[i], uep);
		if (sbwrite (sbw, (char *)&uep, sizeof (uep)) != sizeof (uep))
			FAILGOTO (SPWERR_FWFAILED, "sbwrite(BINFO data)", handle_error);
	}
	sbwrite_stop (sbw, &data, &size); sbw = NULL;

	/* Update header and write BINFO data buffer */
	hdr.size = size;

	cbio.data = data; cbio.size = size; cbio.comp = &(hdr.comp);
	if (!cbwrite (fd, cbio, "uhte pointer data", compress)) FAILGOTO (SPWERR_FWFAILED, "cbwrite(BINFO data)", handle_error);
        safe_free (data);

	return (SPWERR_OK);

handle_error:
	if (data) safe_free (data);
	if (sbw) sbwrite_stop (sbw);
	bseekset (fd, p0);
	return (rc);
}

static void
prep_binfo_data (SPWAW_UHT_BINFO *info, UHT_BATTLE_INFO &data, ULONG pos)
{
	memset (&data, 0, sizeof(data));

	data.bdi	= info->bdate.btlidx;
	SPWAW_date2stamp (&(info->bdate.date), &(data.bdd));
	data.cnt	= info->cnt;
	data.data	= pos;
}

static SPWAW_ERROR
UHT_save_binfo_list (SPWAW_UHT *src, int fd, bool compress, UHT_HEADER &hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		p0, p1;
	unsigned long	isize;
	UHT_BATTLE_INFO	*ihdrs;

	p0 = bseekget (fd);

	/* Allocate storage for BINFO headers */
	isize = sizeof (UHT_BATTLE_INFO) * hdr.bcnt;
	ihdrs = safe_nmalloc (UHT_BATTLE_INFO, hdr.bcnt); COOMGOTO (ihdrs, "BINFO list", handle_error);
	bseekmove (fd, isize);

	/* Save BINFO data and update BINFO headers */
	for (ULONG i=0; i<hdr.bcnt; i++) {
		prep_binfo_data (src->blist.info[i], ihdrs[i], bseekget(fd) - p0);
		rc = UHT_save_binfo (src->blist.info[i], fd, compress, ihdrs[i]); ERRORGOTO ("UHT_save_binfo()", handle_error);
	}

	/* Write BINFO headers */
	p1 = bseekget (fd); bseekset (fd, p0);
	if (!bwrite (fd, (char *)ihdrs, isize)) FAILGOTO (SPWERR_FWFAILED, "bwrite(BINFO list) failed", handle_error);
	safe_free (ihdrs);
	bseekset (fd, p1);

	return (SPWERR_OK);

handle_error:
	if (ihdrs) safe_free (ihdrs);
	bseekset (fd, p0);
	return (rc);
}


SPWAW_ERROR
UHT_save (SPWAW_UHT *src, int fd, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	STRTAB		*stab = NULL;
	UHT_HEADER	hdr;
	long		p0, p1;

	CNULLARG (src);
	stab = (STRTAB *)src->dossier->stab;

	p0 = bseekget (fd);
	bseekmove (fd, sizeof (hdr));

	/* Prepare header */
	memset (&hdr, 0, sizeof (hdr));
	memcpy (hdr.magic, UHT_MAGIC, strlen (UHT_MAGIC));
	hdr.version = UHT_VERSION;

	/* Save UHTE list */
	hdr.ucnt  = src->cnt;
	hdr.udata = bseekget (fd) - p0;
	rc = UHT_save_uhte_list (src, fd, compress, hdr); ERRORGOTO ("UHT_save_uhte_list()", handle_error);

	/* Save UHTE_BINFO list */
	hdr.bcnt  = src->blist.cnt;
	hdr.bdata = bseekget (fd) - p0;
	rc = UHT_save_binfo_list (src, fd, compress, hdr); ERRORGOTO ("UHT_save_binfo_list()", handle_error);

	/* Write header */
	p1 = bseekget (fd); bseekset (fd, p0);
	if (!bwrite (fd, (char *)&hdr, sizeof (hdr))) FAILGOTO (SPWERR_FWFAILED, "bwrite(UHT hdr) failed", handle_error);
	bseekset (fd, p1);

	return (SPWERR_OK);

handle_error:
	bseekset (fd, p0);
	return (rc);
}
