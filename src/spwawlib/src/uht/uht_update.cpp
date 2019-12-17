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
#include "common/internal.h"

SPWAW_ERROR
UHT_update (SPWAW_UHT *uht, SPWAW_BATTLE *b)
{
	SPWAW_BATTLE	*pb, *nb;
	USHORT		status;
	char		buf[16];

	CNULLARG (uht); CNULLARG (b);

	SPWAW_BDATE(b->bdate, bdate);
	UHTLOG1 ("UHT_update: \"%s\"\n", bdate);

	pb = b->prev; nb = b->next;

	for (USHORT i=0; i<b->racnt; i++) {
		UHTLOG4 ("RA[%05u] src=%05u dst=%05u rpl=%u | ", i, b->ra[i].src, b->ra[i].dst, b->ra[i].rpl);
		BIRURR rr; rr.b = b; rr.i = i;

		if ((rr.b->ra[rr.i].src == SPWAW_BADIDX) && (rr.b->ra[rr.i].dst == SPWAW_BADIDX)) {
			UHTTRACE0 ("src/dst = NO/NO ");

			UHTTRACE0 ("-> fresh commission\n");
			uht_commission (uht, rr);

			continue;
		}

		if ((rr.b->ra[rr.i].src != SPWAW_BADIDX) && (rr.b->ra[rr.i].dst == SPWAW_BADIDX)) {
			UHTTRACE0 ("src/dst = YES/NO ");
			BIRURR pr; pr.b = pb; pr.i = b->ra[i].src;

			status = uht_detect_changes (pr, rr, pr.b->ra[pr.i].rpl);
			if (status != UHT_NOSTATUS) {
				SPWAW_BDATE(pr.b->bdate, pbdate); SPWAW_BDATE(rr.b->bdate, bdate);
				uht_status_log (status, buf, sizeof(buf));
				UHTTRACE7 ("-> (%s) [%s:%05u] %s -> [%s:%05u] %s\n",
					buf,
					pbdate, pr.i, pr.b->info_sob->pbir_core.uir[pr.i].snap->data.lname,
					bdate, rr.i, rr.b->info_sob->pbir_core.uir[rr.i].snap->data.lname);
				uht_commission (uht, rr);
				INIT_BIRURR_FILTER (f); uht_set_filter (pr, f);
				dossier_search_back (pr, pr, f);
				uht_link (uht, pr, rr, status);
			} else {
				UHTTRACE0 ("-> adjust decommission\n");

				INIT_BIRURR_FILTER (f); uht_set_filter (pr, f);
				dossier_search_back (pr, pr, f);
				uht_adjust_decommission (uht, pr, rr);
			}

			continue;
		}

		if ((rr.b->ra[rr.i].src == SPWAW_BADIDX) && (rr.b->ra[rr.i].dst != SPWAW_BADIDX)) {
			UHTTRACE0 ("src/dst = NO/YES ");
			BIRURR nr; nr.b = nb; nr.i = b->ra[i].dst;

			status = uht_detect_changes (rr, nr, rr.b->ra[rr.i].rpl);
			if (status != UHT_NOSTATUS) {
				SPWAW_BDATE(rr.b->bdate, bdate); SPWAW_BDATE(nr.b->bdate, nbdate);
				uht_status_log (status, buf, sizeof(buf));
				UHTTRACE7 ("-> (%s) [%s:%05u] %s -> [%s:%05u] %s\n",
					buf,
					bdate, rr.i, rr.b->info_sob->pbir_core.uir[rr.i].snap->data.lname,
					nbdate, nr.i, nr.b->info_sob->pbir_core.uir[nr.i].snap->data.lname);
				uht_commission (uht, rr);
				uht_link (uht, rr, nr, status);
			} else {
				UHTTRACE0 ("-> adjust commission\n");
				rr.b = rr.b->next; rr.i = rr.b->ra[rr.i].dst;
				nr.b = b; nr.i = i;
				uht_adjust_commission (uht, rr, nr);
			}

			continue;
		}

		UHTTRACE0 ("src/dst = YES/YES - ");

		BIRURR pr; pr.b = pb; pr.i = b->ra[i].src;
		USHORT pstatus = uht_detect_changes (pr, rr, pr.b->ra[pr.i].rpl);

		BIRURR nr; nr.b = nb; nr.i = b->ra[i].dst;
		USHORT nstatus = uht_detect_changes (rr, nr, rr.b->ra[rr.i].rpl);

		if ((pstatus == UHT_NOSTATUS) && (nstatus == UHT_NOSTATUS)) {
			UHTTRACE0 ("pchange/nchange = NO/NO\n");
			UHTTRACE0 ("-> existing unit - NOP\n");
		} else if ((pstatus != UHT_NOSTATUS) && (nstatus == UHT_NOSTATUS)) {
			UHTTRACE0 ("pchange/nchange = YES/NO\n");

			UHTTRACE0 ("-> adjust commission (dst)\n");
			BIRURR cr; cr.b = rr.b->next; cr.i = rr.b->ra[rr.i].dst;
			uht_adjust_commission (uht, cr, rr);

			UHTTRACE0 ("-> adjust src link\n");
			INIT_BIRURR_FILTER (f); uht_set_filter (pr, f);
			dossier_search_back (pr, pr, f);
			uht_link (uht, pr, rr, pstatus);
		} else if ((pstatus == UHT_NOSTATUS) && (nstatus != UHT_NOSTATUS)) {
			UHTTRACE0 ("pchange/nchange = NO/YES\n");

			UHTTRACE0 ("-> adjust src link\n");
			INIT_BIRURR_FILTER (f); uht_set_filter (pr, f);
			dossier_search_back (pr, pr, f);
			uht_link (uht, pr, nr, nstatus);
		} else {
			UHTTRACE0 ("pchange/nchange = YES/YES\n");

			INIT_BIRURR_FILTER (f); uht_set_filter (pr, f);
			dossier_search_back (pr, pr, f);

			USHORT status = uht_detect_changes (pr, nr, pr.b->ra[pr.i].rpl);
			if (status == UHT_NOSTATUS) {
				UHTTRACE0 ("-> break chain\n");
				uht_split_commission (uht, pr, nr);
			}

			UHTTRACE0 ("-> fresh commission\n");
			uht_commission (uht, rr);

			{
				SPWAW_BDATE(pr.b->bdate, prbdate); SPWAW_BDATE(rr.b->bdate, bdate);
				uht_status_log (pstatus, buf, sizeof(buf));
				UHTTRACE7 ("-> (%s) [%s:%05u] %s -> [%s:%05u] %s\n",
					buf,
					prbdate, pr.i, pr.b->info_sob->pbir_core.uir[pr.i].snap->data.lname,
					bdate, rr.i, rr.b->info_sob->pbir_core.uir[rr.i].snap->data.lname);
			}
			uht_link (uht, pr, rr, pstatus);

			{
				SPWAW_BDATE(rr.b->bdate, bdate); SPWAW_BDATE(nr.b->bdate, nbdate);
				uht_status_log (nstatus, buf, sizeof(buf));
				UHTTRACE7 ("-> (%s) [%s:%05u] %s -> [%s:%05u] %s\n",
					buf,
					bdate, rr.i, rr.b->info_sob->pbir_core.uir[rr.i].snap->data.lname,
					nbdate, nr.i, nr.b->info_sob->pbir_core.uir[nr.i].snap->data.lname);
			}
			uht_link (uht, rr, nr, nstatus);
		}
	}

	if (pb) {
		for (USHORT i=0; i<pb->racnt; i++) {
			BIRURR pr; pr.b = pb; pr.i = i;

			if (pr.b->ra[pr.i].dst == SPWAW_BADIDX) {
				SPWAW_BDATE(pr.b->bdate, bdate);
				UHTLOG2 ("--> detected lost unit [%s:%05u]\n", bdate, pr.i);
				INIT_BIRURR_FILTER (f); uht_set_filter (pr, f);
				dossier_search_back (pr, pr, f);
				uht_decommission (uht, pr, pb);
			}
		}
	}

	if (nb) {
		for (USHORT i=0; i<nb->racnt; i++) {
			BIRURR nr; nr.b = nb; nr.i = i;

			if (nr.b->ra[nr.i].src == SPWAW_BADIDX) {
				SPWAW_BDATE(nr.b->bdate, nbdate);
				UHTLOG2 ("==> warning: LOST TRACK OF UNIT [%s:%05u]\n", nbdate, nr.i);
			}
		}
	}

	uht_sort (uht);

	UHT_debug_dump (uht);

	return (SPWERR_OK);
}
