/** \file
 * The SPWaW war cabinet - GUI - battle turn report.
 *
 * Copyright (C) 2005-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_reports_bturn.h"

GuiRptTrn::GuiRptTrn (QWidget *P)
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

	GUINEW (d.overview, GuiRptTrnOvr (), ERR_GUI_REPORTS_INIT_FAILED, "overview page");
	GUIERR (d.overview, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.force_core, GuiRptTrnFrc (), ERR_GUI_REPORTS_INIT_FAILED, "core force page");
	GUIERR (d.force_core, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.force_spt, GuiRptTrnFrc (), ERR_GUI_REPORTS_INIT_FAILED, "support force page");
	GUIERR (d.force_spt, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.force_opp, GuiRptTrnFrc (), ERR_GUI_REPORTS_INIT_FAILED, "opponent force page");
	GUIERR (d.force_opp, ERR_GUI_REPORTS_INIT_FAILED);

#if	EXPERIMENTAL
	GUINEW (d.compare, GuiRptTrnCmp (), ERR_GUI_REPORTS_INIT_FAILED, "comparison page");
	GUIERR (d.compare, ERR_GUI_REPORTS_INIT_FAILED);
#endif	/* EXPERIMENTAL */

	GUINEW (d.stratmap, GuiRptTrnSMap (), ERR_GUI_REPORTS_INIT_FAILED, "strategic map page");
	GUIERR (d.stratmap, ERR_GUI_REPORTS_INIT_FAILED);

	if (!connect (this, SIGNAL (currentChanged(int)), SLOT (selectedCurrentIndex(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <currentChanged> to <selectedCurrentIndex>");

	set_enabled (false);

	d.overview->set_parent (this);
	d.force_core->set_parent (this, true, true);
	d.force_spt->set_parent (this, true, false);
	d.force_opp->set_parent (this, false, false);
#if	EXPERIMENTAL
	d.compare->set_parent (this);
#endif	/* EXPERIMENTAL */
	d.stratmap->set_parent (this);

	d.last_index = currentIndex();
	tabBar()->setTabTextColor (currentIndex(), *RES_color (RID_TAB_FG_SEL));

	SET_GUICLS_NOERR;
}

GuiRptTrn::~GuiRptTrn (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.overview;
	delete d.force_core;
	delete d.force_spt;
	delete d.force_opp;
#if	EXPERIMENTAL
	delete d.compare;
#endif	/* EXPERIMENTAL */
	delete d.stratmap;
}

void
GuiRptTrn::enable (bool flag)
{
	if (flag == d.enabled) return;

	set_enabled (flag);
}

void
GuiRptTrn::set_enabled (bool flag)
{
	bool	hidden = isHidden();

	if (!d.item) flag = false;
	d.enabled = flag;

	if (!hidden) hide();
	if (flag) {
		char	buf[256];

		memset (buf, 0, sizeof (buf));

		removeTab (indexOf (d.disabled_label));

		addTab (d.overview, "Overview");
		if (d.item->campaign) {
			addTab (d.force_core, "Core force");
			addTab (d.force_spt, "Support force");
		} else {
			addTab (d.force_spt, "Support force");
		}
		addTab (d.force_opp, "Opponent force");
#if	EXPERIMENTAL
		addTab (d.compare, "Comparisons");
#endif	/* EXPERIMENTAL */
		addTab (d.stratmap, "Strategic map");

		setCurrentIndex (d.last_index);
		tabBar()->setTabTextColor (currentIndex(), *RES_color (RID_TAB_FG_SEL));
	} else {
		removeTab (indexOf (d.overview));
		removeTab (indexOf (d.force_core));
		removeTab (indexOf (d.force_spt));
		removeTab (indexOf (d.force_opp));
#if	EXPERIMENTAL
		removeTab (indexOf (d.compare));
#endif	/* EXPERIMENTAL */
		removeTab (indexOf (d.stratmap));

		addTab (d.disabled_label, "Overview");
	}
	if (!hidden) show();
}

void
GuiRptTrn::select (MDLD_TREE_ITEM *item)
{
	if (d.item == item) return;

	d.item = item;
	enable (d.item != NULL);
	refresh();
}

void
GuiRptTrn::refresh (void)
{
	DBG_TRACE_FENTER;

	if (d.enabled) {
		char	buf[256];

		memset (buf, 0, sizeof (buf));

		if (d.item->campaign) {
			setTabIcon (indexOf (d.force_core), *RES_flag (d.item->data.t->battle->dossier->OOB));
			setTabIcon (indexOf (d.force_spt), *RES_flag (d.item->data.t->battle->dossier->OOB));
		} else {
			snprintf (buf, sizeof (buf) - 1, "%s force", SPWAW_oob_people (d.item->data.t->battle->OOB_p1));
			setTabText (indexOf (d.force_spt), buf);
			setTabIcon (indexOf (d.force_spt), *RES_flag (d.item->data.t->battle->OOB_p1));
		}
		snprintf (buf, sizeof (buf) - 1, "%s force", SPWAW_oob_people (d.item->data.t->battle->OOB_p2));
		setTabText (indexOf (d.force_opp), buf);
		setTabIcon (indexOf (d.force_opp), *RES_flag (d.item->data.t->battle->OOB_p2));
	}

	d.overview->refresh();
	d.force_core->refresh();
	d.force_spt->refresh();
	d.force_opp->refresh();
#if	EXPERIMENTAL
	d.compare->refresh();
#endif	/* EXPERIMENTAL */
	d.stratmap->refresh();

	DBG_TRACE_FLEAVE;
}

MDLD_TREE_ITEM *
GuiRptTrn::current (void)
{
	return (d.item);
}

void
GuiRptTrn::will_delete (MDLD_TREE_ITEM *item)
{
	if (!item) return;
	switch (item->type) {
		case MDLD_TREE_DOSSIER:
			break;
		case MDLD_TREE_BATTLE:
			if (item != d.item->parent) return;
			break;
		case MDLD_TREE_BTURN:
			if (item != d.item) return;
			break;
	}

	select (NULL);
}

void
GuiRptTrn::selectedCurrentIndex (int index)
{
	int	i;

	if (!isVisible()) return;

	refresh();

	for (i=0; i<count(); i++)
		tabBar()->setTabTextColor (i, *((i!=index) ? RES_color (RID_TAB_FG_DEFAULT): RES_color (RID_TAB_FG_SEL)));

	d.last_index = index;
}
