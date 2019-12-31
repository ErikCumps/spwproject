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

static SPWAW_ERROR
UHT_update_for_rebuild (SPWAW_UHT *uht, SPWAW_BATTLE *b)
{
	SPWAW_ERROR	rc;
	SPWAW_UHT_BINFO	*info;
	SPWAW_BATTLE	*pb;
	USHORT		status;
	char		buf[16];

	CNULLARG (uht); CNULLARG (b);

	SPWAW_BDATE(b->bdate, bdate);
	UHTLOG1 ("\n### UHT_update_for_rebuild: \"%s\"\n", bdate);

	rc = UHT_battle_info (uht, b, &info); ROE ("UHT_battle_info");

	pb = b->prev;
	if (!pb) {
		UHTLOG0 ("No previous battle -> all fresh commissions...\n");
		for (USHORT i=0; i<b->racnt; i++) {
			UHTLOG4 ("RA[%05u] src=%05u dst=%05u rpl=%u | ", i, b->ra[i].src, b->ra[i].dst, b->ra[i].rpl);
			BIRURR rr; rr.b = b; rr.i = i;

			if (rr.b->ra[rr.i].src != SPWAW_BADIDX) {
				UHTLOG0 ("==> warning: DETECTED UNEXPECTED RA SRC\n");
			}

			info->list[i] = uht_commission (uht, rr, uht_detect_status (rr));
		}
	} else {
		UHTLOG0 ("Tracking from previous battle...\n");
		for (USHORT i=0; i<b->racnt; i++) {
			UHTLOG4 ("RA[%05u] src=%05u dst=%05u rpl=%u | ", i, b->ra[i].src, b->ra[i].dst, b->ra[i].rpl);
			BIRURR rr; rr.b = b; rr.i = i;

			if (rr.b->ra[rr.i].src == SPWAW_BADIDX) {
				UHTTRACE0 ("src=NO -> fresh commission\n");
				info->list[i] = uht_commission (uht, rr, uht_detect_status (rr));
				continue;
			}

			BIRURR pr; pr.b = pb; pr.i = b->ra[i].src; pr.u = pr.b->uhtinfo->list[pr.i];

			status = uht_detect_changes (pr, rr, pr.b->ra[pr.i].rpl);
			if (status != UHT_NOSTATUS) {
				SPWAW_BDATE(pr.b->bdate, pbdate); SPWAW_BDATE(rr.b->bdate, bdate);
				uht_status_log (status, buf, sizeof(buf));
				UHTTRACE7 ("src=YES -> (%s) [%s:%05u] %s -> [%s:%05u] %s\n",
					buf,
					pbdate, pr.i, pr.u->lname,
					bdate,  rr.i, rr.b->info_sob->pbir_core.uir[rr.i].snap->data.lname);
				info->list[i] = uht_commission (uht, rr, status);
				uht_link (uht, pr, rr, status);
			} else {
				UHTTRACE0 ("src=YES -> adjust decommission\n");
				info->list[i] = uht_adjust_decommission (uht, pr, rr);
			}
		}

		if (pb) {
			for (USHORT i=0; i<pb->racnt; i++) {
				BIRURR pr; pr.b = pb; pr.i = i; pr.u = pr.b->uhtinfo->list[pr.i];

				if (pr.b->ra[pr.i].dst == SPWAW_BADIDX) {
					SPWAW_BDATE(pr.b->bdate, pbdate);
					UHTLOG2 ("--> detected lost unit [%s:%05u]\n", pbdate, pr.i);
					uht_decommission (uht, pr, pb);
				}
			}
		}
	}

	uht_sort (uht);

	return (SPWERR_OK);
}

SPWAW_ERROR
UHT_rebuild (SPWAW_UHT *uht)
{
	SPWAW_ERROR	rc;
	CNULLARG (uht);

	UHTLOG0 ("Rebuilding UHT!\n");

	rc = UHT_clean (uht); ROE ("UHT_clean()");

	for (USHORT i=0; i<uht->dossier->bcnt; i++) {
		rc = UHT_update_for_rebuild (uht, uht->dossier->blist[i]); ROE ("UHT_update_for_rebuild()");
		UHT_debug_dump (uht->dossier->blist[i]->uhtinfo);
	}

	UHT_debug_dump (uht);

	return (SPWERR_OK);
}
