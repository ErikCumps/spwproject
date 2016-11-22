/** \file
 * The SPWaW war cabinet - GUI - order of battle widget.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_oob.h"
#include "gui_oob_view.h"
#include "model/model_oob.h"
#include "gui_reports_dossier.h"
#include "gui_reports_battle.h"
#include "gui_reports_bturn.h"

#ifdef	_DEBUG
//FIXME: #include "model/model_sanity.h"
#endif	/* _DEBUG */

GuiOob::GuiOob (QWidget *P)
	: QFrame (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	GUINEW (d.model, ModelOob (), ERR_GUI_REPORTS_OOB_INIT_FAILED, "data model");

#ifdef	_DEBUG
	//FIXME: ModelSanity *s = NULL;
	//FIXME: GUINEW (s, ModelSanity (d.model, this), ERR_GUI_REPORTS_OOB_INIT_FAILED, "data model sanity checker");
#endif	/* _DEBUG */

	GUINEW (d.font, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_REPORTS_OOB_INIT_FAILED, "font");

	GUINEW (d.layout, QGridLayout (this), ERR_GUI_REPORTS_OOB_INIT_FAILED, "layout");

	GUINEW (d.highlight, QComboBox (this), ERR_GUI_REPORTS_OOB_INIT_FAILED, "highlight");
	for (int h=MDLO_HILITE_NONE; h<=MDLO_HILITE_LIMIT; h++)
		d.highlight->addItem (QString ("Highlight: ") + QString (MDLO_HILITE_lookup ((MDLO_HILITE)h)));
	d.highlight->setEditable (false);

	GUINEW (d.dltsort, QCheckBox ("Sort on difference?", this), ERR_GUI_REPORTS_OOB_INIT_FAILED, "dltsort");
	d.dltsort->setCheckState (Qt::Unchecked);

	GUINEW (d.prevcmp, QCheckBox ("prevcmp?", this), ERR_GUI_REPORTS_OOB_INIT_FAILED, "prevcmp");
	d.prevcmp->setCheckState (Qt::Unchecked);

	GUINEW (d.autosort, QCheckBox ("Automatically resort?", this), ERR_GUI_REPORTS_OOB_INIT_FAILED, "autosort");
	d.autosort->setCheckState (Qt::Checked);

	GUINEW (d.split, QSplitter (Qt::Horizontal, this), ERR_GUI_REPORTS_OOB_INIT_FAILED, "split");
	d.split->setChildrenCollapsible (false);

	GUINEW (d.hdr_oob, GuiOobView (true, this, d.split), ERR_GUI_REPORTS_OOB_INIT_FAILED, "hdr_oob");
	GUIERR (d.hdr_oob, ERR_GUI_REPORTS_OOB_INIT_FAILED);

	GUINEW (d.bdy_oob, GuiOobView (false, this, d.split), ERR_GUI_REPORTS_OOB_INIT_FAILED, "bdy_oob");
	GUIERR (d.bdy_oob, ERR_GUI_REPORTS_OOB_INIT_FAILED);

	d.layout->addWidget (d.highlight,	0, 0, 1, 1);
	d.layout->addWidget (d.prevcmp,		0, 1, 1, 1);
	d.layout->addWidget (d.dltsort,		0, 2, 1, 1);
	d.layout->addWidget (d.autosort,	0, 3, 1, 1);
	d.layout->addWidget (d.split,		2, 0, 1, 4);

	if (!connect (d.dltsort, SIGNAL(stateChanged(int)), SLOT (dltsort_change(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <dltsort:stateChanged> to <dltsort_change>");

	if (!connect (d.prevcmp, SIGNAL(stateChanged(int)), SLOT (prevcmp_change(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <prevcmp:stateChanged> to <prevcmp_change>");

	if (!connect (d.autosort, SIGNAL(stateChanged(int)), SLOT (autosort_change(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <autosort:stateChanged> to <autosort_change>");

	if (!connect (d.highlight, SIGNAL (activated(int)), SLOT (set_highlight(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <highlight:activated> to <set_highlight>");

	if (!connect (d.bdy_oob->verticalScrollBar(), SIGNAL (valueChanged(int)), d.hdr_oob, SLOT (scroll(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <bdy_oob:scrolled> to <hdr_oob:scroll>");

	if (!connect (d.hdr_oob, SIGNAL (wantscroll(int)), d.bdy_oob, SLOT (scroll(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <hdr_oob:wantscroll> to <bdy_oob:scroll>");

	if (!connect (d.bdy_oob, SIGNAL (sorted(GuiOobView*,int,Qt::SortOrder)), this, SLOT (sort(GuiOobView*,int,Qt::SortOrder))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <bdy_oob:sorted> to <sort>");

	if (!connect (d.hdr_oob, SIGNAL (sorted(GuiOobView*,int,Qt::SortOrder)), this, SLOT (sort(GuiOobView*,int,Qt::SortOrder))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <hdr_oob:sorted> to <sort>");

	if (!connect (d.hdr_oob, SIGNAL (was_collapsed(GuiOobView *,const QModelIndex &)), SLOT (collapse(GuiOobView *,const QModelIndex &))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <hdr_oob:was_collapsed> to <collapse>");

	if (!connect (d.hdr_oob, SIGNAL (was_expanded(GuiOobView *,const QModelIndex &)), SLOT (expand(GuiOobView *,const QModelIndex &))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <hdr_oob:was_expanded> to <expand>");

	if (!connect (d.hdr_oob, SIGNAL (wheeled(QWheelEvent*)), d.bdy_oob, SLOT (wheel(QWheelEvent*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <hdr_oob:wheeled> to <bdy_oob:wheel>");

	if (!connect (this, SIGNAL (cmpcurr(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpcurr(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <cmpcurr> to <dossier:set_cmpcurr>");

	if (!connect (this, SIGNAL (cmpbase(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpbase(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <cmpbase> to <dossier:set_cmpbase>");

	if (!connect (d.bdy_oob, SIGNAL (selected(GuiOobView*,const QModelIndex&)), SLOT (selected(GuiOobView*,const QModelIndex&))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <bdy_oob:selected> to <selected>");

	if (!connect (d.hdr_oob, SIGNAL (selected(GuiOobView*,const QModelIndex&)), SLOT (selected(GuiOobView*,const QModelIndex&))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <hdr_oob:selected> to <selected>");

	d.pflag = d.cflag = true;
	d.Vautosort = true;

	SET_GUICLS_NOERR;
}

GuiOob::~GuiOob (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
	delete d.model;
}

void
GuiOob::set_parent (GuiRptDsr *parent, bool player)
{
	d.ptype	 = MDLD_TREE_DOSSIER;
	d.pptr.d = parent;
	d.pflag	 = player;
	d.cflag	 = true;

	d.prevcmp->setText ("Last battle only?");
	d.prevcmp->setHidden (false);

	d.hdr_oob->reparented();
	d.bdy_oob->reparented();
}

void
GuiOob::set_parent (GuiRptBtl *parent, bool player, bool core)
{
	d.ptype	 = MDLD_TREE_BATTLE;
	d.pptr.b = parent;
	d.pflag	 = player;
	d.cflag	 = core;

	d.prevcmp->setText ("-unused-");
	d.prevcmp->setHidden (true);

	d.hdr_oob->reparented();
	d.bdy_oob->reparented();
}

void
GuiOob::set_parent (GuiRptTrn *parent, bool player, bool core)
{
	d.ptype	 = MDLD_TREE_BTURN;
	d.pptr.t = parent;
	d.pflag	 = player;
	d.cflag	 = core;

	d.prevcmp->setText ("This turn only?");
	d.prevcmp->setHidden (false);

	d.hdr_oob->reparented();
	d.bdy_oob->reparented();
}

void
GuiOob::update (void)
{
	MDLD_TREE_ITEM	*item = NULL;
	bool		skip;
	QModelIndex	idx, pidx;

	d.psort = true;
	switch (d.ptype) {
		case MDLD_TREE_DOSSIER:
			item = d.pptr.d ? d.pptr.d->current() : NULL;
			if (item && d.pdata) d.psort = false;
			break;
		case MDLD_TREE_BATTLE:
			item = d.pptr.b ? d.pptr.b->current() : NULL;
			if (item && d.pdata) d.psort = (item->data.b != d.pdata->data.b);
			break;
		case MDLD_TREE_BTURN:
			item = d.pptr.t ? d.pptr.t->current() : NULL;
			if (item && d.pdata) d.psort = (item->data.t->battle != d.pdata->data.t->battle);
			break;
		default:
			break;
	}

	skip =  !d.reftrack.changed (item);
	skip &= !GUIVALCHANGED (Vdltsort);
	skip &= !GUIVALCHANGED (Vprevcmp);
	skip &= !GUIVALCHANGED (Vautosort);
	if (skip) goto skip_data_update;

	DBG_TRACE_UPDATE;

	d.pdata = item; d.pcurr = d.pbase = NULL;
	if (d.pdata) {
		idx = d.hdr_oob->currentIndex(); pidx = idx.parent();
		switch (d.ptype) {
			case MDLD_TREE_DOSSIER:
				if (!d.pdata->data.d) return;
				if (!d.pdata->clast) return;

				if (pidx != QModelIndex()) idx = pidx;

				if (d.Vprevcmp) {
					d.pcurr = d.pdata->clast;
					d.pbase = d.pcurr->prev ? d.pcurr->prev : d.pcurr;
				} else {
					d.pcurr = d.pdata->clast;
					d.pbase = d.pdata->cfirst;
				}
				d.model->load (d.pcurr->data.b, d.pbase->data.b, d.pflag, true);
				break;
			case MDLD_TREE_BATTLE:
				if (!d.pdata->data.b) return;

				d.pcurr = d.pdata;
				if (d.Vprevcmp && d.pdata->prev) {
					d.pbase = d.pdata->prev;
				} else {
					d.pbase = d.pdata;
				}
				d.model->load (d.pcurr->data.b, d.pbase->data.b, d.pflag, d.cflag);
				break;
			case MDLD_TREE_BTURN:
				if (!d.pdata->data.t) return;

				d.pcurr = d.pdata;
				if (d.Vprevcmp && d.pdata->prev) {
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
		d.hdr_oob->select (idx);
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
}

void
GuiOob::refresh (void)
{
	DBG_TRACE_FENTER;

	update();
	d.hdr_oob->reload (d.psort || d.Vautosort);
	d.bdy_oob->reload (d.psort || d.Vautosort);

	d.psort = false;

	DBG_TRACE_FLEAVE;
}

void
GuiOob::dltsort_change (int state)
{
	d.Vdltsort = (state != Qt::Unchecked);

	d.model->set_dltsort (d.Vdltsort);
	if (d.Vautosort) {
		d.model->resort();
		refresh();
	}
}

void
GuiOob::prevcmp_change (int state)
{
	d.Vprevcmp = (state != Qt::Unchecked);

	update(); d.psort = false;
	d.hdr_oob->reload (d.Vautosort); d.hdr_oob->reparented();
	d.bdy_oob->reload (d.Vautosort); d.bdy_oob->reparented();
}

void
GuiOob::autosort_change (int state)
{
	d.Vautosort = (state != Qt::Unchecked);

	if (d.Vautosort) {
		refresh();
		d.hdr_oob->reload(false);
		d.bdy_oob->reload(false);
	}
}

void
GuiOob::set_highlight (int idx)
{
	d.model->highlight ((MDLO_HILITE)idx);
	d.hdr_oob->refresh();
	d.bdy_oob->refresh();
}

void
GuiOob::sort (GuiOobView *who, int idx, Qt::SortOrder order)
{
	d.model->sort (idx, order);

	if (who != d.bdy_oob) d.bdy_oob->sort (idx, order);
	if (who != d.hdr_oob) d.hdr_oob->sort (idx, order);
}

void
GuiOob::collapse (GuiOobView *who, const QModelIndex &index)
{
	d.model->set_expanded (index, false);
	if (who != d.bdy_oob) d.bdy_oob->collapse (index);
	if (who != d.hdr_oob) d.hdr_oob->collapse (index);
}

void
GuiOob::expand (GuiOobView *who, const QModelIndex &index)
{
	d.model->set_expanded (index, true);
	if (who != d.bdy_oob) d.bdy_oob->expand (index);
	if (who != d.hdr_oob) d.hdr_oob->expand (index);
}

void
GuiOob::selected (GuiOobView *who, const QModelIndex &index)
{
	if (who != d.hdr_oob) d.hdr_oob->select (index);
	if (who != d.bdy_oob) d.bdy_oob->select (index);
	d.hdr_oob->scrollTo (index);
	d.hdr_oob->refresh();
}
