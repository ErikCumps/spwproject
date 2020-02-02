/** \file
 * The SPWaW war cabinet - utility functions - unit history tracking list jobs.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"
#include "util_uht_list_job.h"

#define	JOBHDRBUFSZ	256
#define	JOBLSTBUFSZ	32768

void
UHT_list_job (UHT_LIST_JOB &job)
{
	char			hbuf[JOBHDRBUFSZ];
	UtilStrbuf		hsb(hbuf, sizeof(hbuf), true, true);
	char			lbuf[JOBLSTBUFSZ];
	UtilStrbuf		lsb(lbuf, sizeof(lbuf), true, true);
	char			*hdrpre, *hdrpst;
	char			*lstpre, *lstpst;
	SPWAW_UHT_LIST_JOB	uljob;
	unsigned int		ucnt = 0;
	bool			empty = false;

	hdrpre = job.out.hdrpre ? job.out.hdrpre : ""; hdrpst = job.out.hdrpst ? job.out.hdrpst : "";
	lstpre = job.out.lstpre ? job.out.lstpre : ""; lstpst = job.out.lstpst ? job.out.lstpst : "";

	switch (job.type) {
		case UHT_LIST_DOSSIER:
			if (!job.in.d.dossier) return;
			uljob.type	= SPWAW_UHT_LIST_CHAIN;
			uljob.src.uht	= &(job.in.d.dossier->uht);
			uljob.reversed	= job.how.reversed;
			break;
		case UHT_LIST_BATTLE:
			if (!job.in.b.battle || !job.in.b.battle->uhtinfo || !job.in.b.battle->next) return;
			uljob.type	  = SPWAW_UHT_LIST_BINFO;
			uljob.src.bid.bid = job.in.b.battle->uhtinfo;
			uljob.src.bid.nbd = &(job.in.b.battle->next->bdate);
			uljob.reversed	  = false;
			break;
		default:
			return;
	}

	uljob.status		= job.how.status;
	uljob.allow_decomm	= job.how.allow_decomm;
	uljob.data_cb		= job.dext.data;
	uljob.extra		= job.dext.extra;
	uljob.dst		= NULL;

	SPWAW_UHT_list_job (uljob);

	lsb.printf ("%s", lstpre);
	if (uljob.dst->cnt) {
		if (job.sort.sorted) {
			SPWAW_UHT_list_job_sort (uljob, job.sort.type, job.sort.ascending, job.sort.cmp);
		}
		if (!job.dext.counted) {
			ucnt = uljob.dst->cnt;
		}
		for (unsigned int i=0; i<uljob.dst->cnt; i++) {
			if (job.out.visitor) {
				SPWAW_DOSSIER_UIR *uir = SPWAW_UHT_lookup_SOBUIR (uljob.dst->smap[i]->uhte, &(uljob.dst->smap[i]->uhte->FBD), true);
				job.out.visitor (uljob.dst->smap[i], uir, &lsb);
			} else {
				if (uljob.dst->smap[i]->data) {
					lsb.printf ("%s\n", ((UtilStrbuf *)(uljob.dst->smap[i]->data))->data());
				}
			}
			if (uljob.dst->smap[i]->data) {
				delete (UtilStrbuf *)(uljob.dst->smap[i]->data);
			}
			if (job.dext.counted) {
				ucnt += uljob.dst->smap[i]->count;
			}
		}
		hsb.printf ("%s%s: %u%s", hdrpre, job.what, ucnt, hdrpst);
	} else {
		empty = true;
		lsb.printf ("-");
		hsb.printf ("%s%s: none%s", hdrpre, job.what, hdrpst);
	}
	if (!empty || !job.out.skip_if_empty) {
		lsb.printf ("%s", lstpst);
		if (job.out.strbuf) {
			job.out.strbuf->add (hbuf);
			job.out.strbuf->add ("\n");
			job.out.strbuf->add (lbuf);
		} else {
			if (job.out.hdr) job.out.hdr->setText (hbuf);
			if (job.out.lst) job.out.lst->setText (lbuf);
		}
	}

	SPWAW_UHT_list_free (&(uljob.dst));
}
