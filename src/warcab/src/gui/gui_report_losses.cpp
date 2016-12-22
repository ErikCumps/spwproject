/** \file
 * The SPWaW war cabinet - GUI - force losses report.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_report_losses.h"
#include "model/model_roster.h"
#include "gui_reports_dossier.h"
#include "gui_reports_battle.h"
#include "gui_reports_bturn.h"

// TODO: consider externalizing
#define	LISTMAX	5

GuiRptLoss::GuiRptLoss (QWidget *P)
	: QScrollArea (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	GUINEW (d.model, ModelRoster (), ERR_GUI_REPORTS_INIT_FAILED, "data model");

	GUINEW (d.font, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_REPORTS_INIT_FAILED, "font");

	GUINEW (d.frame, QFrame (this), ERR_GUI_REPORTS_INIT_FAILED, "frame");
	GUINEW (d.layout, QGridLayout (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "layout");

	d.layout->setVerticalSpacing (20);

	GUINEW (d.label_nodata, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_nodata");
	d.label_nodata->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label_nodata->setWordWrap (true);
	d.label_nodata->setFont (*d.font);
	d.label_nodata->setText ("No information available yet.");

	GUINEW (d.label_khdr, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_khdr");
	d.label_khdr->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	//d.label_khdr->setWordWrap (true);
	d.label_khdr->setWordWrap (false);
	d.label_khdr->setFont (*d.font);

	GUINEW (d.label_klist, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_klist");
	d.label_klist->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label_klist->setWordWrap (true);
	d.label_klist->setFont (*d.font);

	GUINEW (d.label_ahdr, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_ahdr");
	d.label_ahdr->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	//d.label_ahdr->setWordWrap (true);
	d.label_ahdr->setWordWrap (false);
	d.label_ahdr->setFont (*d.font);

	GUINEW (d.label_alist, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_alist");
	d.label_alist->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label_alist->setWordWrap (true);
	d.label_alist->setFont (*d.font);

	GUINEW (d.label_dhdr, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_dhr");
	d.label_dhdr->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	//d.label_dhdr->setWordWrap (true);
	d.label_dhdr->setWordWrap (false);
	d.label_dhdr->setFont (*d.font);

	GUINEW (d.label_dlist, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_dlist");
	d.label_dlist->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label_dlist->setWordWrap (true);
	d.label_dlist->setFont (*d.font);

	GUINEW (d.spacer, QSpacerItem (0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding), ERR_GUI_REPORTS_INIT_FAILED, "spacer");

	d.layout->addWidget (d.label_nodata,	0, 0, 1, 3);
	d.layout->addWidget (d.label_khdr,	1, 0, 1, 1);
	d.layout->addWidget (d.label_ahdr,	1, 1, 1, 1);
	d.layout->addWidget (d.label_dhdr,	1, 2, 1, 1);
	d.layout->addWidget (d.label_klist,	2, 0, 1, 1);
	d.layout->addWidget (d.label_alist,	2, 1, 1, 1);
	d.layout->addWidget (d.label_dlist,	2, 2, 1, 1);
	d.layout->addItem   (d.spacer,		3, 0, 1, 3);

	d.layout->setColumnStretch (0, 1);
	d.layout->setColumnStretch (1, 1);
	d.layout->setColumnStretch (2, 1);

	setWidget(d.frame);
	setWidgetResizable (true);

	if (!connect (this, SIGNAL (cmpcurr(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpcurr(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <cmpcurr> to <dossier:set_cmpcurr>");

	if (!connect (this, SIGNAL (cmpbase(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpbase(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <cmpbase> to <dossier:set_cmpbase>");

	d.pflag = d.cflag = true;

	SET_GUICLS_NOERR;
}

GuiRptLoss::~GuiRptLoss (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
	delete d.model;
}

void
GuiRptLoss::set_parent (GuiRptDsr *parent, bool player)
{
	d.ptype	 = MDLD_TREE_DOSSIER;
	d.pptr.d = parent;
	d.pflag	 = player;
	d.cflag	 = true;
}

void
GuiRptLoss::set_parent (GuiRptBtl *parent, bool player, bool core)
{
	d.ptype	 = MDLD_TREE_BATTLE;
	d.pptr.b = parent;
	d.pflag	 = player;
	d.cflag	 = core;
}

void
GuiRptLoss::set_parent (GuiRptTrn *parent, bool player, bool core)
{
	d.ptype	 = MDLD_TREE_BTURN;
	d.pptr.t = parent;
	d.pflag	 = player;
	d.cflag	 = core;
}

void
GuiRptLoss::update (void)
{
	bool	skip;

	DBG_TRACE_FENTER;

	d.pcurr = d.pbase = NULL;
	switch (d.ptype) {
		case MDLD_TREE_DOSSIER:
			d.pdata = d.pptr.d ? d.pptr.d->current() : NULL;
			if (d.pdata) {
				if (!d.pdata->data.d || !d.pdata->children.size()) d.pdata = NULL;
			}
			break;
		case MDLD_TREE_BATTLE:
			d.pdata = d.pptr.b ? d.pptr.b->current() : NULL;
			if (d.pdata) {
				if (!d.pdata->data.b || !d.pdata->children.size()) d.pdata = NULL;
			}
			break;
		case MDLD_TREE_BTURN:
			d.pdata = d.pptr.t ? d.pptr.t->current() : NULL;
			if (d.pdata) {
				if (!d.pdata->data.t) d.pdata = NULL;
			}
			break;
		default:
			break;
	}

	skip =  !d.reftrack.changed (d.pdata);
	if (skip) goto skip_data_update;

	DBG_TRACE_UPDATE;

	if (d.pdata) {
		switch (d.ptype) {
			case MDLD_TREE_DOSSIER:
				d.pcurr = d.pdata->clast;
				d.pbase = d.pdata->cfirst;
				d.model->load (d.pcurr->data.b, d.pbase->data.b, d.pflag, true);
				break;
			case MDLD_TREE_BATTLE:
				d.pcurr = d.pbase = d.pdata;
				d.model->load (d.pcurr->data.b, d.pbase->data.b, d.pflag, d.cflag);
				break;
			case MDLD_TREE_BTURN:
				d.pcurr = d.pdata;
				if (d.pdata->prev) {
					d.pbase = d.pdata->prev;
					d.model->load (d.pcurr->data.t, d.pbase->data.t, d.pflag, d.cflag);
				} else {
					d.pbase = d.pdata->parent;
					d.model->load (d.pcurr->data.t, d.pcurr->data.t, d.pflag, d.cflag);
				}
			default:
				break;
		}
	} else {
		d.pbase = d.pcurr = NULL;
		d.model->clear();
	}

skip_data_update:
	/* Only emit these signals when widget is visible */
	if (isVisible()) {
		emit cmpcurr (d.pcurr);
		emit cmpbase (d.pbase);
	}

	DBG_TRACE_FLEAVE;
}

void
GuiRptLoss::list_killed_dossier (char *buf, unsigned int size, int &icnt)
{
	UtilStrbuf		str(buf, size, true, true);
	SPWAW_DOSSIER		*p;
	SPWAW_BATTLE		*b;
	SPWAW_SNAP_OOB_UEL	*up;

	icnt = 0;
	if ((d.pdata == NULL) || ((p = d.pdata->data.d) == NULL)) return;

	str.clear();

	for (int i=0; i<p->ucnt; i++) {
		USHORT	idx, nidx;

		idx = nidx = i; b = p->bfirst;
		while (b) {
			up = b->info_eob->pbir.uir[idx].snap;

			if (!up->data.alive) {
				str.printf ("%3.3s %s, %s %s<br>",
					up->strings.uid, up->data.type, up->strings.rank, up->data.name);
				icnt++;
			}

			if ((nidx = b->ra[idx].dst) == SPWAW_BADIDX) break;
			idx = nidx; b = b->next;
		}
	}
}

void
GuiRptLoss::list_killed (char *buf, unsigned int size, int &icnt)
{
	UtilStrbuf		str(buf, size, true, true);
	char			tbuf[4096];
	UtilStrbuf		tstr(tbuf, sizeof (tbuf), true, true);
	ModelRosterRawData	data[LISTMAX];
	int			idx, i;
	bool			stop;
	bool			lostunit, lostcrew;

	if (d.ptype == MDLD_TREE_DOSSIER) return (list_killed_dossier (buf, size, icnt));

	icnt = 0;

	str.clear();

	d.model->set_dltsort (false);
	d.model->sort (MDLR_COLUMN_STATUS, Qt::DescendingOrder);

	idx = 0; stop = false;
	while (1) {
		int cnt = d.model->rawdata (idx, MDLR_COLUMN_STATUS, data, LISTMAX);
		if (!cnt) break;

		tstr.clear();
		for (i=0; i<cnt; i++) {
			if (SPWDLT_int (data[i].dlt) == SPWAW_UREADY) { stop = true; break; }
			if (!data[i].uir->snap->attr.gen.losses) continue;

			lostunit = !data[i].uir->snap->data.alive;
			lostcrew = data[i].uir->snap->data.crew
				   && (!data[i].uir->snap->data.aunit.up || !data[i].uir->snap->data.aunit.up->data.alive);

			tstr.printf ("%3.3s %s, %s (%s%s%s)<br>",
				data[i].uir->snap->strings.uid, data[i].uir->snap->data.type,
				data[i].uir->snap->strings.rank,
				lostunit?"unit":"",
				(lostunit && lostcrew) ? " and " : "",
				lostcrew?"crew":"");

			icnt += data[i].uir->snap->attr.gen.losses;
		}
		str.add (tbuf);

		if (stop) break;
		idx += cnt;
	}
}

void
GuiRptLoss::list_abandoned_dossier (char *buf, unsigned int size, int &icnt)
{
	UtilStrbuf		str(buf, size, true, true);
	SPWAW_DOSSIER		*p;
	SPWAW_BATTLE		*b;
	SPWAW_SNAP_OOB_UEL	*up;

	icnt = 0;
	if ((d.pdata == NULL) || ((p = d.pdata->data.d) == NULL)) return;

	str.clear();

	for (int i=0; i<p->ucnt; i++) {
		USHORT	idx, nidx;

		idx = nidx = i; b = p->bfirst;
		while (b) {
			up = b->info_eob->pbir.uir[idx].snap;
			if (up->data.aband != SPWAW_ANONE) {
				str.printf ("%3.3s %s, %s %s<br>",
					up->strings.uid, up->data.type, up->strings.rank, up->data.name);
				icnt++;
			}

			if ((nidx = b->ra[idx].dst) == SPWAW_BADIDX) break;
			idx = nidx; b = b->next;
		}
	}
}

void
GuiRptLoss::list_abandoned (char *buf, unsigned int size, int &icnt)
{
	UtilStrbuf		str(buf, size, true, true);
	char			tbuf[4096];
	UtilStrbuf		tstr(tbuf, sizeof (tbuf), true, true);
	ModelRosterRawData	data[LISTMAX];
	int			idx, i;
	bool			stop;

	if (d.ptype == MDLD_TREE_DOSSIER) return (list_abandoned_dossier (buf, size, icnt));

	icnt = 0;

	str.clear();

	d.model->set_dltsort (false);
	d.model->sort (MDLR_COLUMN_ABAND, Qt::AscendingOrder);

	idx = 0; stop = false;
	while (1) {
		int cnt = d.model->rawdata (idx, MDLR_COLUMN_ABAND, data, LISTMAX);
		if (!cnt) break;

		tstr.clear();
		for (i=0; i<cnt; i++) {
			if (SPWDLT_int (data[i].dlt) == SPWAW_ANONE) { stop = true; break; }
//FIXME			if (!SPWDLT_check (data[i].dlt)) continue;
			//if (data[i].uir->snap->data.lost) continue;
			//if (!data[i].uir->snap->data.alive) continue;

			//tstr.printf ("%3.3s %s %s %s<br>",
			//	data[i].uir->snap->strings.uid, data[i].uir->snap->data.type,
			//	data[i].uir->snap->strings.rank, data[i].uir->snap->data.name);
			tstr.printf ("%3.3s %s, %s<br>",
				data[i].uir->snap->strings.uid, data[i].uir->snap->data.type,
				data[i].uir->snap->strings.rank);

			icnt++;
		}
		str.add (tbuf);

		if (stop) break;
		idx += cnt;
	}
}

typedef struct s_DMGRPT {
	SPWAW_BATTLE	*ptr;
	USHORT		idx;
	unsigned long	dmg;
} DMGRPT;

static int
dmgrpt_cmp (const void *a, const void *b)
{
	DMGRPT	*va;
	DMGRPT	*vb;
	int	cmp;

	DEVASSERT (a != NULL); DEVASSERT (b != NULL);
	va = (DMGRPT *)a; vb = (DMGRPT *)b;

	if ((va->ptr == NULL) || (vb->ptr == NULL)) return (-1);

	cmp = ((va->dmg > vb->dmg) ? -1 : ((va->dmg < vb->dmg) ? 1 : 0));

	if (cmp == 0) {
		USHORT usa = va->ptr->info_sob->pbir.uir[va->idx].snap->data.uidx;
		USHORT usb = vb->ptr->info_sob->pbir.uir[vb->idx].snap->data.uidx;
		cmp = ((usa < usb) ? -1 : ((usa > usb) ? 1 : 0));
	}

	return (cmp);
}

void
GuiRptLoss::list_damaged_dossier (char *buf, unsigned int size, int &icnt)
{
	UtilStrbuf		str(buf, size, true, true);
	SPWAW_DOSSIER		*p;
	DMGRPT			*rpt;
	SPWAW_BATTLE		*b;
	SPWAW_SNAP_OOB_UEL	*up;

	icnt = 0;
	if ((d.pdata == NULL) || ((p = d.pdata->data.d) == NULL)) return;

	str.clear();

	SL_SAFE_CALLOC (rpt, p->ucnt, sizeof (DMGRPT));
	for (int i=0; i<p->ucnt; i++) {
		USHORT		idx, nidx;
		unsigned long	dmg;

		idx = nidx = i; b = p->bfirst; dmg = 0;
		while (b) {
			up = b->info_eob->pbir.uir[idx].snap;

			if (up->data.damage && up->data.alive && (up->data.aband == SPWAW_ANONE)) {
				dmg += up->data.damage;
			}

			if ((nidx = b->ra[idx].dst) == SPWAW_BADIDX) break;
			idx = nidx; b = b->next;
		}
		if (!dmg) continue;

		rpt[i].ptr = b;
		rpt[i].idx = idx;
		rpt[i].dmg = dmg;
	}

	qsort (rpt, p->ucnt, sizeof (DMGRPT), dmgrpt_cmp);

	for (int i=0; i<p->ucnt; i++) {
		if (rpt[i].dmg == 0) continue;

		up = rpt[i].ptr->info_sob->pbir.uir[rpt[i].idx].snap;

		str.printf ("%3.3s %s, %s %s (%ld)<br>",
			up->strings.uid, up->data.type, up->strings.rank, up->data.name, rpt[i].dmg);

		icnt++;
	}

	SL_SAFE_FREE (rpt);
}

void
GuiRptLoss::list_damaged (char *buf, unsigned int size, int &icnt)
{
	UtilStrbuf		str(buf, size, true, true);
	char			tbuf[4096];
	UtilStrbuf		tstr(tbuf, sizeof (tbuf), true, true);
	ModelRosterRawData	data[LISTMAX];
	int			idx, i;
	bool			stop;

	if (d.ptype == MDLD_TREE_DOSSIER) return (list_damaged_dossier (buf, size, icnt));

	icnt = 0;

	str.clear();

	d.model->set_dltsort (true);
	d.model->sort (MDLR_COLUMN_DMG, Qt::AscendingOrder);

	idx = 0; stop = false;
	while (1) {
		int cnt = d.model->rawdata (idx, MDLR_COLUMN_DMG, data, LISTMAX);
		if (!cnt) break;

		tstr.clear();
		for (i=0; i<cnt; i++) {
			if (SPWDLT_int (data[i].dlt) <= 0) { stop = true; break; }
			if (data[i].uir->snap->data.aband != SPWAW_ANONE) continue;
			if (!data[i].uir->snap->data.alive) continue;

			//tstr.printf ("%3.3s %s %s %s (%d)<br>",
			//	data[i].uir->snap->strings.uid, data[i].uir->snap->data.type,
			//	data[i].uir->snap->strings.rank, data[i].uir->snap->data.name,
			//	SPWDLT_int (data[i].dlt));
			tstr.printf ("%3.3s %s, %s (%d)<br>",
				data[i].uir->snap->strings.uid, data[i].uir->snap->data.type,
				data[i].uir->snap->strings.rank,
				SPWDLT_int (data[i].dlt));

			icnt++;
		}
		str.add (tbuf);

		if (stop) break;
		idx += cnt;
	}
}

void
GuiRptLoss::refresh (void)
{
	bool		nodata;
	char		buf[16384];
	UtilStrbuf	str(buf, sizeof (buf), true, true);
	int		cnt;
	int		mw[2];

	DBG_TRACE_FENTER;

	update();

	nodata = !d.pdata;
	d.label_nodata->setHidden (!nodata);
	d.label_klist->setHidden (nodata);
	d.label_alist->setHidden (nodata);
	d.label_dlist->setHidden (nodata);
	if (nodata) goto leave;

	list_killed (buf, sizeof (buf), cnt);
	d.label_klist->setText (buf);

	str.clear();
	str.printf ("<h3><u>Destroyed units: %d</u></h3>", cnt);
	d.label_khdr->setText (buf);

	list_abandoned (buf, sizeof (buf), cnt);
	d.label_alist->setText (buf);

	str.clear();
	str.printf ("<h3><u>Abandoned units: %d</u></h3>", cnt);
	d.label_ahdr->setText (buf);

	list_damaged (buf, sizeof (buf), cnt);
	d.label_dlist->setText (buf);

	str.clear();
	str.printf ("<h3><u>Damaged units: %d</u></h3>", cnt);
	d.label_dhdr->setText (buf);

	mw[0] = 0;
	mw[1] = d.label_klist->minimumSizeHint().width(); if (mw[1] > mw[0]) mw[0] = mw[1];
	mw[1] = d.label_alist->minimumSizeHint().width(); if (mw[1] > mw[0]) mw[0] = mw[1];
	mw[1] = d.label_dlist->minimumSizeHint().width(); if (mw[1] > mw[0]) mw[0] = mw[1];
	for (int i=0; i<3; i++) d.layout->setColumnMinimumWidth (i, mw[0]);

leave:
	DBG_TRACE_FLEAVE;
}
