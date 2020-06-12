/** \file
 * The SPWaW war cabinet - GUI - dossier report.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_reports_dossier.h"

GuiRptDsr::GuiRptDsr (QWidget *P)
	: QTabWidget (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Fix the QTabWidget background color */
	setPalette (GUI_APP->palette());
	setAutoFillBackground (true);

	GUINEW (d.disabled_label, QLabel ("No report available yet."), ERR_GUI_REPORTS_INIT_FAILED, "disabled_label");
	d.disabled_label->setAlignment (Qt::AlignHCenter|Qt::AlignVCenter);

	GUINEW (d.overview, GuiRptDsrOvr (), ERR_GUI_REPORTS_INIT_FAILED, "overview page");
	GUIERR (d.overview, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.cnd, GuiRptCnd (), ERR_GUI_REPORTS_INIT_FAILED, "core force status page");
	GUIERR (d.cnd, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.prg, GuiRptPrg (), ERR_GUI_REPORTS_INIT_FAILED, "core force progress page");
	GUIERR (d.prg, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.kill, GuiRptKill (), ERR_GUI_REPORTS_INIT_FAILED, "kill");
	GUIERR (d.kill, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.loss, GuiRptLoss (), ERR_GUI_REPORTS_INIT_FAILED, "loss");
	GUIERR (d.loss, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.oob, GuiRptOob (), ERR_GUI_REPORTS_INIT_FAILED, "core force oob page");
	GUIERR (d.oob, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.rst, GuiRptRst (), ERR_GUI_REPORTS_INIT_FAILED, "core force roster page");
	GUIERR (d.rst, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.hst, GuiRptHst (), ERR_GUI_REPORTS_INIT_FAILED, "core force history page");
	GUIERR (d.hst, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.mmas, GuiRptMMAS (), ERR_GUI_REPORTS_INIT_FAILED, "core force mmas page");
	GUIERR (d.mmas, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.graphs, GuiRptDsrPlt (), ERR_GUI_REPORTS_INIT_FAILED, "graphs page");
	GUIERR (d.graphs, ERR_GUI_REPORTS_INIT_FAILED);

	if (!connect (this, SIGNAL (currentChanged(int)), SLOT (selectedCurrentIndex(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <currentChanged> to <selectedCurrentIndex>");

	set_enabled (false);

	d.overview->set_parent	(this);
	d.cnd->set_parent	(this, true);
	d.prg->set_parent	(this, true);
	d.kill->set_parent	(this, true);
	d.loss->set_parent	(this, true);
	d.oob->set_parent	(this, true);
	d.rst->set_parent	(this, true);
	d.hst->set_parent	(this, true);
	d.mmas->set_parent	(this, true);
	d.graphs->set_parent	(this);

	d.last_index = currentIndex();
	tabBar()->setTabTextColor (currentIndex(), *RES_color (RID_TAB_FG_SEL));

	SET_GUICLS_NOERR;
}

GuiRptDsr::~GuiRptDsr (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.overview;
	delete d.cnd;
	delete d.prg;
	delete d.kill;
	delete d.loss;
	delete d.oob;
	delete d.rst;
	delete d.hst;
	delete d.mmas;
	delete d.graphs;
}

void
GuiRptDsr::enable (bool flag)
{
	if (flag == d.enabled) return;

	set_enabled (flag);
}

void
GuiRptDsr::set_enabled (bool flag)
{
	bool	hidden = isHidden();

	d.enabled = flag;

	if (!hidden) hide();
	if (flag) {
		removeTab (indexOf (d.disabled_label));
		addTab (d.overview, "Overview");
		if (d.item->dossier_type == SPWAW_CAMPAIGN_DOSSIER) {
			addTab (d.cnd, "Status");
			addTab (d.prg, "Progress");
			addTab (d.kill, "Kills");
			addTab (d.loss, "Losses");
			addTab (d.oob, "OOB");
			addTab (d.rst, "Roster");
			addTab (d.hst, "History");
			addTab (d.mmas, "MMAS");
			addTab (d.graphs, "Battle graphs");
		}

		setCurrentIndex (d.last_index);
		tabBar()->setTabTextColor (currentIndex(), *RES_color (RID_TAB_FG_SEL));
	} else {
		removeTab (indexOf (d.overview));
		removeTab (indexOf (d.cnd));
		removeTab (indexOf (d.prg));
		removeTab (indexOf (d.kill));
		removeTab (indexOf (d.loss));
		removeTab (indexOf (d.oob));
		removeTab (indexOf (d.rst));
		removeTab (indexOf (d.hst));
		removeTab (indexOf (d.mmas));
		removeTab (indexOf (d.graphs));
		addTab (d.disabled_label, "Overview");
	}
	if (!hidden) show();
}

void
GuiRptDsr::select (MDLD_TREE_ITEM *item)
{
	int	bcnt;
	bool	skip;

	d.item = item;
	bcnt = ((d.item != NULL) && (d.item->data.d != NULL)) ? d.item->data.d->bcnt : 0;

	skip = !d.reftrack.changed (d.item);
	if (skip) return;

	enable ((d.item != NULL) && bcnt);
	refresh(false);
}

void
GuiRptDsr::refresh (bool forced)
{
	DBG_TRACE_FENTER;

	d.overview->refresh(forced);
	d.cnd->refresh(forced);
	d.prg->refresh(forced);
	d.kill->refresh(forced);
	d.loss->refresh(forced);
	d.oob->refresh();	//FIXME?
	d.rst->refresh(forced);
	d.hst->refresh(forced);
	d.mmas->refresh();	//FIXME?
	d.graphs->refresh();	//FIXME?

	DBG_TRACE_FLEAVE;
}

MDLD_TREE_ITEM *
GuiRptDsr::current (void)
{
	return (d.item);
}

void
GuiRptDsr::will_delete (MDLD_TREE_ITEM *item)
{
	if (!item) return;

	select (NULL);
}

void
GuiRptDsr::selectedCurrentIndex (int index)
{
	int	i;

	refresh(false);

	for (i=0; i<count(); i++)
		tabBar()->setTabTextColor (i, *((i!=index) ? RES_color (RID_TAB_FG_DEFAULT): RES_color (RID_TAB_FG_SEL)));

	d.last_index = index;
}
