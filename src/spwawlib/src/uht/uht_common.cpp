/** \file
 * The SPWaW Library - unit history tracking handling.
 *
 * Copyright (C) 2019-2020 Erik Cumps <erik.cumps@gmail.com>
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
	if (!uht) return (NULL);
	if (rr.u) return (rr.u);

	for (unsigned int i=0; i<uht->cnt; i++) {
		if (SPWAW_bdate_cmp (&(uht->list[i]->FBI), &(rr.b->bdate)) != 0) continue;
		if (uht->list[i]->FUI != rr.i) continue;

		rr.u = uht->list[i];
		break;
	}

	if (!rr.u) { UHTLOG0 ("warning: failed to find UHTE!\n"); }

	return (rr.u);
}

SPWAW_UHTE *
uht_commission (SPWAW_UHT *uht, BIRURR &rr, USHORT status)
{
	SPWAW_DOSSIER_UIR	*suir = NULL;
	SPWAW_DOSSIER_UIR	*euir = NULL;
	char			buf[16];

	rr.u = NULL;
	if (!uht) return (NULL);
	if (rr.i >= rr.b->info_sob->pbir_core.ucnt) return (NULL);

	suir = &(rr.b->info_sob->pbir_core.uir[rr.i]);
	euir = &(rr.b->info_eob->pbir_core.uir[rr.i]);

	SPWAW_BDATE(rr.b->bdate, bdate, true);
	uht_status_log (status, buf, sizeof(buf));
	UHTLOG6 ("| uht_commission [%s:%05u] %s: %s, status=0x%4.4x (%s)\n",
		bdate, rr.i, suir->snap->strings.uid, suir->snap->data.lname, status, buf);

	rr.u = rr.u = uht_new_element (uht);
	if (!rr.u) return (NULL);

	rr.u->uht	= uht;

	rr.u->lname	= STRTAB_add ((STRTAB*)uht->dossier->stab, suir->snap->data.lname);
	rr.u->UID	= STRTAB_add ((STRTAB*)uht->dossier->stab, suir->snap->strings.uid);
	rr.u->uname	= STRTAB_add ((STRTAB*)uht->dossier->stab, suir->snap->data.uname);
	rr.u->rank	= suir->snap->data.rank;
	rr.u->FMID	= suir->snap->data.FMID;
	rr.u->FSID	= suir->snap->data.FSID;

	rr.u->FBI	= rr.b->bdate;
	rr.u->FUI	= rr.i;

	rr.u->LBI	= rr.b->bdate;

	rr.u->status	= status;

	rr.u->prev	= NULL;
	rr.u->next	= NULL;

	if (status & UHT_DAMAGED) {
		rr.u->v_damage = euir->snap->data.damage;
	}

	return (rr.u);
}

SPWAW_UHTE *
uht_split_commission (SPWAW_UHT *uht, BIRURR &rr, BIRURR &nrr)
{
	SPWAW_DOSSIER_UIR	*nuir = NULL;

	nrr.u = NULL;
	if (!uht) return (NULL);
	if (nrr.i >= nrr.b->info_sob->pbir_core.ucnt) return (NULL);

	nuir = &(nrr.b->info_sob->pbir_core.uir[nrr.i]);

	SPWAW_BDATE(rr.b->bdate, bdate, true); SPWAW_BDATE(nrr.b->bdate, nbdate, true);
	UHTLOG6 ("| uht_split_commission [%s:%05u] %s: %s from [%s:%05u]\n",
		nbdate, nrr.i, nuir->snap->strings.uid, nuir->snap->data.lname, bdate, rr.i);

	nrr.u = uht_new_element (uht);
	if (!nrr.u) return (NULL);

	nrr.u->uht	= uht;

	nrr.u->lname	= STRTAB_add ((STRTAB*)uht->dossier->stab, nuir->snap->data.lname);
	nrr.u->UID	= STRTAB_add ((STRTAB*)uht->dossier->stab, nuir->snap->strings.uid);
	nrr.u->uname	= STRTAB_add ((STRTAB*)uht->dossier->stab, nuir->snap->data.uname);
	nrr.u->rank	= nuir->snap->data.rank;
	nrr.u->FMID	= nuir->snap->data.FMID;
	nrr.u->FSID	= nuir->snap->data.FSID;

	nrr.u->FBI	= nrr.b->bdate;
	nrr.u->FUI	= nrr.i;

	find_uhte (uht, rr);
	if (rr.u != NULL) {
		nrr.u->LBI	= rr.u->LBI;
		nrr.u->status	= rr.u->status;
		nrr.u->prev	= rr.u->prev;
		nrr.u->next	= rr.u->next;
	} else {
		nrr.u->LBI	= nrr.b->bdate;
		nrr.u->status	= UHT_NOSTATUS;
		nrr.u->prev	= NULL;
		nrr.u->next	= NULL;
	}

	return (nrr.u);
}

SPWAW_UHTE *
uht_adjust_commission (SPWAW_UHT *uht, BIRURR &rr, BIRURR &nrr)
{
	SPWAW_DOSSIER_UIR	*nuir = NULL;

	nrr.u = NULL;
	if (!uht) return (NULL);
	if (nrr.i >= nrr.b->info_sob->pbir_core.ucnt) return (NULL);

	nuir = &(nrr.b->info_sob->pbir_core.uir[nrr.i]);

	SPWAW_BDATE(rr.b->bdate, bdate, true); SPWAW_BDATE(nrr.b->bdate, nbdate, true);
	UHTLOG6 ("| uht_adjust_commission [%s:%05u] -> [%s:%05u] %s: %s\n",
		 bdate, rr.i, nbdate, nrr.i, nuir->snap->strings.uid, nuir->snap->data.lname);

	nrr.u = find_uhte (uht, rr);
	if (nrr.u) {
		if (nrr.u->lname) STRTAB_del ((STRTAB*)uht->dossier->stab, nrr.u->lname);
		if (nrr.u->UID) STRTAB_del ((STRTAB*)uht->dossier->stab, nrr.u->UID);
		if (nrr.u->uname) STRTAB_del ((STRTAB*)uht->dossier->stab, nrr.u->uname);

		nrr.u->lname	= STRTAB_add ((STRTAB*)uht->dossier->stab, nuir->snap->data.lname);
		nrr.u->UID	= STRTAB_add ((STRTAB*)uht->dossier->stab, nuir->snap->strings.uid);
		nrr.u->uname	= STRTAB_add ((STRTAB*)uht->dossier->stab, nuir->snap->data.uname);
		nrr.u->rank	= nuir->snap->data.rank;
		nrr.u->FMID	= nuir->snap->data.FMID;
		nrr.u->FSID	= nuir->snap->data.FSID;

		nrr.u->FBI	= nrr.b->bdate;
		nrr.u->FUI	= nrr.i;
	}

	return (nrr.u);
}

SPWAW_UHTE *
uht_decommission (SPWAW_UHT *uht, BIRURR &rr, SPWAW_BATTLE *db)
{
	SPWAW_DOSSIER_UIR	*uir = NULL;

	rr.u = NULL;
	if (!uht || !db) return (NULL);
	if (rr.i >= rr.b->info_sob->pbir_core.ucnt) return (NULL);

	uir = &(rr.b->info_sob->pbir_core.uir[rr.i]);

	SPWAW_BDATE(rr.b->bdate, bdate, true); SPWAW_BDATE(db->bdate, dbdate, true);
	UHTLOG3 ("| uht_decommission [%s:%05u] %s\n",
		 bdate, rr.i, dbdate);

	find_uhte (uht, rr);
	if (rr.u) {
		rr.u->LBI = db->bdate;

		if (rr.u->next) {
			rr.u->next->prev = NULL;
			rr.u->next	 = NULL;
		}
	}

	return (rr.u);
}

SPWAW_UHTE *
uht_adjust_decommission (SPWAW_UHT *uht, BIRURR &rr, BIRURR &nrr)
{
	SPWAW_DOSSIER_UIR	*nuir = NULL;

	nrr.u = NULL;
	if (!uht) return (NULL);
	if (nrr.i >= nrr.b->info_sob->pbir_core.ucnt) return (NULL);

	nuir = &(nrr.b->info_sob->pbir_core.uir[nrr.i]);

	SPWAW_BDATE(rr.b->bdate, bdate, true); SPWAW_BDATE(nrr.b->bdate, nbdate, true);
	UHTLOG4 ("| uht_adjust_decommission [%s:%05u] -> [%s:%05u]\n",
		 bdate, rr.i, nbdate, nrr.i);

	nrr.u = find_uhte (uht, rr);
	if (nrr.u) {
		nrr.u->LBI = nrr.b->bdate;
	}

	return (nrr.u);
}

void
uht_link (SPWAW_UHT *uht, BIRURR &frr, BIRURR &trr, USHORT status)
{
	char		buf[16];
	SPWAW_UHTE	*fptr = NULL;
	SPWAW_UHTE	*tptr = NULL;

	if (!uht) return;

	SPWAW_BDATE(frr.b->bdate, fbdate, true); SPWAW_BDATE(trr.b->bdate, tbdate, true);
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
			// Fix target uhte status (it may have been updated)
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

	/* Count initial UHTE */
	uht->icnt = 0;
	for (unsigned int i=0; i<uht->cnt; i++) {
		if (SPWAW_UHT_is_initial (uht->smap[i])) uht->icnt++;
	}
}

void
uht_status_log (USHORT status, char *buf, int len)
{
	if (!buf || !len) return;
	memset (buf, 0, len);

	snprintf (buf, len - 1, "%c%c%c%c%c%c%c%c%c",
		(status & UHT_RENAMED	) ? 'N' : '_',
		(status & UHT_REPLACED	) ? 'R' : '_',
		(status & UHT_REASSIGNED) ? 'A' : '_',
		(status & UHT_UPGRADED	) ? 'U' : '_',
		(status & UHT_PROMOTED	) ? 'P' : '_',
		(status & UHT_DEMOTED	) ? 'D' : '_',
		(status & UHT_DAMAGED	) ? 'd' : '_',
		(status & UHT_ABANDONED	) ? 'a' : '_',
		(status & UHT_DESTROYED	) ? 'x' : '_'
		);
}

USHORT
uht_detect_status (BIRURR &rr)
{
	SPWAW_DOSSIER_UIR	*uir;
	USHORT			status = UHT_NOSTATUS;

	uir = &(rr.b->info_eob->pbir_core.uir[rr.i]);

	if (uir->snap->data.damage && uir->snap->data.alive && (uir->snap->data.aband == SPWAW_ANONE)) {
		status |= UHT_DAMAGED;
	}

	if ((uir->snap->data.aband != SPWAW_ANONE) && uir->snap->data.alive) {
		status |= UHT_ABANDONED;
	}

	if (!uir->snap->data.alive) {
		status |= UHT_DESTROYED;
	}

	return (status);
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

	status |= uht_detect_status (nrr);

	return (status);
}
