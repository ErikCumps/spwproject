/** \file
 * The Steel Panthers World at War report tool.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL V2
 */

#include "spwreport.h"
#include "utils.h"

static void
report_formations (FILE *rf, SPWAW_SNAP_OOB_FRAW *ptr)
{
	DWORD	i;

	if (!ptr) return;

	smart_title (rf, '-', "RAWLINK formations report:\n");

	fprintf (rf, "count: %u\n", ptr->cnt);
	fprintf (rf, "start: %u\n", ptr->start);
	fprintf (rf, "\n");

	if (!ptr->cnt || !ptr->raw || !ptr->ridx) return;

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_SNAP_OOB_FELRAW *p = &(ptr->raw[i]);
		fprintf (rf, "{%4lu} RID=%4.4x, FID=%2.2x, leader=%4.4x <%s>\n", i, p->RID, p->FID, p->leader, p->name);
	}
	fprintf (rf, "\n");

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_RINDEX *r = &(ptr->ridx[i]);
		fprintf (rf, "ridx[%4lu] rid=%4.4x, idx=%4.4x\n", i, r->rid, r->idx);
	}
	fprintf (rf, "\n");
}

static void
report_units (FILE *rf, SPWAW_SNAP_OOB_URAW *ptr)
{
	DWORD	i;

	if (!ptr) return;

	smart_title (rf, '-', "RAWLINK units report:\n");

	fprintf (rf, "count: %u\n", ptr->cnt);
	fprintf (rf, "\n");

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_SNAP_OOB_UELRAW *p = &(ptr->raw[i]);
		fprintf (rf, "{%4lu} RID=%4.4x, FRID=%2.2x, FMID=%2.2x, FSID=%2.2x, leader=%4.4x, crew=%4.4x, loader=%4.4x, target=%4.4x <%s>\n",
			i, p->RID, p->FRID, p->FMID, p->FSID, p->leader, p->crew, p->loader, p->target, p->name);
	}
	fprintf (rf, "\n");

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_RINDEX *r = &(ptr->ridx[i]);
		fprintf (rf, "ridx[%4lu] rid=%4.4x, idx=%4.4x\n", i, r->rid, r->idx);
	}
	fprintf (rf, "\n");
}

static void
report_leaders (FILE *rf, SPWAW_SNAP_OOB_LRAW *ptr)
{
	DWORD	i;

	if (!ptr) return;

	smart_title (rf, '-', "RAWLINK leaders report:\n");

	fprintf (rf, "count: %u\n", ptr->cnt);
	fprintf (rf, "\n");

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_SNAP_OOB_LELRAW *p = &(ptr->raw[i]);
		fprintf (rf, "{%4lu} RID=%4.4x, URID=%4.4x <%s>\n", i, p->RID, p->URID, p->name);
	}
	fprintf (rf, "\n");

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_RINDEX *r = &(ptr->ridx[i]);
		fprintf (rf, "ridx[%4lu] rid=%4.4x, idx=%4.4x\n", i, r->rid, r->idx);
	}
	fprintf (rf, "\n");
}

static void
report_positions (FILE *rf, SPWAW_SNAP_OOB_PRAW *ptr)
{
	DWORD	i;

	if (!ptr) return;

	smart_title (rf, '-', "RAWLINK unit positions report:\n");

	fprintf (rf, "count: %u\n", ptr->cnt);
	fprintf (rf, "\n");

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_SNAP_OOB_PELRAW *p = &(ptr->raw[i]);
		fprintf (rf, "{%4lu} RID=%4.4x, URID=%4.4x\n", i, p->RID, p->URID);
	}
	fprintf (rf, "\n");

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_RINDEX *r = &(ptr->ridx[i]);
		fprintf (rf, "ridx[%4lu] rid=%4.4x, idx=%4.4x\n", i, r->rid, r->idx);
	}
	fprintf (rf, "\n");
}

static void
report_oob (FILE *rf, SPWAW_SNAP_OOB_RAW *ptr, int player)
{
	if (!ptr) return;

	smart_title (rf, '=', "RAWLINK player %u OOB report:\n", player);

	report_formations (rf, &(ptr->formations));
	report_units (rf, &(ptr->units));
	report_leaders (rf, &(ptr->leaders));
	report_positions (rf, &(ptr->positions));

	fprintf (rf, "\n");
}

void
rawlink_report (SPWAW_SNAPSHOT *ptr, FILE *rf)
{
	if (!ptr || !rf) return;

	report_oob	(rf, &(ptr->raw.OOBp1), 1);
	report_oob	(rf, &(ptr->raw.OOBp2), 2);

	return;
}
