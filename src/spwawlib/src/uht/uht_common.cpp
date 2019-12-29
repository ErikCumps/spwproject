/** \file
 * The SPWaW Library - unit history tracking handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "uht/uht_common.h"
#include "uht/uht.h"
#include "common/internal.h"

#define	UHT_LISTCNT_GROW	128

SPWAW_UHTE *
uht_new_element (SPWAW_UHT *uht)
{
	SPWAW_ERROR	rc;
	SPWAW_UHTE	*uhte = NULL;
	unsigned int	idx;

	if (!uht) return (NULL);

	uhte = safe_malloc (SPWAW_UHTE); COOMGOTO (uhte, "SPWAW_UHTE", handle_error);
	uhte->uht = uht;

	if (uht->cnt >= uht->len) {
		unsigned int nlen = uht->len + UHT_LISTCNT_GROW;

		SPWAW_UHTE **nlist = (SPWAW_UHTE **)safe_realloc (uht->list, nlen * sizeof (SPWAW_UHTE *));
		COOMGOTO (nlist, "SPWAW_UHTE list", handle_error);

		SPWAW_UHTE **nsmap = (SPWAW_UHTE **)safe_realloc (uht->smap, nlen * sizeof (SPWAW_UHTE *));
		COOMGOTO (nsmap, "SPWAW_UHTE smap", handle_error);

		uht->list = nlist;
		uht->smap = nsmap;
		uht->len = nlen;
	}

	idx = uht->cnt++;
	uhte->idx = idx;
	uht->list[idx] = uht->smap[idx] = uhte;

	return (uhte);

handle_error:
	if (uhte) safe_free (uhte);
	return (NULL);
}

static SPWAW_UHTE *
find_uhte (SPWAW_UHT *uht, BIRURR &rr)
{
	SPWAW_UHTE	*uptr = NULL;

	if (!uht) return (NULL);

	for (unsigned int i=0; i<uht->cnt; i++) {
		if (SPWAW_bdate_cmp (&(uht->list[i]->FBI), &(rr.b->bdate)) != 0) continue;
		if (uht->list[i]->FUI != rr.i) continue;

		uptr = uht->list[i];
		break;
	}

	if (!uptr) { UHTLOG0 ("warning: failed to find UHTE!\n"); }

	return (uptr);
}

SPWAW_UHTE *
uht_commission (SPWAW_UHT *uht, BIRURR &rr)
{
	SPWAW_DOSSIER_UIR	*uir = NULL;
	SPWAW_UHTE		*uhte = NULL;

	if (!uht) return (NULL);
	if (rr.i >= rr.b->info_sob->pbir_core.ucnt) return (NULL);

	uir = &(rr.b->info_sob->pbir_core.uir[rr.i]);

	SPWAW_BDATE(rr.b->bdate, bdate);
	UHTLOG4 ("| uht_commission [%s:%05u] %s: %s\n",
		bdate, rr.i, uir->snap->strings.uid, uir->snap->data.lname);

	uhte = uht_new_element (uht);
	if (!uhte) return (NULL);

	uhte->uht	= uht;

	uhte->lname	= STRTAB_add ((STRTAB*)uht->dossier->stab, uir->snap->data.lname);
	uhte->UID	= STRTAB_add ((STRTAB*)uht->dossier->stab, uir->snap->strings.uid);
	uhte->uname	= STRTAB_add ((STRTAB*)uht->dossier->stab, uir->snap->data.uname);
	uhte->rank	= uir->snap->data.rank;
	uhte->FMID	= uir->snap->data.FMID;
	uhte->FSID	= uir->snap->data.FSID;

	uhte->FBI	= rr.b->bdate;
	uhte->FUI	= rr.i;

	uhte->LBI	= rr.b->bdate;

	uhte->status	= UHT_NOSTATUS;

	uhte->prev	= NULL;
	uhte->next	= NULL;

	return (uhte);
}

SPWAW_UHTE *
uht_split_commission (SPWAW_UHT *uht, BIRURR &rr, BIRURR &nrr)
{
	SPWAW_DOSSIER_UIR	*nuir = NULL;
	SPWAW_UHTE		*uptr = NULL;
	SPWAW_UHTE		*uhte = NULL;

	if (!uht) return (NULL);
	if (nrr.i >= nrr.b->info_sob->pbir_core.ucnt) return (NULL);

	nuir = &(nrr.b->info_sob->pbir_core.uir[nrr.i]);

	SPWAW_BDATE(rr.b->bdate, bdate); SPWAW_BDATE(nrr.b->bdate, nbdate);
	UHTLOG6 ("| uht_split_commission [%s:%05u] %s: %s from [%s:%05u]\n",
		nbdate, nrr.i, nuir->snap->strings.uid, nuir->snap->data.lname, bdate, rr.i);

	uptr = find_uhte (uht, rr);

	uhte = uht_new_element (uht);
	if (!uhte) return (NULL);

	uhte->uht	= uht;

	uhte->lname	= STRTAB_add ((STRTAB*)uht->dossier->stab, nuir->snap->data.lname);
	uhte->UID	= STRTAB_add ((STRTAB*)uht->dossier->stab, nuir->snap->strings.uid);
	uhte->uname	= STRTAB_add ((STRTAB*)uht->dossier->stab, nuir->snap->data.uname);
	uhte->rank	= nuir->snap->data.rank;
	uhte->FMID	= nuir->snap->data.FMID;
	uhte->FSID	= nuir->snap->data.FSID;

	uhte->FBI	= nrr.b->bdate;
	uhte->FUI	= nrr.i;

	if (uptr != NULL) {
		uhte->LBI	= uptr->LBI;
		uhte->status	= uptr->status;
		uhte->prev	= uptr->prev;
		uhte->next	= uptr->next;
	} else {
		uhte->LBI	= nrr.b->bdate;
		uhte->status	= UHT_NOSTATUS;
		uhte->prev	= NULL;
		uhte->next	= NULL;
	}

	return (uhte);
}

SPWAW_UHTE *
uht_adjust_commission (SPWAW_UHT *uht, BIRURR &rr, BIRURR &nrr)
{
	SPWAW_DOSSIER_UIR	*nuir = NULL;
	SPWAW_UHTE		*uptr = NULL;

	if (!uht) return (NULL);
	if (nrr.i >= nrr.b->info_sob->pbir_core.ucnt) return (NULL);

	nuir = &(nrr.b->info_sob->pbir_core.uir[nrr.i]);

	SPWAW_BDATE(rr.b->bdate, bdate); SPWAW_BDATE(nrr.b->bdate, nbdate);
	UHTLOG6 ("| uht_adjust_commission [%s:%05u] -> [%s:%05u] %s: %s\n",
		 bdate, rr.i, nbdate, nrr.i, nuir->snap->strings.uid, nuir->snap->data.lname);

	uptr = find_uhte (uht, rr);
	if (uptr) {
		if (uptr->lname) STRTAB_del ((STRTAB*)uht->dossier->stab, uptr->lname);
		if (uptr->UID) STRTAB_del ((STRTAB*)uht->dossier->stab, uptr->UID);
		if (uptr->uname) STRTAB_del ((STRTAB*)uht->dossier->stab, uptr->uname);

		uptr->lname	= STRTAB_add ((STRTAB*)uht->dossier->stab, nuir->snap->data.lname);
		uptr->UID	= STRTAB_add ((STRTAB*)uht->dossier->stab, nuir->snap->strings.uid);
		uptr->uname	= STRTAB_add ((STRTAB*)uht->dossier->stab, nuir->snap->data.uname);
		uptr->rank	= nuir->snap->data.rank;
		uptr->FMID	= nuir->snap->data.FMID;
		uptr->FSID	= nuir->snap->data.FSID;

		uptr->FBI	= nrr.b->bdate;
		uptr->FUI	= nrr.i;
	}

	return (uptr);
}

SPWAW_UHTE *
uht_decommission (SPWAW_UHT *uht, BIRURR &rr, SPWAW_BATTLE *db)
{
	SPWAW_DOSSIER_UIR	*uir = NULL;
	SPWAW_UHTE		*uptr = NULL;

	if (!uht || !db) return (NULL);
	if (rr.i >= rr.b->info_sob->pbir_core.ucnt) return (NULL);

	uir = &(rr.b->info_sob->pbir_core.uir[rr.i]);

	SPWAW_BDATE(rr.b->bdate, bdate); SPWAW_BDATE(db->bdate, dbdate);
	UHTLOG3 ("| uht_decommission [%s:%05u] %s\n",
		 bdate, rr.i, dbdate);

	uptr = find_uhte (uht, rr);
	if (uptr) {
		uptr->LBI = db->bdate;

		if (uptr->next) {
			uptr->next->prev = false;
			uptr->next	 = NULL;
		}
	}

	return (uptr);
}

SPWAW_UHTE *
uht_adjust_decommission (SPWAW_UHT *uht, BIRURR &rr, BIRURR &nrr)
{
	SPWAW_DOSSIER_UIR	*nuir = NULL;
	SPWAW_UHTE		*uptr = NULL;

	if (!uht) return (NULL);
	if (nrr.i >= nrr.b->info_sob->pbir_core.ucnt) return (NULL);

	nuir = &(nrr.b->info_sob->pbir_core.uir[nrr.i]);

	SPWAW_BDATE(rr.b->bdate, bdate); SPWAW_BDATE(nrr.b->bdate, nbdate);
	UHTLOG4 ("| uht_adjust_decommission [%s:%05u] -> [%s:%05u]\n",
		 bdate, rr.i, nbdate, nrr.i);

	uptr = find_uhte (uht, rr);
	if (uptr) {
		uptr->LBI = nrr.b->bdate;
	}

	return (uptr);
}

void
uht_link (SPWAW_UHT *uht, BIRURR &frr, BIRURR &trr, USHORT status)
{
	char		buf[16];
	SPWAW_UHTE	*fptr = NULL;
	SPWAW_UHTE	*tptr = NULL;

	if (!uht) return;

	SPWAW_BDATE(frr.b->bdate, fbdate); SPWAW_BDATE(trr.b->bdate, tbdate);
	uht_status_log (status, buf, sizeof(buf));
	UHTLOG5 ("| uht_link [%s:%05u] -> [%s:%05u] (%s)\n",
		fbdate, frr.i, tbdate, trr.i, buf);

	fptr = find_uhte (uht, frr);
	tptr = find_uhte (uht, trr);

	UHTTRACE2 ("| -> fptr=0x%8.8x, tptr=0x%8.8x\n", fptr, tptr);

	if ((fptr != NULL) && (tptr != NULL)) {
		UHTTRACE1 ("| -> next=0x%8.8x\n", fptr->next);
		if (!fptr->next || (fptr->next == tptr)) {
			SPWAW_BATTLE *b = dossier_find_battle (uht->dossier, &(tptr->FBI));
			fptr->LBI = b->prev->bdate;
			fptr->next = tptr;
			fptr->next->prev = fptr;
			fptr->next->status = status;
		} else {
			UHTLOG0 ("! warning: re-linking is not allowed!\n");

		}
	} else {
		UHTLOG0 ("| warning: can not link unknown UHTE!\n");
	}
}

static int
sort_uht (const void *a, const void *b)
{
	SPWAW_UHTE	*fa = *((SPWAW_UHTE **)a);
	SPWAW_UHTE	*fb = *((SPWAW_UHTE **)b);

	SPWAW_BATTLE_DATE *cba = &(fa->FBI);
	SPWAW_BATTLE_DATE *cbb = &(fb->FBI);

	int fma = fa->FMID;
	int fmb = fb->FMID;

	int fsa = fa->FSID;
	int fsb = fb->FSID;

	int cbcmp = SPWAW_bdate_cmp (cba, cbb);

	return ((cbcmp==0)?((fma==fmb)?((fsa==fsb)?0:((fsa<fsb)?-1:+1)):((fma<fmb)?-1:+1)):cbcmp);
}

void
uht_sort (SPWAW_UHT *uht)
{
	if (!uht || (uht->cnt <= 1)) return;

	qsort (uht->smap, uht->cnt, sizeof (SPWAW_UHTE *), sort_uht);
}

void
uht_status_log (USHORT status, char *buf, int len)
{
	if (!buf || !len) return;
	memset (buf, 0, len);

	snprintf (buf, len - 1, "%c%c%c%c%c%c",
		(status&UHT_RENAMED)?'r':'_',
		(status&UHT_REPLACED)?'R':'_',
		(status&UHT_REASSIGNED)?'A':'_',
		(status&UHT_UPGRADED)?'U':'_',
		(status&UHT_PROMOTED)?'P':'_',
		(status&UHT_DEMOTED)?'D':'_');
}

USHORT
uht_detect_changes (BIRURR &orr, BIRURR &nrr, bool rpl)
{
	SPWAW_DOSSIER_UIR	*ouir, *nuir;
	USHORT			status = UHT_NOSTATUS;

	ouir = &(orr.b->info_sob->pbir_core.uir[orr.i]);
	nuir = &(nrr.b->info_sob->pbir_core.uir[nrr.i]);

	if (strcmp (ouir->snap->data.lname, nuir->snap->data.lname) != 0) {
		if (rpl) {
			status |= UHT_REPLACED;
		} else {
			status |= UHT_RENAMED;
		}
	}

	if (strcmp (ouir->snap->strings.uid, nuir->snap->strings.uid) != 0) {
		status |= UHT_REASSIGNED;
	}

	if (strcmp (ouir->snap->data.uname, nuir->snap->data.uname) != 0) {
		status |= UHT_UPGRADED;
	}

	if (ouir->snap->data.rank != nuir->snap->data.rank) {
		if (ouir->snap->data.rank > nuir->snap->data.rank) {
			status |= UHT_DEMOTED;
		} else {
			status |= UHT_PROMOTED;
		}
	}

	return (status);
}

void
uht_set_filter (BIRURR &rr, BIRURR_FILTER &f)
{
	SPWAW_DOSSIER_UIR	*uir;

	uir = &(rr.b->info_sob->pbir_core.uir[rr.i]);

	f.lname	= uir->snap->data.lname;
	f.UID	= uir->snap->strings.uid;
	f.uname	= uir->snap->data.uname;
	f.rank	= &uir->snap->data.rank;
}
