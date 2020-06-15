/** \file
 * The SPWaW war cabinet - GUI - min-max-average-spread widget.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_mmas.h"
#include "gui_mmas_view.h"
#include "model/model_mmas.h"
#include "gui_reports_dossier.h"
#include "gui_reports_battle.h"

#define	PLOT_MARGIN	5

static MDLMMAS_TYPE typemap_dossier[] = {
	MDLMMAS_TYPE_EXP, MDLMMAS_TYPE_MOR,
	MDLMMAS_TYPE_RAL, MDLMMAS_TYPE_INF, MDLMMAS_TYPE_ARM, MDLMMAS_TYPE_ART,
	MDLMMAS_TYPE_RDY, MDLMMAS_TYPE_KILLS, MDLMMAS_TYPE_COST, MDLMMAS_TYPE_SPEED
};

static MDLMMAS_TYPE typemap_battle[] = {
	MDLMMAS_TYPE_SUP,
	MDLMMAS_TYPE_RDY, MDLMMAS_TYPE_KILLS, MDLMMAS_TYPE_SPEED
};

GuiMMAS::GuiMMAS (QWidget *P)
	: QFrame (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	GUINEW (d.model, ModelMMAS(), ERR_GUI_REPORTS_MMAS_INIT_FAILED, "MMAS data model");

	GUINEW (d.font, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_REPORTS_MMAS_INIT_FAILED, "font");

	GUINEW (d.layout, QGridLayout (this), ERR_GUI_REPORTS_MMAS_INIT_FAILED, "layout");

	GUINEW (d.intel, QLabel (this), ERR_GUI_REPORTS_INIT_FAILED, "intel");
	d.intel->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.intel->setWordWrap (true);
	d.intel->setSizePolicy (QSizePolicy (QSizePolicy::Minimum, QSizePolicy::Fixed));

	GUINEW (d.type, QComboBox (this), ERR_GUI_REPORTS_MMAS_INIT_FAILED, "type");
	d.type->setEditable (false);

	GUINEW (d.filter, QComboBox (this), ERR_GUI_REPORTS_MMAS_INIT_FAILED, "filter");
	d.filter->setEditable (false);

	GUINEW (d.target, QComboBox (this), ERR_GUI_REPORTS_MMAS_INIT_FAILED, "target");
	d.target->setEditable (false);

	GUINEW (d.timeline, QCheckBox ("Time-based axis?", this), ERR_GUI_REPORTS_MMAS_INIT_FAILED, "timeline");
	d.timeline->setCheckState (Qt::Unchecked);
	d.Vtimeline = false;

	GUINEW (d.prevcmp, QCheckBox ("prevcmp?", this), ERR_GUI_REPORTS_MMAS_INIT_FAILED, "prevcmp");
	d.prevcmp->setCheckState (Qt::Unchecked);

	GUINEW (d.plot, PlotMMAS (d.model, this), ERR_GUI_REPORTS_MMAS_INIT_FAILED, "plot");
	GUIERR (d.plot, ERR_GUI_REPORTS_MMAS_INIT_FAILED);
	d.plot->setMargin (PLOT_MARGIN);

	GUINEW (d.scroller, QScrollArea (this), ERR_GUI_REPORTS_MMAS_INIT_FAILED, "scroller");
	d.scroller->setWidget (d.plot);
	d.scroller->setWidgetResizable (true);

	GUINEW (d.split, QSplitter (Qt::Horizontal, this), ERR_GUI_REPORTS_MMAS_INIT_FAILED, "split");
	d.split->setChildrenCollapsible (false);

	GUINEW (d.hdr_mmas, GuiMMASView (true, this, d.split), ERR_GUI_REPORTS_MMAS_INIT_FAILED, "hdr_mmas");
	GUIERR (d.hdr_mmas, ERR_GUI_REPORTS_MMAS_INIT_FAILED);

	GUINEW (d.bdy_mmas, GuiMMASView (false, this, d.split), ERR_GUI_REPORTS_MMAS_INIT_FAILED, "bdy_mmas");
	GUIERR (d.bdy_mmas, ERR_GUI_REPORTS_MMAS_INIT_FAILED);

	d.layout->addWidget (d.intel,		0, 0, 1, 3);
	d.layout->addWidget (d.type,		1, 0, 1, 2);
	d.layout->addWidget (d.filter,		1, 2, 1, 1);
	d.layout->addWidget (d.timeline,	2, 0, 1, 1);
	d.layout->addWidget (d.prevcmp,		2, 1, 1, 1);
	d.layout->addWidget (d.target,		2, 2, 1, 1);
	d.layout->addWidget (d.scroller,	3, 0, 1, 3);
	d.layout->addWidget (d.split,		4, 0, 1, 3);

	if (!connect (d.type, SIGNAL (activated(int)), SLOT (set_type(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_MMAS_INIT_FAILED, "failed to connect <type:activated> to <set_type>");

	if (!connect (d.filter, SIGNAL (activated(int)), SLOT (set_filter(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_MMAS_INIT_FAILED, "failed to connect <filter:activated> to <set_filter>");

	if (!connect (d.target, SIGNAL (activated(int)), SLOT (set_target(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_MMAS_INIT_FAILED, "failed to connect <target:activated> to <set_target>");

	if (!connect (d.timeline, SIGNAL(stateChanged(int)), SLOT (timeline_change(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <timeline:stateChanged> to <timeline_change>");

	if (!connect (d.prevcmp, SIGNAL(stateChanged(int)), SLOT (prevcmp_change(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_MMAS_INIT_FAILED, "failed to connect <prevcmp:stateChanged> to <prevcmp_change>");

	if (!connect (d.bdy_mmas->verticalScrollBar(), SIGNAL (valueChanged (int)), d.hdr_mmas, SLOT (scroll(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_MMAS_INIT_FAILED, "failed to connect <bdy_mmas:scrolled> to <hdr_mmas:scroll>");

	if (!connect (d.hdr_mmas, SIGNAL (wantscroll(int)), d.bdy_mmas, SLOT (scroll(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_MMAS_INIT_FAILED, "failed to connect <hdr_mmas:wantscroll> to <bdy_mmas:scroll>");

	if (!connect (d.hdr_mmas, SIGNAL (wheeled(QWheelEvent*)), d.bdy_mmas, SLOT (wheel(QWheelEvent*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_MMAS_INIT_FAILED, "failed to connect <hdr_mmas:wheeled> to <bdy_mmas:wheel>");

	if (!connect (this, SIGNAL (cmpcurr(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpcurr(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_MMAS_INIT_FAILED, "failed to connect <cmpcurr> to <dossier:set_cmpcurr>");

	if (!connect (this, SIGNAL (cmpbase(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpbase(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_MMAS_INIT_FAILED, "failed to connect <cmpbase> to <dossier:set_cmpbase>");

	if (!connect (d.bdy_mmas, SIGNAL (selected(GuiMMASView*,const QModelIndex&)), this, SLOT (selected(GuiMMASView*,const QModelIndex&))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_MMAS_INIT_FAILED, "failed to connect <bdy_mmas:selected> to <selected>");

	if (!connect (d.hdr_mmas, SIGNAL (selected(GuiMMASView*,const QModelIndex&)), this, SLOT (selected(GuiMMASView*,const QModelIndex&))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_MMAS_INIT_FAILED, "failed to connect <hdr_mmas:selected> to <selected>");

	if (!connect (GUI_WIN, SIGNAL (selected_intel_mode(INTEL_MODE)), this, SLOT (intel_mode_set(INTEL_MODE))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <mainwindow:selected_intel_mode> to <intel_mode_set>");

	if (!connect (GUI_WIN, SIGNAL (selected_intel_mode(INTEL_MODE)), d.model, SLOT (intel_mode_set(INTEL_MODE))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <mainwindow:selected_intel_mode> to <model:intel_mode_set>");

	if (!connect (GUI_WIN, SIGNAL (selected_intel_mode(INTEL_MODE)), d.plot, SLOT (intel_mode_set(INTEL_MODE))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <mainwindow:selected_intel_mode> to <plot:intel_mode_set>");

	d.filters = d.model->filters();
	if (d.filters) {
		SL_SAFE_CALLOC (d.Vtargets, d.filters->cnt, sizeof (GUI_FILTER_TARGET));
		for (int i=0; i<d.filters->cnt; i++) d.filter->addItem (QString ("Filter: ") + QString (d.filters->list[i].name));
	} else {
		d.Vtargets = NULL;
		d.filter->addItem (QString ("Show all"));
	}

	d.pflag = d.cflag = true;
	d.Vtype = MDLMMAS_TYPE_EXP;

	SET_GUICLS_NOERR;
}

GuiMMAS::~GuiMMAS (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
	delete d.model;
	SL_SAFE_FREE (d.Vtargets);
}

void
GuiMMAS::set_parent (GuiRptDsr *parent, bool player)
{
	d.ptype	 = MDLD_TREE_DOSSIER;
	d.pptr.d = parent;
	d.pflag	 = player;
	d.cflag	 = true;

	d.prevcmp->setText ("Compare with previous battle?");
	d.prevcmp->setHidden (false);

	d.type->clear();

	d.typemap.map = typemap_dossier;
	d.typemap.cnt = sizeof (typemap_dossier)/sizeof (MDLMMAS_TYPE);
	for (int i=0; i<d.typemap.cnt; i++)
		d.type->addItem (QString ("Type: ") + QString (MDLMMAS_TYPE_lookup (d.typemap.map[i])));

	set_type (0);

	d.timeline->setHidden (false);
}

void
GuiMMAS::set_parent (GuiRptBtl *parent, bool player, bool core)
{
	d.ptype	 = MDLD_TREE_BATTLE;
	d.pptr.b = parent;
	d.pflag	 = player;
	d.cflag	 = core;

	d.prevcmp->setText ("Compare with previous turn?");
	d.prevcmp->setHidden (false);

	d.type->clear();

	d.typemap.map = typemap_battle;
	d.typemap.cnt = sizeof (typemap_battle)/sizeof (MDLMMAS_TYPE);
	for (int i=0; i<d.typemap.cnt; i++)
		d.type->addItem (QString ("Type: ") + QString (MDLMMAS_TYPE_lookup (d.typemap.map[i])));

	set_type (0);

	d.timeline->setHidden (true);
}

void
GuiMMAS::set_parent (GuiRptTrn *parent, bool player)
{
	d.ptype	 = MDLD_TREE_BTURN;
	d.pptr.t = parent;
	d.pflag	 = player;
	d.cflag	 = false;

	d.prevcmp->setText ("-unused-");
	d.prevcmp->setHidden (true);

	d.type->clear();

	d.typemap.map = NULL;
	d.typemap.cnt = 0;

	d.timeline->setHidden (true);
}

void
GuiMMAS::update (void)
{
	MDLD_TREE_ITEM	*item = NULL;
	bool		skip;
	QModelIndex	idx;
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
	skip &= !GUIVALCHANGED (Vtimeline);
	skip &= !GUIVALCHANGED (Vprevcmp);
	skip &= !GUIVALCHANGED(Vtype);
	if (skip) goto skip_data_update;

	DBG_TRACE_UPDATE;

	if (item && d.ptype == MDLD_TREE_DOSSIER) {
		switch (item->data.d->gametype) {
			case SPWAW_GAME_TYPE_WINSPWW2:
				d.timeline->setCheckState (Qt::Unchecked);
				d.timeline->setHidden (true);
				break;
			default:
				d.timeline->setHidden (false);
				break;
		}
	}

	d.plot->set_axistype (d.Vtimeline ? AXIS_TIME : AXIS_INDEX);

	d.pdata = item;
	if (d.pdata) {
		idx = d.hdr_mmas->currentIndex();
		switch (d.ptype) {
			case MDLD_TREE_DOSSIER:
				d.model->load (item->data.d, d.Vtype, d.pflag, d.Vprevcmp, d.Vintel_mode);
				break;
			case MDLD_TREE_BATTLE:
				d.model->load (item->data.b, d.Vtype, d.pflag, d.cflag, d.Vprevcmp, d.Vintel_mode);
				break;
			default:
				break;
		}
		d.hdr_mmas->select (idx);
	} else {
		d.model->clear();
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
GuiMMAS::refresh (void)
{
	DBG_TRACE_FENTER;

	update();
	d.plot->refresh();
	d.hdr_mmas->reload();
	d.bdy_mmas->reload();

	/* Force the QScrollArea to resize the plot widget */
	d.scroller->setWidgetResizable(false); d.scroller->setWidgetResizable(true);

	DBG_TRACE_FLEAVE;
}

void
GuiMMAS::set_type (int idx)
{
	if ((idx >= 0) && (idx < d.typemap.cnt))
		d.Vtype = d.typemap.map[idx];
	else
		d.Vtype = MDLMMAS_TYPE_EXP;

	refresh();
}

void
GuiMMAS::set_filter (int idx)
{
	MDL_MF_TGTLST	*list;

	if (!d.filters) return;

	d.Vfilter = idx; if ((d.Vfilter < 0) || (d.Vfilter >= d.filters->cnt)) d.Vfilter = 0;
	d.target->clear();

	list = d.Vtargets[d.Vfilter].list = d.model->targets (d.filters->list[d.Vfilter].id);
	if (list) {
		for (int i=0; i<list->cnt; i++) {
			d.target->addItem (QString ("Target: ") + QString (list->list[i].name));
		}
		if (d.Vtargets[d.Vfilter].curr >= list->cnt) d.Vtargets[d.Vfilter].curr = 0;
	} else {
		d.Vtargets[d.Vfilter].curr = 0;
	}
	d.target->setCurrentIndex (d.Vtargets[d.Vfilter].curr);
	set_target (d.Vtargets[d.Vfilter].curr);
}

void
GuiMMAS::set_target (int idx)
{
	MDL_MF_TGTLST	*list;
	MDL_MFID	fid = 0;
	MDL_MF_TGTID	tid = 0;

	if (!d.filters || !d.Vtargets) return;

	list = d.Vtargets[d.Vfilter].list;
	if (list) {
		d.Vtargets[d.Vfilter].curr = (idx > 0) ? idx : 0;
		fid = d.filters->list[d.Vfilter].id;
		tid = list->list[d.Vtargets[d.Vfilter].curr].id;
	}

	d.model->filter (fid, tid);
	d.plot->refresh();
	d.hdr_mmas->refresh();
	d.bdy_mmas->refresh();
}

void
GuiMMAS::timeline_change (int state)
{
	d.Vtimeline = (state != Qt::Unchecked);
	refresh();
}

void
GuiMMAS::prevcmp_change (int state)
{
	d.Vprevcmp = (state != Qt::Unchecked);
	refresh();
}

void
GuiMMAS::selected (GuiMMASView *who, const QModelIndex &index)
{
	if (who != d.hdr_mmas) d.hdr_mmas->select (index);
	if (who != d.bdy_mmas) d.bdy_mmas->select (index);
	d.hdr_mmas->scrollTo (index);
	d.hdr_mmas->refresh();
}

void
GuiMMAS::intel_mode_set (INTEL_MODE mode)
{
	d.Vintel_mode = mode;

	intelmode2label (d.Vintel_mode, d.pflag, d.intel);
}