/** \file
 * The SPWaW war cabinet - GUI - unit roster widget.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_roster.h"
#include "gui_reports_dossier.h"
#include "gui_reports_battle.h"
#include "gui_reports_bturn.h"

/*! These highlights are available for dossier views */
static MDLR_HILITE	hilite_dossier[] = {
	MDLR_HILITE_NONE, MDLR_HILITE_RANK, MDLR_HILITE_EXP
};

/*! These highlights are available for battle and turn views */
static MDLR_HILITE	hilite_battle[] = {
	MDLR_HILITE_NONE, MDLR_HILITE_RANK, MDLR_HILITE_USTATUS, MDLR_HILITE_ENTR, MDLR_HILITE_EXP
};

GuiRoster::GuiRoster (QWidget *P)
	: QFrame (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	GUINEW (d.rgfont, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "rgfont");
	GUINEW (d.dcfont, QFont ("Courier", 8, QFont::Normal, true) , ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "dcfont");

	GUINEW (d.model, ModelRoster (d.rgfont, d.dcfont), ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "data model");

	GUINEW (d.layout, QGridLayout (this), ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "layout");

#if	EXPERIMENTAL
	GUINEW (d.filter_spec, QComboBox (this), ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "filter_spec");
	for (int h=MDLR_FILTER_NONE; h<=MDLR_FILTER_LIMIT; h++)
		d.filter_spec->addItem (QString ("Filter spec: ") + QString (MDLR_FILTER_lookup ((MDLR_FILTER)h)));
	d.filter_spec->setEditable (false);

	GUINEW (d.filter_tgt, QComboBox (this), ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "filter_tgt");
	d.filter_tgt->addItem ("Filter target: none");
	d.filter_tgt->setEditable (false);
#endif	/* EXPERIMENTAL */

	GUINEW (d.highlight, QComboBox (this), ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "highlight");
	d.highlight->setEditable (false);

	GUINEW (d.dltsort, QCheckBox ("Sort on difference?", this), ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "dltsort");
	d.dltsort->setCheckState (Qt::Unchecked);

	GUINEW (d.prevcmp, QCheckBox ("prevcmp?", this), ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "prevcmp");
	d.prevcmp->setCheckState (Qt::Unchecked);

	GUINEW (d.autosort, QCheckBox ("Automatically resort?", this), ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "autosort");
	d.autosort->setCheckState (Qt::Checked);

	GUINEW (d.split, QSplitter (Qt::Horizontal, this), ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "split");
	d.split->setChildrenCollapsible (false);

	GUINEW (d.hdr_roster, GuiRosterView (true, this, d.split), ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "hdr_roster");
	GUIERR (d.hdr_roster, ERR_GUI_REPORTS_ROSTER_INIT_FAILED);

	GUINEW (d.bdy_roster, GuiRosterView (false, this, d.split), ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "bdy_roster");
	GUIERR (d.bdy_roster, ERR_GUI_REPORTS_ROSTER_INIT_FAILED);

#if	EXPERIMENTAL
	d.layout->addWidget (d.filter_spec,	0, 0, 1, 2);
	d.layout->addWidget (d.filter_tgt,	0, 2, 1, 2);
#endif	/* EXPERIMENTAL */
	d.layout->addWidget (d.highlight,	1, 0, 1, 1);
	d.layout->addWidget (d.prevcmp,		1, 1, 1, 1);
	d.layout->addWidget (d.dltsort,		1, 2, 1, 1);
	d.layout->addWidget (d.autosort,	1, 3, 1, 1);
	d.layout->addWidget (d.split,		2, 0, 1, 4);

	if (!connect (d.dltsort, SIGNAL(stateChanged(int)), SLOT (dltsort_change(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <dltsort:stateChanged> to <dltsort_change>");

	if (!connect (d.prevcmp, SIGNAL(stateChanged(int)), SLOT (prevcmp_change(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <prevcmp:stateChanged> to <prevcmp_change>");

	if (!connect (d.autosort, SIGNAL(stateChanged(int)), SLOT (autosort_change(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <autosort:stateChanged> to <autosort_change>");

	if (!connect (d.highlight, SIGNAL (activated(int)), SLOT (set_highlight(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <highlight:activated> to <set_highlight>");

#if	EXPERIMENTAL
	if (!connect (d.filter_spec, SIGNAL (activated(int)), SLOT (set_filter_spec(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <filter_spec:activated> to <set_filter_spec>");

	if (!connect (d.filter_tgt, SIGNAL (activated(int)), SLOT (set_filter_tgt(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <filter_tgt:activated> to <set_filter_tgt>");
#endif	/* EXPERIMENTAL */

	if (!connect (d.bdy_roster->verticalScrollBar(), SIGNAL (valueChanged (int)), d.hdr_roster, SLOT (scroll(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <bdy_roster:scrolled> to <hdr_roster:scroll>");

	if (!connect (d.hdr_roster, SIGNAL (wantscroll(int)), d.bdy_roster, SLOT (scroll(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <hdr_roster:wantscroll> to <bdy_roster:scroll>");

	if (!connect (d.bdy_roster, SIGNAL (sorted(GuiRosterView*,int,Qt::SortOrder)), this, SLOT (sort(GuiRosterView*,int,Qt::SortOrder))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <bdy_roster:sorted> to <sort>");

	if (!connect (d.hdr_roster, SIGNAL (sorted(GuiRosterView*,int,Qt::SortOrder)), this, SLOT (sort(GuiRosterView*,int,Qt::SortOrder))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <hdr_roster:sorted> to <sort>");

	if (!connect (d.hdr_roster, SIGNAL (wheeled(QWheelEvent*)), d.bdy_roster, SLOT (wheel(QWheelEvent*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <hdr_roster:wheeled> to <bdy_roster:wheel>");

	if (!connect (this, SIGNAL (cmpcurr(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpcurr(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <cmpcurr> to <dossier:set_cmpcurr>");

	if (!connect (this, SIGNAL (cmpbase(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpbase(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <cmpbase> to <dossier:set_cmpbase>");

	if (!connect (d.bdy_roster, SIGNAL (selected(GuiRosterView*,const QModelIndex&)), this, SLOT (selected(GuiRosterView*,const QModelIndex&))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <bdy_roster:selected> to <selected>");

	if (!connect (d.hdr_roster, SIGNAL (selected(GuiRosterView*,const QModelIndex&)), this, SLOT (selected(GuiRosterView*,const QModelIndex&))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <hdr_roster:selected> to <selected>");

	d.pflag = d.cflag = true;
	d.mflag = false;
	d.Vautosort = true;

#if	EXPERIMENTAL
	d.Vfspec = MDLR_FILTER_NONE;
	for (int h=MDLR_FILTER_NONE; h<=MDLR_FILTER_LIMIT; h++) {
		xMDL_FILTER_new (&(d.Vftgts[h].list));
		d.Vftgts[h].curr = 0;
	}
#endif	/* EXPERIMENTAL */

	SET_GUICLS_NOERR;
}

GuiRoster::~GuiRoster (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets

#if	EXPERIMENTAL
	for (int h=MDLR_FILTER_NONE; h<=MDLR_FILTER_LIMIT; h++) xMDL_FILTER_free (&(d.Vftgts[h].list));
#endif	/* EXPERIMENTAL */
	delete d.model;
	delete d.rgfont;
	delete d.dcfont;
}

void
GuiRoster::set_parent (GuiRptDsr *parent, bool player)
{
	d.ptype	 = MDLD_TREE_DOSSIER;
	d.pptr.d = parent;
	d.pflag	 = player;
	d.cflag	 = true;

	d.prevcmp->setText ("-unused-");
	d.prevcmp->setHidden (true);

	d.hl_array = hilite_dossier;
	d.hl_count = ARRAYCOUNT(hilite_dossier);
	setup_highlight();
}

void
GuiRoster::set_parent (GuiRptBtl *parent, bool player, bool core)
{
	d.ptype	 = MDLD_TREE_BATTLE;
	d.pptr.b = parent;
	d.pflag	 = player;
	d.cflag	 = core;

	d.prevcmp->setText ("-unused-");
	d.prevcmp->setHidden (true);

	d.hl_array = hilite_battle;
	d.hl_count = ARRAYCOUNT(hilite_battle);
	setup_highlight();
}

void
GuiRoster::set_parent (GuiRptTrn *parent, bool player, bool core)
{
	d.ptype	 = MDLD_TREE_BTURN;
	d.pptr.t = parent;
	d.pflag	 = player;
	d.cflag	 = core;

	d.prevcmp->setText ("Last turn only?");
	d.prevcmp->setHidden (false);

	d.hl_array = hilite_battle;
	d.hl_count = ARRAYCOUNT(hilite_battle);
	setup_highlight();
}

void	
GuiRoster::setup_highlight (void)
{
	d.highlight->clear();
	for (int i=0; i<d.hl_count; i++)
		d.highlight->addItem (QString ("Highlight: ") + QString (MDLR_HILITE_lookup (d.hl_array[i])));
}

bool
GuiRoster::update (bool forced)
{
	MDLD_TREE_ITEM	*item = NULL;
	bool		skip;
	QModelIndex	idx;

	DBG_TRACE_FENTER;

	d.psort = true;
	switch (d.ptype) {
		case MDLD_TREE_DOSSIER:
			item = d.pptr.d ? d.pptr.d->current() : NULL;
			if (item && d.pdata) d.psort = false;
			d.mflag = (d.pflag && d.cflag && item && item->dossier_type == SPWAW_CAMPAIGN_DOSSIER);
			break;
		case MDLD_TREE_BATTLE:
			item = d.pptr.b ? d.pptr.b->current() : NULL;
			if (item && d.pdata) d.psort = (item->data.b != d.pdata->data.b);
			d.mflag = (d.pflag && d.cflag && item && item->dossier_type == SPWAW_CAMPAIGN_DOSSIER);
			break;
		case MDLD_TREE_BTURN:
			item = d.pptr.t ? d.pptr.t->current() : NULL;
			if (item && d.pdata) d.psort = (item->data.t->battle != d.pdata->data.t->battle);
			d.mflag = false;
			break;
		default:
			break;
	}

	skip =  !d.reftrack.changed (item);
	skip &= !GUIVALCHANGED (Vdltsort);
	skip &= !GUIVALCHANGED (Vprevcmp);
	skip &= !GUIVALCHANGED (Vautosort);
	skip &= !forced;
	if (skip) goto skip_data_update;

	DBG_TRACE_UPDATE;

	d.pdata = item; d.pcurr = d.pbase = NULL;
	if (d.pdata) {
		idx = d.hdr_roster->currentIndex();
		switch (d.ptype) {
			case MDLD_TREE_DOSSIER:
				if (!d.pdata->data.d) return (skip);

				d.model->load (d.pdata->data.d, CFG_full_history());
				d.grvmode = GRV_MODE_DOSSIER;
				break;
			case MDLD_TREE_BATTLE:
				if (!d.pdata->data.b) return (skip);

				d.pcurr = d.pdata;
				d.pbase = d.pdata->prev ? d.pdata->prev : d.pdata;
				d.model->load (d.pcurr->data.b, d.pbase->data.b, d.pflag, d.cflag);
				d.grvmode = GRV_MODE_BATTLE;
				break;
			case MDLD_TREE_BTURN:
				if (!d.pdata->data.t) return (skip);

				d.pcurr = d.pdata;
				if (d.Vprevcmp && d.pdata->prev) {
					d.pbase = d.pdata->prev;
					d.model->load (d.pcurr->data.t, d.pbase->data.t, d.pflag, d.cflag);
				} else {
					d.pbase = d.pdata->parent->cfirst;
					d.model->load (d.pcurr->data.t, d.pbase->data.t, d.pflag, d.cflag);
				}
				d.grvmode = GRV_MODE_TURN;
				break;
			default:
				break;
		}
		d.hdr_roster->select (idx);
	} else {
		d.pbase = d.pcurr = NULL;
		d.model->clear();
	}
	d.model->set_marking (d.mflag);

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
GuiRoster::refresh (bool forced)
{
	bool	skip;

	DBG_TRACE_FENTER;

	skip = update(forced);
	if (skip) goto leave;

	d.hdr_roster->reload (d.grvmode, d.psort || d.Vautosort, d.mflag);
	d.bdy_roster->reload (d.grvmode, d.psort || d.Vautosort, d.mflag);

	d.psort = false;

leave:
	DBG_TRACE_FLEAVE;
}

void
GuiRoster::dltsort_change (int state)
{
	d.Vdltsort = (state != Qt::Unchecked);

	d.model->set_dltsort (d.Vdltsort);
	if (d.Vautosort) {
		d.model->resort();
		refresh();
	}
}

void
GuiRoster::prevcmp_change (int state)
{
	d.Vprevcmp = (state != Qt::Unchecked);

	refresh();
	d.hdr_roster->reload (d.grvmode, false, d.mflag);
	d.bdy_roster->reload (d.grvmode, false, d.mflag);
}

void
GuiRoster::autosort_change (int state)
{
	d.Vautosort = (state != Qt::Unchecked);

	if (d.Vautosort) {
		refresh();
		d.hdr_roster->reload(d.grvmode, false, d.mflag);
		d.bdy_roster->reload(d.grvmode, false, d.mflag);
	}
}

void
GuiRoster::set_highlight (int idx)
{
	d.model->highlight ((MDLR_HILITE)d.hl_array[idx]);
	d.hdr_roster->refresh();
	d.bdy_roster->refresh();
}

#if	EXPERIMENTAL
void
GuiRoster::set_filter_spec (int idx)
{
	unsigned int	i;

	d.Vfspec = (MDLR_FILTER)idx;

	d.model->filter_targets (d.Vfspec, d.Vftgts[d.Vfspec].list);

	d.filter_tgt->clear();
	for (i=0; i<d.Vftgts[d.Vfspec].list->used; i++)
		d.filter_tgt->addItem (QString ("Filter target: ") + QString (d.Vftgts[d.Vfspec].list->data[i].name));

	if (d.Vftgts[d.Vfspec].curr >= d.Vftgts[d.Vfspec].list->used) d.Vftgts[d.Vfspec].curr = 0;
	d.filter_tgt->setCurrentIndex (d.Vftgts[d.Vfspec].curr);
}

void
GuiRoster::set_filter_tgt (int idx)
{
	d.Vftgts[d.Vfspec].curr = (idx > 0) ? idx : 0;
	//d.model->filter (d.Vfspec, d.Vftgts[d.Vfspec]);
	//d.hdr_roster->refresh();
	//d.bdy_roster->refresh();
}
#endif	/* EXPERIMENTAL */

void
GuiRoster::sort (GuiRosterView *who, int idx, Qt::SortOrder order)
{
	if (idx == MDLR_COLUMN_CDSTATUS) return;

	d.model->sort (idx, order);

	if (who != d.bdy_roster) d.bdy_roster->sort (idx, order);
	if (who != d.hdr_roster) d.hdr_roster->sort (idx, order);
}

void
GuiRoster::selected (GuiRosterView *who, const QModelIndex &index)
{
	if (who != d.hdr_roster) d.hdr_roster->select (index);
	if (who != d.bdy_roster) d.bdy_roster->select (index);
	d.hdr_roster->scrollTo (index);
	d.hdr_roster->refresh();
}