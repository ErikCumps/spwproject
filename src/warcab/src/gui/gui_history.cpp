/** \file
 * The SPWaW war cabinet - GUI - unit history widget.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_history.h"
#include "gui_history_view.h"
#include "gui_unitlist_view.h"
#include "model/model_history.h"
#include "model/model_unitlist.h"
#include "gui_reports_dossier.h"
#include "gui_reports_battle.h"

/*! These highlights are available for dossier views */
static MDLH_HILITE	hilite_dossier[] = {
	MDLH_HILITE_NONE, MDLH_HILITE_RANK, MDLH_HILITE_EXP
};

/*! These highlights are available for battle and turn views */
static MDLH_HILITE	hilite_battle[] = {
	MDLH_HILITE_NONE, MDLH_HILITE_USTATUS, MDLH_HILITE_ENTR
};

GuiHistory::GuiHistory (QWidget *P)
	: QFrame (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	GUINEW (d.lmodel, ModelUnitlist(), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "unitlist data model");

	GUINEW (d.hmodel, ModelHistory(), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "history data model");

	GUINEW (d.font, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "font");

	GUINEW (d.layout, QGridLayout (this), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "layout");

	GUINEW (d.highlight, QComboBox (this), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "highlight");
	d.highlight->setEditable (false);

	GUINEW (d.prevcmp, QCheckBox ("prevcmp?", this), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "prevcmp");
	d.prevcmp->setCheckState (Qt::Unchecked);

	GUINEW (d.spacer, QSpacerItem (1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum), ERR_GUI_SMAP_INIT_FAILED, "spacer");

	GUINEW (d.split, QSplitter (Qt::Horizontal, this), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "split");
	d.split->setChildrenCollapsible (false);

	GUINEW (d.unitlist, GuiUnitlistView (this, d.split), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "unitlist");
	GUIERR (d.unitlist, ERR_GUI_REPORTS_HISTORY_INIT_FAILED);

	GUINEW (d.hdr_history, GuiHistoryView (true, this, d.split), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "hdr_history");
	GUIERR (d.hdr_history, ERR_GUI_REPORTS_HISTORY_INIT_FAILED);

	GUINEW (d.bdy_history, GuiHistoryView (false, this, d.split), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "bdy_history");
	GUIERR (d.bdy_history, ERR_GUI_REPORTS_HISTORY_INIT_FAILED);

	d.layout->addWidget (d.highlight,	0, 0, 1, 1);
	d.layout->addWidget (d.prevcmp,		0, 1, 1, 1);
	d.layout->addItem   (d.spacer,		0, 1, 1, 2);
	d.layout->addWidget (d.split,		1, 0, 1, 4);

	if (!connect (d.prevcmp, SIGNAL(stateChanged(int)), SLOT (prevcmp_change(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "failed to connect <prevcmp:stateChanged> to <prevcmp_change>");

	if (!connect (d.highlight, SIGNAL (activated(int)), SLOT (set_highlight(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "failed to connect <highlight:activated> to <set_highlight>");

	if (!connect (d.unitlist, SIGNAL (selected(int)), SLOT (set_select(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "failed to connect <unitlist:selected> to <set_select>");

	if (!connect (d.bdy_history->verticalScrollBar(), SIGNAL (valueChanged (int)), d.hdr_history, SLOT (scroll(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "failed to connect <bdy_history:scrolled> to <hdr_history:scroll>");

	if (!connect (d.hdr_history, SIGNAL (wantscroll(int)), d.bdy_history, SLOT (scroll(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "failed to connect <hdr_history:wantscroll> to <bdy_history:scroll>");

	if (!connect (d.hdr_history, SIGNAL (wheeled(QWheelEvent*)), d.bdy_history, SLOT (wheel(QWheelEvent*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "failed to connect <hdr_history:wheeled> to <bdy_history:wheel>");

	if (!connect (this, SIGNAL (cmpcurr(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpcurr(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "failed to connect <cmpcurr> to <dossier:set_cmpcurr>");

	if (!connect (this, SIGNAL (cmpbase(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpbase(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "failed to connect <cmpbase> to <dossier:set_cmpbase>");

	if (!connect (d.bdy_history, SIGNAL (selected(GuiHistoryView*,const QModelIndex&)), this, SLOT (selected(GuiHistoryView*,const QModelIndex&))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "failed to connect <bdy_history:selected> to <selected>");

	if (!connect (d.hdr_history, SIGNAL (selected(GuiHistoryView*,const QModelIndex&)), this, SLOT (selected(GuiHistoryView*,const QModelIndex&))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "failed to connect <hdr_history:selected> to <selected>");

	d.pflag = d.cflag = true;

	SET_GUICLS_NOERR;
}

GuiHistory::~GuiHistory (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
	delete d.lmodel;
	delete d.hmodel;
}

void
GuiHistory::set_parent (GuiRptDsr *parent, bool player)
{
	d.ptype	 = MDLD_TREE_DOSSIER;
	d.pptr.d = parent;
	d.pflag	 = player;
	d.cflag	 = true;

	d.prevcmp->setText ("Last battle only?");
	d.prevcmp->setHidden (false);

	d.hl_array = hilite_dossier;
	d.hl_count = ARRAYCOUNT(hilite_dossier);
	setup_highlight();
}

void
GuiHistory::set_parent (GuiRptBtl *parent, bool player, bool core)
{
	d.ptype	 = MDLD_TREE_BATTLE;
	d.pptr.b = parent;
	d.pflag	 = player;
	d.cflag	 = core;

	d.prevcmp->setText ("Compare with previous turn?");
	d.prevcmp->setHidden (false);

	d.hl_array = hilite_battle;
	d.hl_count = ARRAYCOUNT(hilite_battle);
	setup_highlight();
}

void
GuiHistory::set_parent (GuiRptTrn *parent, bool player)
{
	d.ptype	 = MDLD_TREE_BTURN;
	d.pptr.t = parent;
	d.pflag	 = player;
	d.cflag	 = false;

	d.prevcmp->setText ("This turn only?");
	d.prevcmp->setHidden (false);

	d.hl_array = hilite_battle;
	d.hl_count = ARRAYCOUNT(hilite_battle);
	setup_highlight();
}

void	
GuiHistory::setup_highlight (void)
{
	d.highlight->clear();
	for (int i=0; i<d.hl_count; i++)
		d.highlight->addItem (QString ("Highlight: ") + QString (MDLH_HILITE_lookup (d.hl_array[i])));
}

void
GuiHistory::update (void)
{
	MDLD_TREE_ITEM	*item = NULL;
	bool		skip;
	QModelIndex	idx;
	MDLH_INFO	info;
	QString		s;

	DBG_TRACE_FENTER;

	switch (d.ptype) {
		case MDLD_TREE_DOSSIER:
			item = d.pptr.d ? d.pptr.d->current() : NULL;
			break;
		case MDLD_TREE_BATTLE:
			item = d.pptr.b ? d.pptr.b->current() : NULL;
			break;
		default:
			break;
	}

	skip =  !d.reftrack.changed (item);
	skip &= !GUIVALCHANGED (uidx);
	skip &= !GUIVALCHANGED (Vprevcmp);
	if (skip) goto skip_data_update;

	DBG_TRACE_UPDATE;

	d.pdata = item;
	if (d.pdata) {
		idx = d.hdr_history->currentIndex();
		switch (d.ptype) {
			case MDLD_TREE_DOSSIER:
				d.hmodel->load (item->data.d, d.Vprevcmp, d.uidx);
				d.hdr_history->battleview (false);
				d.bdy_history->battleview (false);
				break;
			case MDLD_TREE_BATTLE:
				d.hmodel->load (item->data.b, d.pflag, d.cflag, d.Vprevcmp, d.uidx);
				d.hdr_history->battleview (true);
				d.bdy_history->battleview (true);
				break;
			default:
				break;
		}
		d.hdr_history->select (idx);

		d.hmodel->info (info);

		idx = d.unitlist->currentIndex();
		d.lmodel->load (info.bir, info.bir_cnt);
		d.unitlist->select (idx);
	} else {
		d.hmodel->clear();
		d.lmodel->clear();
	}

skip_data_update:
	/* Only emit these signals when widget is visible */
	if (isVisible()) {
		emit cmpcurr (NULL);
		emit cmpbase (NULL);
	}

	DBG_TRACE_FLEAVE;
}

void
GuiHistory::refresh (void)
{
	DBG_TRACE_FENTER;

	update();
	d.unitlist->reload();
	d.hdr_history->reload();
	d.bdy_history->reload();

	DBG_TRACE_FLEAVE;
}

void
GuiHistory::prevcmp_change (int state)
{
	d.Vprevcmp = (state != Qt::Unchecked);

	refresh();
}

void
GuiHistory::set_select (int idx)
{
	d.uidx = (idx < 0) ? 0 : (USHORT)idx;
	d.hmodel->select (d.uidx);
	d.hdr_history->refresh();
	d.bdy_history->refresh();
}

void
GuiHistory::set_highlight (int idx)
{
	d.hmodel->highlight ((MDLH_HILITE)d.hl_array[idx]);
	d.hdr_history->refresh();
	d.bdy_history->refresh();
}

void
GuiHistory::selected (GuiHistoryView *who, const QModelIndex &index)
{
	if (who != d.hdr_history) d.hdr_history->select (index);
	if (who != d.bdy_history) d.bdy_history->select (index);
	d.hdr_history->scrollTo (index);
	d.hdr_history->refresh();
}
