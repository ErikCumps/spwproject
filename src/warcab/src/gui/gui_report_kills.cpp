/** \file
 * The SPWaW war cabinet - GUI - force kills report.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_report_kills.h"
#include "model/model_roster.h"
#include "gui_reports_dossier.h"
#include "gui_reports_battle.h"
#include "gui_reports_bturn.h"

// TODO: consider externalizing
#define	LISTMAX	5

GuiRptKill::GuiRptKill (QWidget *P)
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
	d.label_khdr->setWordWrap (true);
	d.label_khdr->setFont (*d.font);

	GUINEW (d.label_klist, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_klist");
	d.label_klist->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label_klist->setWordWrap (true);
	d.label_klist->setFont (*d.font);

	GUINEW (d.spacer, QSpacerItem (0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding), ERR_GUI_REPORTS_INIT_FAILED, "spacer");

	d.layout->addWidget (d.label_nodata,	0, 0, 1, 1);
	d.layout->addWidget (d.label_khdr,	1, 0, 1, 1);
	d.layout->addWidget (d.label_klist,	2, 0, 1, 1);
	d.layout->addItem   (d.spacer,		3, 0, 1, 1);

	setWidget(d.frame);
	setWidgetResizable (true);

	if (!connect (this, SIGNAL (cmpcurr(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpcurr(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <cmpcurr> to <dossier:set_cmpcurr>");

	if (!connect (this, SIGNAL (cmpbase(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpbase(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <cmpbase> to <dossier:set_cmpbase>");

	d.pflag = d.cflag = true;

	SET_GUICLS_NOERR;
}

GuiRptKill::~GuiRptKill (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
	delete d.model;
}

void
GuiRptKill::set_parent (GuiRptDsr *parent, bool player)
{
	d.ptype	 = MDLD_TREE_DOSSIER;
	d.pptr.d = parent;
	d.pflag	 = player;
	d.cflag	 = true;
}

void
GuiRptKill::set_parent (GuiRptBtl *parent, bool player, bool core)
{
	d.ptype	 = MDLD_TREE_BATTLE;
	d.pptr.b = parent;
	d.pflag	 = player;
	d.cflag	 = core;
}

void
GuiRptKill::set_parent (GuiRptTrn *parent, bool player, bool core)
{
	d.ptype	 = MDLD_TREE_BTURN;
	d.pptr.t = parent;
	d.pflag	 = player;
	d.cflag	 = core;
}

bool
GuiRptKill::update (bool forced)
{
	bool		skip;

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

	skip  = !d.reftrack.changed (d.pdata);
	skip &= !forced;
	if (skip) goto skip_data_update;

	DBG_TRACE_UPDATE;

	if (d.pdata) {
		switch (d.ptype) {
			case MDLD_TREE_DOSSIER:
				d.pcurr = d.pdata->clast;
				d.pbase = d.pdata->cfirst;
				d.model->load (d.pdata->data.d, CFG_full_history());
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
				break;
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

	return (skip);
}

void
GuiRptKill::list_kills (char *buf, unsigned int size, int &kcnt, bool dossier)
{
	UtilStrbuf		str(buf, size, true, true);
	char			tbuf[4096];
	UtilStrbuf		tstr(tbuf, sizeof (tbuf), true, true);
	ModelRosterRawData	data[LISTMAX];
	int			idx, i;
	bool			stop;

	kcnt = 0;

	str.clear();

	d.model->set_dltsort (!dossier);
	d.model->sort (MDLR_COLUMN_KILL, Qt::AscendingOrder);

	idx = 0; stop = false;
	while (1) {
		int cnt = d.model->rawdata (idx, MDLR_COLUMN_KILL, data, LISTMAX);
		if (!cnt) break;

		tstr.clear();
		for (i=0; i<cnt; i++) {
			if (SPWDLT_getint (data[i].dlt) <= 0) { stop = true; break; }

			bool dc = data[i].uhte?SPWAW_UHT_is_decommissioned (data[i].uhte):false;

			if (dc) {
				tstr.printf ("<font color=%s>", qPrintable(RES_htmlcolor (RID_GM_DLT_INA)));
				tstr.printf ("<i>");
			}
			tstr.printf ("%3.3s %s %s %s (%d)",
				data[i].uir->snap->strings.uid, data[i].uir->snap->data.dname,
				data[i].uir->snap->strings.rank, data[i].uir->snap->data.lname,
				SPWDLT_getint (data[i].dlt));
			if (dc) {
				tstr.printf (" <small>decommissioned</small></i>");
				tstr.printf ("</font>");
			}
			tstr.printf ("<br>");

			kcnt += SPWDLT_getint (data[i].dlt);
		}
		str.add (tbuf);

		if (stop) break;
		idx += cnt;
	}
}

void
GuiRptKill::refresh (bool forced)
{
	bool		skip;
	bool		nodata;
	char		buf[32768];
	UtilStrbuf	str(buf, sizeof (buf), true, true);
	int		kcnt;

	DBG_TRACE_FENTER;

	skip = update(forced);
	if (skip) goto leave;

	nodata = !d.pdata;
	d.label_nodata->setHidden (!nodata);
	d.label_khdr->setHidden (nodata);
	d.label_klist->setHidden (nodata);
	if (nodata) goto leave;

	list_kills (buf, sizeof (buf), kcnt, d.ptype == MDLD_TREE_DOSSIER);
	d.label_klist->setText (buf);

	str.clear();
	str.printf ("<h3><u>Recorded kills: %d</u></h3>", kcnt);
	d.label_khdr->setText (buf);

leave:
	DBG_TRACE_FLEAVE;
}
