/** \file
 * The SPWaW war cabinet - GUI - battle report.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_reports_battle.h"

GuiRptBtl::GuiRptBtl (QWidget *P)
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

	GUINEW (d.overview, GuiRptBtlOvr (), ERR_GUI_REPORTS_INIT_FAILED, "overview page");
	GUIERR (d.overview, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.force_core, GuiRptBtlFrc (), ERR_GUI_REPORTS_INIT_FAILED, "core force page");
	GUIERR (d.force_core, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.force_spt, GuiRptBtlFrc (), ERR_GUI_REPORTS_INIT_FAILED, "support force page");
	GUIERR (d.force_spt, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.force_opp, GuiRptBtlFrc (), ERR_GUI_REPORTS_INIT_FAILED, "opponent force page");
	GUIERR (d.force_opp, ERR_GUI_REPORTS_INIT_FAILED);

#if	EXPERIMENTAL
	GUINEW (d.compare, GuiRptBtlCmp (), ERR_GUI_REPORTS_INIT_FAILED, "comparison page");
	GUIERR (d.compare, ERR_GUI_REPORTS_INIT_FAILED);
#endif	/* EXPERIMENTAL */

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

	d.last_index = currentIndex();
	tabBar()->setTabTextColor (currentIndex(), *RES_color (RID_TAB_FG_SEL));

	SET_GUICLS_NOERR;
}

GuiRptBtl::~GuiRptBtl (void)
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
}

void
GuiRptBtl::enable (bool flag)
{
	if (flag == d.enabled) return;

	set_enabled (flag);
}

void
GuiRptBtl::set_enabled (bool flag)
{
	bool	hidden = isHidden();

	d.enabled = flag;
	if (!hidden) hide();
	if (flag) {
		removeTab (indexOf (d.disabled_label));
		addTab (d.overview, "Overview");
		if (d.item->dossier_type != SPWAW_STDALONE_DOSSIER) {
			addTab (d.force_core, "Core force");
			if (d.item->data.b->snap->OOBp1.support.formations.cnt != 0) addTab (d.force_spt, "Support force");
		} else {
			addTab (d.force_spt, "Support force");
		}
		addTab (d.force_opp, "Opponent force");
#if	EXPERIMENTAL
		addTab (d.compare, "Comparisons");
#endif	/* EXPERIMENTAL */

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
		addTab (d.disabled_label, "Overview");
	}
	if (!hidden) show();
}

void
GuiRptBtl::select (MDLD_TREE_ITEM *item)
{
	bool	skip;

	d.item = item;

	skip = !d.reftrack.changed (d.item);
	if (skip) return;

	enable (d.item != NULL);
	refresh(false);
}

void
GuiRptBtl::refresh (bool forced)
{
	DBG_TRACE_FENTER;

	if (d.enabled) {
		char	buf[256];

		memset (buf, 0, sizeof (buf));

		if (d.item->dossier_type != SPWAW_STDALONE_DOSSIER) {
			setTabIcon (indexOf (d.force_core), *RES_flagbyid (d.item->data.b->snap->game.battle.strings.flagid_p1));
			setTabIcon (indexOf (d.force_spt), *RES_flagbyid (d.item->data.b->snap->game.battle.strings.flagid_p1));
		} else {
			snprintf (buf, sizeof (buf) - 1, "%s force", d.item->data.b->snap->game.battle.strings.people_p1);
			setTabText (indexOf (d.force_spt), buf);
			setTabIcon (indexOf (d.force_spt), *RES_flagbyid (d.item->data.b->snap->game.battle.strings.flagid_p1));
		}
		snprintf (buf, sizeof (buf) - 1, "%s force", d.item->data.b->snap->game.battle.strings.people_p2);
		setTabText (indexOf (d.force_opp), buf);
		setTabIcon (indexOf (d.force_opp), *RES_flagbyid (d.item->data.b->snap->game.battle.strings.flagid_p2));
	}

	d.overview->refresh(forced);
	d.force_core->refresh(forced);
	d.force_spt->refresh(forced);
	d.force_opp->refresh(forced);
#if	EXPERIMENTAL
	d.compare->refresh(forced);
#endif	/* EXPERIMENTAL */

	DBG_TRACE_FLEAVE;
}

MDLD_TREE_ITEM *
GuiRptBtl::current (void)
{
	return (d.item);
}

void
GuiRptBtl::will_delete (MDLD_TREE_ITEM *item)
{
	if (!item) return;
	switch (item->type) {
		case MDLD_TREE_DOSSIER:
			break;
		case MDLD_TREE_BATTLE:
			if (item != d.item) return;
			break;
		case MDLD_TREE_BTURN:
			if (item->parent != d.item) return;
			break;
	}

	select (NULL);
}

void
GuiRptBtl::selectedCurrentIndex (int index)
{
	int	i;

	if (!isVisible()) return;

	refresh();

	for (i=0; i<count(); i++)
		tabBar()->setTabTextColor (i, *((i!=index) ? RES_color (RID_TAB_FG_DEFAULT): RES_color (RID_TAB_FG_SEL)));

	d.last_index = index;
}
