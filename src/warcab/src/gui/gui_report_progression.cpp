/** \file
 * The SPWaW war cabinet - GUI - force progression report.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_report_progression.h"
#include "gui_reports_dossier.h"
#include "gui_reports_battle.h"
#include "gui_reports_bturn.h"

// TODO: consider externalizing
#define	LISTMAX	5

GuiRptPrg::GuiRptPrg (QWidget *P)
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

	GUINEW (d.label_exp, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_exp");
	d.label_exp->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label_exp->setWordWrap (true);
	d.label_exp->setFont (*d.font);

	GUINEW (d.label_mor, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_mor");
	d.label_mor->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label_mor->setWordWrap (true);
	d.label_mor->setFont (*d.font);

	GUINEW (d.label_ral, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_ral");
	d.label_ral->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label_ral->setWordWrap (true);
	d.label_ral->setFont (*d.font);

	GUINEW (d.label_inf, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_inf");
	d.label_inf->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label_inf->setWordWrap (true);
	d.label_inf->setFont (*d.font);

	GUINEW (d.label_arm, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_arm");
	d.label_arm->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label_arm->setWordWrap (true);
	d.label_arm->setFont (*d.font);

	GUINEW (d.label_art, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_art");
	d.label_art->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label_art->setWordWrap (true);
	d.label_art->setFont (*d.font);

	GUINEW (d.spacer, QSpacerItem (0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding), ERR_GUI_REPORTS_INIT_FAILED, "spacer");

	d.layout->addWidget (d.label_nodata,	0, 0, 1, 3);
	d.layout->addWidget (d.label_exp,	1, 0, 1, 1);
	d.layout->addWidget (d.label_mor,	1, 1, 1, 1);
	d.layout->addWidget (d.label_ral,	1, 2, 1, 1);
	d.layout->addWidget (d.label_inf,	2, 0, 1, 1);
	d.layout->addWidget (d.label_arm,	2, 1, 1, 1);
	d.layout->addWidget (d.label_art,	2, 2, 1, 1);
	d.layout->addItem   (d.spacer,		3, 0, 1, 3);

	setWidget(d.frame);
	setWidgetResizable (true);

	//setFrameStyle (QFrame::NoFrame);

	if (!connect (this, SIGNAL (cmpcurr(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpcurr(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <cmpcurr> to <dossier:set_cmpcurr>");

	if (!connect (this, SIGNAL (cmpbase(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpbase(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <cmpbase> to <dossier:set_cmpbase>");

	d.pflag = d.cflag = true;

	SET_GUICLS_NOERR;
}

GuiRptPrg::~GuiRptPrg (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
	delete d.model;
}

void
GuiRptPrg::set_parent (GuiRptDsr *parent, bool player)
{
	d.ptype	 = MDLD_TREE_DOSSIER;
	d.pptr.d = parent;
	d.pflag	 = player;
	d.cflag	 = true;

	d.label_exp->show();
	d.label_mor->show();
	d.label_ral->show();
	d.label_inf->show();
	d.label_arm->show();
	d.label_art->show();
}

void
GuiRptPrg::set_parent (GuiRptBtl *parent, bool player, bool core)
{
	d.ptype	 = MDLD_TREE_BATTLE;
	d.pptr.b = parent;
	d.pflag	 = player;
	d.cflag	 = core;

	if (core) {
		d.label_exp->show();
		d.label_mor->show();
		d.label_ral->show();
		d.label_inf->show();
		d.label_arm->show();
		d.label_art->show();
	} else {
		d.label_exp->hide();
		d.label_mor->hide();
		d.label_ral->hide();
		d.label_inf->hide();
		d.label_arm->hide();
		d.label_art->hide();
	}
}

void
GuiRptPrg::set_parent (GuiRptTrn *parent, bool player, bool core)
{
	d.ptype	 = MDLD_TREE_BTURN;
	d.pptr.t = parent;
	d.pflag	 = player;
	d.cflag	 = core;

	d.label_exp->hide();
	d.label_mor->hide();
	d.label_ral->hide();
	d.label_inf->hide();
	d.label_arm->hide();
	d.label_art->hide();
}

void
GuiRptPrg::update (void)
{
	bool		skip;
	MDLD_TREE_ITEM	*pcurr, *pbase;

	DBG_TRACE_FENTER;

	pcurr = pbase = NULL;
	switch (d.ptype) {
		case MDLD_TREE_DOSSIER:
			d.pdata = d.pptr.d ? d.pptr.d->current() : NULL;
			if (d.pdata) {
				if (!d.pdata->data.d || (d.pdata->children.size() <= 1)) d.pdata = NULL;
			}
			break;
		case MDLD_TREE_BATTLE:
			d.pdata = d.pptr.b ? d.pptr.b->current() : NULL;
			if (d.pdata) {
				if (!d.pdata->data.b || (d.pdata->children.size() <= 1)) d.pdata = NULL;
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
				pcurr = d.pdata->clast;
				pbase = d.pdata->cfirst;
				d.model->load (pcurr->data.b, pbase->data.b, d.pflag, true);
				break;
			case MDLD_TREE_BATTLE:
				pcurr = d.pdata->next ? d.pdata->next : d.pdata;
				pbase = d.pdata;
				d.model->load (pcurr->data.b, pbase->data.b, d.pflag, d.cflag);
				break;
			case MDLD_TREE_BTURN:
				pcurr = d.pdata;
				if (d.pdata->prev) {
					pbase = d.pdata->prev;
					d.model->load (pcurr->data.t, pbase->data.t, d.pflag, d.cflag);
				} else {
					pbase = d.pdata->parent;
					d.model->load (pcurr->data.t, pcurr->data.t, d.pflag, d.cflag);
				}
			default:
				break;
		}
	} else {
		pbase = pcurr = NULL;
		d.model->clear();
	}

skip_data_update:
	/* Only emit these signals when widget is visible */
	if (isVisible()) {
		emit cmpcurr (pcurr);
		emit cmpbase (pbase);
	}

	DBG_TRACE_FLEAVE;
}

static inline unsigned long
dlt_color (int dlt)
{
	if (dlt > 0) return (RID_GM_DLT_POS);
	if (dlt < 0) return (RID_GM_DLT_NEG);
	return (RID_GM_DLT_NTR);
}

void
GuiRptPrg::mkshortlist (char *title, MDLR_COLUMN col, bool up, char *buf, unsigned int size)
{
	UtilStrbuf		str(buf, size, true, true);
	char			tbuf[4096];
	UtilStrbuf		tstr(tbuf, sizeof (tbuf), true, true);
	ModelRosterRawData	pdata[LISTMAX];
	ModelRosterRawData	ndata[LISTMAX];
	int			i, pcnt = 0, ncnt = 0;

	str.clear();
	str.printf ("<pre><h3><u>%s:</u></h3><br><br>", title);

	d.model->set_dltsort (true);
	d.model->sort (col, up ? Qt::AscendingOrder : Qt::DescendingOrder);
	pcnt = d.model->rawdata (0, col, pdata, LISTMAX);

	d.model->sort (col, up ? Qt::DescendingOrder : Qt::AscendingOrder);
	ncnt = d.model->rawdata (0, col, ndata, LISTMAX);

	str.printf ("Best progression:<br>");

	tstr.clear();
	for (i=0; i<pcnt; i++) {
		if (SPWDLT_getint (pdata[i].dlt) == 0) break;

		tstr.printf ("<font color=%s>", qPrintable(RES_htmlcolor (dlt_color (SPWDLT_getint (pdata[i].dlt)))));
		tstr.printf ("%3.3s %s %s %s (%+d)",
			pdata[i].uir->snap->strings.uid, pdata[i].uir->snap->data.dname,
			pdata[i].uir->snap->strings.rank, pdata[i].uir->snap->data.lname,
			SPWDLT_getint (pdata[i].dlt));
		tstr.printf ("</font><br>");
	}
	if (!i) {
		str.printf ("none<br>");
	} else {
		str.add (tbuf);
	}

	str.printf ("<br>");

	str.printf ("Worst progression:<br>");

	tstr.clear();
	for (i=0; i<ncnt; i++) {
		tstr.printf ("<font color=%s>", qPrintable(RES_htmlcolor (dlt_color (SPWDLT_getint (ndata[i].dlt)))));
		tstr.printf ("%3.3s %s %s %s (%+d)",
			ndata[i].uir->snap->strings.uid, ndata[i].uir->snap->data.dname,
			ndata[i].uir->snap->strings.rank, ndata[i].uir->snap->data.lname,
			SPWDLT_getint (ndata[i].dlt));
		tstr.printf ("</font><br>");
	}
	str.add (tbuf);
}

void
GuiRptPrg::refresh (void)
{
	bool	nodata;
	char	buf[4096];

	DBG_TRACE_FENTER;

	update();

	nodata = !d.pdata || (d.ptype == MDLD_TREE_BTURN);
	d.label_nodata->setHidden (!nodata);
	d.label_exp->setHidden (nodata);
	d.label_mor->setHidden (nodata);
	d.label_ral->setHidden (nodata);
	d.label_inf->setHidden (nodata);
	d.label_arm->setHidden (nodata);
	d.label_art->setHidden (nodata);
	if (nodata) goto leave;

	mkshortlist ("Experience", MDLR_COLUMN_EXP, true, buf, sizeof (buf));
	d.label_exp->setText (buf);

	mkshortlist ("Morale", MDLR_COLUMN_MOR, true, buf, sizeof (buf));
	d.label_mor->setText (buf);

	mkshortlist ("Rally rating", MDLR_COLUMN_RAL, true, buf, sizeof (buf));
	d.label_ral->setText (buf);

	mkshortlist ("Infantry rating", MDLR_COLUMN_INF, true, buf, sizeof (buf));
	d.label_inf->setText (buf);

	mkshortlist ("Armor rating", MDLR_COLUMN_ARM, true, buf, sizeof (buf));
	d.label_arm->setText (buf);

	mkshortlist ("Artillery rating", MDLR_COLUMN_ART, true, buf, sizeof (buf));
	d.label_art->setText (buf);

leave:
	DBG_TRACE_FLEAVE;
}
