/** \file
 * The SPWaW war cabinet - GUI - battle turn report - strategic map.
 *
 * Copyright (C) 2012-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_reports_bturn.h"
#include "gui_table_view.h"

GuiRptTrnSMap::GuiRptTrnSMap (QWidget *P)
	: QScrollArea (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	GUINEW (d.model, ModelTable (), ERR_GUI_SMAP_INIT_FAILED, "data model");

	//GUINEW (d.font, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_SMAP_INIT_FAILED, "font");

	GUINEW (d.frame, QFrame (this), ERR_GUI_SMAP_INIT_FAILED, "frame");
	GUINEW (d.layout, QGridLayout (d.frame), ERR_GUI_SMAP_INIT_FAILED, "layout");

	GUINEW (d.grid, QCheckBox ("show Grid", d.frame), ERR_GUI_SMAP_INIT_FAILED, "grid");
	d.grid->setCheckState (Qt::Unchecked);

	GUINEW (d.vichexes, QCheckBox ("show Victory hexes", d.frame), ERR_GUI_SMAP_INIT_FAILED, "vichexes");
	d.vichexes->setCheckState (Qt::Unchecked);

	GUINEW (d.influence, QCheckBox ("show Influence", d.frame), ERR_GUI_SMAP_INIT_FAILED, "influence");
	d.influence->setCheckState (Qt::Unchecked);

	GUINEW (d.frontline, QCheckBox ("show Frontline", d.frame), ERR_GUI_SMAP_INIT_FAILED, "frontline");
	d.frontline->setCheckState (Qt::Unchecked);

	GUINEW (d.zoom2x, QCheckBox ("Zoom 2X", d.frame), ERR_GUI_SMAP_INIT_FAILED, "zoom2x");
	d.zoom2x->setCheckState (Qt::Unchecked);

	GUINEW (d.save, QPushButton ("Save image", d.frame), ERR_GUI_SMAP_INIT_FAILED, "save");
	d.save->setAutoDefault (false);

	GUINEW (d.spacer1, QSpacerItem (1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum), ERR_GUI_SMAP_INIT_FAILED, "spacer1");

	d.layout->addWidget (d.grid,		0, 0, 1, 1);
	d.layout->addWidget (d.vichexes,	0, 1, 1, 1);
	d.layout->addWidget (d.influence,	0, 2, 1, 1);
	d.layout->addWidget (d.frontline,	0, 3, 1, 1);
	d.layout->addWidget (d.zoom2x,		0, 4, 1, 1);
	d.layout->addItem   (d.spacer1,		0, 5, 1, 1);
	d.layout->addWidget (d.save,		0, 6, 1, 1);

	GUINEW (d.smap, SmapWidget (d.model, d.frame), ERR_GUI_SMAP_INIT_FAILED, "strategic map");
	d.smap->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);

	GUINEW (d.scroller, QScrollArea (d.frame), ERR_GUI_SMAP_INIT_FAILED, "scroller");
	d.scroller->setWidget (d.smap);
	d.scroller->setPalette (*RES_color(RID_SMAP_BG));
	d.scroller->setAutoFillBackground (true);

	d.layout->addWidget (d.scroller,	1, 0, 1, 7);

	GUINEW (d.split, QSplitter (Qt::Horizontal, d.frame), ERR_GUI_SMAP_INIT_FAILED, "split");
	d.split->setChildrenCollapsible (false);

	GUINEW (d.hdr_table, GuiTableView (true, d.model, 2, d.split), ERR_GUI_SMAP_INIT_FAILED, "hdr_table");
	GUIERR (d.hdr_table, ERR_GUI_SMAP_INIT_FAILED);

	GUINEW (d.bdy_table, GuiTableView (false, d.model, 2, d.split), ERR_GUI_SMAP_INIT_FAILED, "bdy_table");
	GUIERR (d.bdy_table, ERR_GUI_SMAP_INIT_FAILED);

	d.layout->addWidget (d.split,		2, 0, 1, 7);

	setWidget(d.frame);
	setWidgetResizable (true);

	if (!connect (d.grid, SIGNAL(stateChanged(int)), SLOT (grid_change(int))))
		SET_GUICLS_ERROR (ERR_GUI_SMAP_INIT_FAILED, "failed to connect <grid:stateChanged> to <grid_change>");

	if (!connect (d.vichexes, SIGNAL(stateChanged(int)), SLOT (vichexes_change(int))))
		SET_GUICLS_ERROR (ERR_GUI_SMAP_INIT_FAILED, "failed to connect <vichexes:stateChanged> to <vichexes_change>");

	if (!connect (d.influence, SIGNAL(stateChanged(int)), SLOT (influence_change(int))))
		SET_GUICLS_ERROR (ERR_GUI_SMAP_INIT_FAILED, "failed to connect <influence:stateChanged> to <influence_change>");

	if (!connect (d.frontline, SIGNAL(stateChanged(int)), SLOT (frontline_change(int))))
		SET_GUICLS_ERROR (ERR_GUI_SMAP_INIT_FAILED, "failed to connect <frontline:stateChanged> to <frontline_change>");

	if (!connect (d.zoom2x, SIGNAL(stateChanged(int)), SLOT (zoom2x_change(int))))
		SET_GUICLS_ERROR (ERR_GUI_SMAP_INIT_FAILED, "failed to connect <zoom2x:stateChanged> to <zoom2x_change>");

	if (!connect (d.save, SIGNAL(clicked(bool)), SLOT (save_clicked(bool))))
		SET_GUICLS_ERROR (ERR_GUI_SMAP_INIT_FAILED, "failed to connect <save:clicked> to <save_clicked>");

	if (!connect (this, SIGNAL (cmpcurr(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpcurr(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_SMAP_INIT_FAILED, "failed to connect <cmpcurr> to <dossier:set_cmpcurr>");

	if (!connect (this, SIGNAL (cmpbase(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpbase(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_SMAP_INIT_FAILED, "failed to connect <cmpbase> to <dossier:set_cmpbase>");

	if (!connect (this, SIGNAL (askscroll()), d.smap, SLOT (askscroll())))
		SET_GUICLS_ERROR (ERR_GUI_SMAP_INIT_FAILED, "failed to connect <askscroll> to <smap:askscroll>");

	if (!connect (d.smap, SIGNAL (wouldscroll(int,int)), this, SLOT (scrollsmap(int,int))))
		SET_GUICLS_ERROR (ERR_GUI_SMAP_INIT_FAILED, "failed to connect <smap:wouldscroll> to <scrollsmap>");

	if (!connect (d.model, SIGNAL (model_updated(void)), d.hdr_table, SLOT(commit(void))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <model:model_updated> to <hdr_table:commit>");
	if (!connect (d.model, SIGNAL (def_updated(void)), d.hdr_table, SLOT(commit_def(void))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <model:def_updated> to <hdr_table:commit_def>");
	if (!connect (d.model, SIGNAL (dat_updated(void)), d.hdr_table, SLOT(commit_dat(void))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <model:dat_updated> to <hdr_table:commit_dat>");

	if (!connect (d.model, SIGNAL (model_updated(void)), d.bdy_table, SLOT(commit(void))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <model:model_updated> to <bdy_table:commit>");
	if (!connect (d.model, SIGNAL (def_updated(void)), d.bdy_table, SLOT(commit_def(void))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <model:def_updated> to <bdy_table:commit_def>");
	if (!connect (d.model, SIGNAL (dat_updated(void)), d.bdy_table, SLOT(commit_dat(void))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <model:dat_updated> to <bdy_table:commit_dat>");

	if (!connect (d.bdy_table->verticalScrollBar(), SIGNAL (valueChanged (int)), d.hdr_table, SLOT (scroll(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <bdy_table:scrolled> to <hdr_table:scroll>");

	if (!connect (d.hdr_table, SIGNAL (wantscroll(int)), d.bdy_table, SLOT (scroll(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <hdr_table:wantscroll> to <bdy_table:scroll>");

	if (!connect (d.hdr_table, SIGNAL (wheeled(QWheelEvent*)), d.bdy_table, SLOT (wheel(QWheelEvent*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <hdr_table:wheeled> to <bdy_table:wheel>");

	if (!connect (d.bdy_table, SIGNAL (selected(GuiTableView*,const QModelIndex&)), this, SLOT (selected(GuiTableView*,const QModelIndex&))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <bdy_table:selected> to <selected>");

	if (!connect (d.hdr_table, SIGNAL (selected(GuiTableView*,const QModelIndex&)), this, SLOT (selected(GuiTableView*,const QModelIndex&))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <hdr_table:selected> to <selected>");

	SET_GUICLS_NOERR;
}

GuiRptTrnSMap::~GuiRptTrnSMap (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.model;
}

void
GuiRptTrnSMap::resizeEvent (QResizeEvent * /*event*/)
{
	refresh();
}

void
GuiRptTrnSMap::grid_change (int state)
{
	d.Vgrid = (state != Qt::Unchecked);
	refresh();
}

void
GuiRptTrnSMap::vichexes_change (int state)
{
	d.Vvichexes = (state != Qt::Unchecked);
	refresh();
}

void
GuiRptTrnSMap::influence_change (int state)
{
	d.Vinfluence = (state != Qt::Unchecked);
	refresh();
}

void
GuiRptTrnSMap::frontline_change (int state)
{
	d.Vfrontline = (state != Qt::Unchecked);
	refresh();
}

void
GuiRptTrnSMap::zoom2x_change (int state)
{
	d.Vzoom2x = (state != Qt::Unchecked);
	refresh();
}

void
GuiRptTrnSMap::save_clicked (bool /*checked*/)
{
	d.smap->save_smap();
}

void
GuiRptTrnSMap::scrollsmap (int x, int y)
{
	d.scroller->ensureVisible (x, y);
}

void
GuiRptTrnSMap::selected (GuiTableView *who, const QModelIndex &index)
{
	if (who != d.hdr_table) d.hdr_table->select (index);
	if (who != d.bdy_table) d.bdy_table->select (index);
	d.hdr_table->scrollTo (index);
	d.hdr_table->refresh();
}

void
GuiRptTrnSMap::set_parent (GuiRptTrn *p)
{
	d.parent = p;
}

void
GuiRptTrnSMap::refresh (void)
{
	MDLD_TREE_ITEM	*item;
	bool		skip_data, skip_render;

	DBG_TRACE_FENTER;

	item = (d.parent != NULL) ? d.parent->current() : NULL;

	skip_data = !d.reftrack.changed (item);
	if (skip_data) goto skip_data_update;

	DBG_TRACE_UPDATE;

	if (!item) {
		d.smap->clear();
	} else {
		d.battle = item->parent;
		if (GUIVALCHANGED(battle)) d.smap->reset_cursor();
		d.smap->load (item->data.t);
	}

skip_data_update:
	skip_render  = !GUIVALCHANGED (Vgrid);
	skip_render &= !GUIVALCHANGED (Vvichexes);
	skip_render &= !GUIVALCHANGED (Vinfluence);
	skip_render &= !GUIVALCHANGED (Vfrontline);
	skip_render &= !GUIVALCHANGED (Vzoom2x);
	if (skip_render) goto skip_render_update;

	d.smap->enable_grid (d.Vgrid, false);
	d.smap->enable_victoryhexes (d.Vvichexes, false);
	d.smap->enable_influence (d.Vinfluence, false);
	d.smap->enable_frontline (d.Vfrontline, false);
	d.smap->set_zoom (d.Vzoom2x ? SmapWidget::ZOOM_2X : SmapWidget::ZOOM_1X, false);
	d.smap->trigger_repaint();

skip_render_update:
	if (isVisible()) {
		emit cmpcurr (NULL);
		emit cmpbase (NULL);
		emit askscroll();
	}

	/* Make sure layout is updated */
	d.smap->updateGeometry();
	d.scroller->updateGeometry();
	d.layout->update();

	DBG_TRACE_FLEAVE;
}
