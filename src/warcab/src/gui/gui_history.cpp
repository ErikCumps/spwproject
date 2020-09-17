/** \file
 * The SPWaW war cabinet - GUI - unit history widget.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
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

	GUINEW (d.rgfont, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "rgfont");
	GUINEW (d.dcfont, QFont ("Courier", 8, QFont::Normal, true) , ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "dcfont");

	GUINEW (d.lmodel, ModelUnitlist(d.rgfont, d.dcfont), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "unitlist data model");

	GUINEW (d.hmodel, ModelHistory(d.rgfont, d.dcfont), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "history data model");

	GUINEW (d.layout, QGridLayout (this), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "layout");

	GUINEW (d.intel, QLabel (this), ERR_GUI_REPORTS_INIT_FAILED, "intel");
	d.intel->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.intel->setWordWrap (true);
	d.intel->setSizePolicy (QSizePolicy (QSizePolicy::Minimum, QSizePolicy::Fixed));

	GUINEW (d.highlight, QComboBox (this), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "highlight");
	d.highlight->setEditable (false);

	GUINEW (d.prevcmp, QCheckBox ("prevcmp?", this), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "prevcmp");
	d.prevcmp->setCheckState (Qt::Unchecked);

	GUINEW (d.spacer, QSpacerItem (1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum), ERR_GUI_SMAP_INIT_FAILED, "spacer");

	GUINEW (d.separator, QFrame (this), ERR_GUI_REPORTS_INIT_FAILED, "separator");
	d.separator->setFrameStyle (QFrame::HLine);

	GUINEW (d.dfont, QFont (), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "detail font");

	GUINEW (d.detail, QLabel (this), ERR_GUI_REPORTS_INIT_FAILED, "detail label");
	d.detail->setSizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
	d.detail->setFont (*d.dfont);

	GUINEW (d.split, QSplitter (Qt::Horizontal, this), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "master split");
	d.split->setChildrenCollapsible (false);
	d.split->setSizePolicy (QSizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

	GUINEW (d.unitlist, GuiUnitlistView (this, d.split), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "unitlist");
	GUIERR (d.unitlist, ERR_GUI_REPORTS_HISTORY_INIT_FAILED);

	GUINEW (d.hdr_history, GuiHistoryView (true, this, d.split), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "hdr_history");
	GUIERR (d.hdr_history, ERR_GUI_REPORTS_HISTORY_INIT_FAILED);

	GUINEW (d.bdy_history, GuiHistoryView (false, this, d.split), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "bdy_history");
	GUIERR (d.bdy_history, ERR_GUI_REPORTS_HISTORY_INIT_FAILED);

	d.layout->addWidget (d.intel,		0, 0, 1, 4);
	d.layout->addWidget (d.highlight,	1, 0, 1, 1);
	d.layout->addWidget (d.prevcmp,		1, 1, 1, 1);
	d.layout->addItem   (d.spacer,		1, 1, 1, 2);
	d.layout->addWidget (d.separator,	2, 0, 1, 4);
	d.layout->addWidget (d.detail,		3, 0, 1, 4);
	d.layout->addWidget (d.split,		4, 0, 1, 4);

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

	if (!connect (GUI_WIN, SIGNAL (selected_intel_mode(INTEL_MODE)), this, SLOT (intel_mode_set(INTEL_MODE))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <mainwindow:selected_intel_mode> to <intel_mode_set>");

	if (!connect (GUI_WIN, SIGNAL (selected_intel_mode(INTEL_MODE)), d.lmodel, SLOT (intel_mode_set(INTEL_MODE))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <mainwindow:selected_intel_mode> to <lmodel:intel_mode_set>");

	if (!connect (GUI_WIN, SIGNAL (selected_intel_mode(INTEL_MODE)), d.hmodel, SLOT (intel_mode_set(INTEL_MODE))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <mainwindow:selected_intel_mode> to <hmodel:intel_mode_set>");

	if (!connect (GUI_WIN, SIGNAL (selected_intel_mode(INTEL_MODE)), d.hdr_history, SLOT (intel_mode_set(INTEL_MODE))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <mainwindow:selected_intel_mode> to <hdr_history:intel_mode_set>");

	if (!connect (GUI_WIN, SIGNAL (selected_intel_mode(INTEL_MODE)), d.bdy_history, SLOT (intel_mode_set(INTEL_MODE))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <mainwindow:selected_intel_mode> to <bdy_history:intel_mode_set>");

	d.pflag = d.cflag = true;
	d.mflag = false;

	SET_GUICLS_NOERR;
}

GuiHistory::~GuiHistory (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.lmodel;
	delete d.hmodel;

	delete d.rgfont;
	delete d.dcfont;
	delete d.dfont;
}

void
GuiHistory::set_parent (GuiRptDsr *parent, bool player)
{
	d.ptype	 = MDLD_TREE_DOSSIER;
	d.pptr.d = parent;
	d.pflag	 = player;
	d.cflag	 = true;

	d.prevcmp->setText ("Compare with previous battle?");
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
GuiHistory::setup_highlight (void)
{
	d.highlight->clear();
	for (int i=0; i<d.hl_count; i++)
		d.highlight->addItem (QString ("Highlight: ") + QString (MDLH_HILITE_lookup (d.hl_array[i])));
}

bool
GuiHistory::update (bool forced)
{
	MDLD_TREE_ITEM	*item = NULL;
	bool		skip;
	QModelIndex	lidx;
	QModelIndex	hidx;
	QString		s;

	DBG_TRACE_FENTER;

	switch (d.ptype) {
		case MDLD_TREE_DOSSIER:
			item = d.pptr.d ? d.pptr.d->current() : NULL;
			d.mflag = (d.pflag && d.cflag && item && (item->dossier_type != SPWAW_STDALONE_DOSSIER));
			break;
		case MDLD_TREE_BATTLE:
			item = d.pptr.b ? d.pptr.b->current() : NULL;
			d.mflag = (d.pflag && d.cflag && item && (item->dossier_type != SPWAW_STDALONE_DOSSIER));
			break;
		default:
			break;
	}

	skip =  !d.reftrack.changed (item);
	skip &= !GUIVALCHANGED (uidx);
	skip &= !GUIVALCHANGED (Vprevcmp);
	skip &= !forced;
	if (skip) goto skip_data_update;

	DBG_TRACE_UPDATE;

	d.pdata = item;
	if (d.pdata) {
		lidx = d.unitlist->currentIndex();
		hidx = d.hdr_history->currentIndex();
		switch (d.ptype) {
			case MDLD_TREE_DOSSIER:
				d.hmodel->load (item->data.d, d.Vprevcmp, d.uidx, d.Vintel_mode);
				d.lmodel->load (item->data.d, CFG_full_history(), d.Vintel_mode);
				d.ghvmode = GHV_MODE_DOSSIER;
				break;
			case MDLD_TREE_BATTLE:
				d.hmodel->load (item->data.b, d.pflag, d.cflag, d.Vprevcmp, d.uidx, d.Vintel_mode);
				d.lmodel->load (item->data.b, d.pflag, d.cflag, d.Vintel_mode);
				d.ghvmode = GHV_MODE_BATTLE;
				break;
			default:
				break;
		}
		d.unitlist->select (lidx);
		d.hdr_history->select (hidx);
	} else {
		d.hmodel->clear();
		d.lmodel->clear();
	}
	d.hmodel->set_marking (d.mflag);

skip_data_update:
	/* Only emit these signals when widget is visible */
	if (isVisible()) {
		emit cmpcurr (NULL);
		emit cmpbase (NULL);
	}

	DBG_TRACE_FLEAVE;

	return (skip);
}

void
GuiHistory::refresh (bool forced)
{
	bool	skip;

	DBG_TRACE_FENTER;

	skip = update(forced);
	if (skip) goto leave;

	d.unitlist->reload();
	d.hdr_history->reload(d.ghvmode, d.mflag, d.pflag, d.Vintel_mode);
	d.bdy_history->reload(d.ghvmode, d.mflag, d.pflag, d.Vintel_mode);

leave:
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
	MDLH_IDENTITY		id;
	char			buf[1024];
	UtilStrbuf		str(buf, sizeof (buf), true, true);

	d.uidx = (idx < 0) ? 0 : (USHORT)idx;
	d.hmodel->select (d.uidx);
	d.hmodel->identity (id);

	str.printf ("%s history", (d.ghvmode == GHV_MODE_DOSSIER) ? "Campaign" : "Battle");
	if (id.uir) {
		str.printf (" for: <b>%s %s %s %s</b>",
			id.uir->snap->strings.uid, id.uir->snap->data.dname,
			id.uir->snap->strings.rank, id.uir->snap->data.lname);
		if (id.uhte) {
			SPWAW_BDATE (id.uhte->FBD, CD, false);
			SPWAW_BDATE (SPWAW_UHT_last (id.uhte)->LBD, DD, false);
			str.printf ("&nbsp;&nbsp;&nbsp;[%s -> %s]", CD, DD);
		}
	}
	d.detail->setText (buf);

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

void
GuiHistory::intel_mode_set (INTEL_MODE mode)
{
	d.Vintel_mode = mode;

	intelmode2label (d.Vintel_mode, d.pflag, d.intel);

	if (!d.pflag && d.Vintel_mode == INTEL_MODE_NONE) {
		d.detail->clear();
	} else {
		set_select (d.uidx);
	}
}