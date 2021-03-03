/** \file
 * The SPWaW war cabinet - GUI - dossier report - graphs.
 *
 * Copyright (C) 2005-2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_reports_dossier_graphs.h"
#include "gui_reports_dossier.h"
#include "gui_plot_table_view.h"
#include "../plot/plot_table.h"

#define	PLOT_MARGIN	5

GuiRptDsrPlt::GuiRptDsrPlt (QWidget *P)
	: QFrame (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Fix the QFrame look */
	setPalette (GUI_APP->palette());
	setAutoFillBackground (true);
	setFrameStyle (QFrame::Panel|QFrame::Plain);

	GUINEW (d.model, ModelPlotTable (), ERR_GUI_REPORTS_INIT_FAILED, "data model");

	graphs_setup();

	GUINEW (d.layout, QGridLayout (this), ERR_GUI_REPORTS_INIT_FAILED, "layout");

	GUINEW (d.graph_type, QComboBox (this), ERR_GUI_REPORTS_INIT_FAILED, "graph_type");
	for (int i=0; i<d.cnt; i++) {
		d.graph_type->addItem (QString("Battle graph: ") + QString (d.list[i]->name));
	}
	d.graph_type->setEditable (false);

	GUINEW (d.plot_type, QComboBox (this), ERR_GUI_REPORTS_INIT_FAILED, "plot_type");
	d.plot_type->addItem (QString ("Plot type: LINE"));
	d.plot_type->addItem (QString ("Plot type: AREA"));
	d.plot_type->addItem (QString ("Plot type: BAR"));
	d.plot_type->setEditable (false);

	GUINEW (d.plot_stacked, QCheckBox ("Stacked plot?", this), ERR_GUI_REPORTS_INIT_FAILED, "plot_stacked");
	d.plot_stacked->setCheckState (Qt::Unchecked);

	GUINEW (d.axis_timeline, QCheckBox ("Time-based axis?", this), ERR_GUI_REPORTS_INIT_FAILED, "axis_timeline");
	d.axis_timeline->setCheckState (Qt::Checked);

	GUINEW (d.plot, PlotTable (d.model, this), ERR_GUI_REPORTS_INIT_FAILED, "plot");
	GUIERR (d.plot, ERR_GUI_REPORTS_INIT_FAILED);
	d.plot->setMargin (PLOT_MARGIN);

	GUINEW (d.scroller, QScrollArea (this), ERR_GUI_REPORTS_MMAS_INIT_FAILED, "scroller");
	d.scroller->setWidget (d.plot);
	d.scroller->setWidgetResizable (true);

	GUINEW (d.split, QSplitter (Qt::Horizontal, this), ERR_GUI_REPORTS_INIT_FAILED, "split");
	d.split->setChildrenCollapsible (false);

	GUINEW (d.hdr_table, GuiPlotTableView (true, d.model, d.split), ERR_GUI_REPORTS_INIT_FAILED, "hdr_table");
	GUIERR (d.hdr_table, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.bdy_table, GuiPlotTableView (false, d.model, d.split), ERR_GUI_REPORTS_INIT_FAILED, "bdy_table");
	GUIERR (d.bdy_table, ERR_GUI_REPORTS_INIT_FAILED);

	d.layout->addWidget (d.graph_type,	0, 0, 1, 3);
	d.layout->addWidget (d.axis_timeline,	1, 0, 1, 1);
	d.layout->addWidget (d.plot_type,	1, 1, 1, 1);
	d.layout->addWidget (d.plot_stacked,	1, 2, 1, 1);
	d.layout->addWidget (d.scroller,	2, 0, 1, 3);
	d.layout->addWidget (d.split,		3, 0, 1, 3);

	if (!connect (d.model, SIGNAL (model_updated(void)), d.plot, SLOT(commit(void))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <model:model_updated> to <plot:commit>");
	if (!connect (d.model, SIGNAL (def_updated(void)), d.plot, SLOT(commit_def(void))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <model:def_updated> to <plot:commit_def>");
	if (!connect (d.model, SIGNAL (dat_updated(void)), d.plot, SLOT(commit_dat(void))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect <model:dat_updated> to <plot:commit_dat>");

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

	if (!connect (d.bdy_table, SIGNAL (selected(GuiPlotTableView*,const QModelIndex&)), this, SLOT (selected(GuiPlotTableView*,const QModelIndex&))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <bdy_table:selected> to <selected>");

	if (!connect (d.hdr_table, SIGNAL (selected(GuiPlotTableView*,const QModelIndex&)), this, SLOT (selected(GuiPlotTableView*,const QModelIndex&))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <hdr_table:selected> to <selected>");

	if (!connect (d.graph_type, SIGNAL (activated (int)), this, SLOT(select_graph (int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <graph_type:activated> to <select_graph>");

	if (!connect (d.plot_type, SIGNAL (activated(int)), SLOT (set_type(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <plot_type:activated> to <set_type>");

	if (!connect (d.plot_stacked, SIGNAL(stateChanged(int)), SLOT (stacked_change(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <plot_stacked:stateChanged> to <stacked_change>");

	if (!connect (d.axis_timeline, SIGNAL(stateChanged(int)), SLOT (timeline_change(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <axis_timeline:stateChanged> to <timeline_change>");

	SET_GUICLS_NOERR;
}

GuiRptDsrPlt::~GuiRptDsrPlt (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	graphs_destroy();
	delete d.model;
}

void
GuiRptDsrPlt::graphs_setup (void)
{
	d.cnt = GUI_BATTLE_GRAPH_TYPE_LIMIT+1;

	SL_SAFE_CALLOC (d.list, d.cnt, sizeof (GuiBattleGraph*));

	for (int i=0; i<d.cnt; i++) {
		d.list[i] = GUI_create_battle_graph((GUI_BATTLE_GRAPH_TYPE)i);
	}

	d.idx = 0;
	d.ptr = d.list[0];
}

void
GuiRptDsrPlt::graphs_destroy (void)
{
	for (int i=0; i<d.cnt; i++) {
		delete d.list[i];
	}
	SL_SAFE_FREE (d.list);
}

void
GuiRptDsrPlt::set_parent (GuiRptDsr *parent)
{
	d.parent = parent;
}

#define	DATE2TIME(d_,t_)	PLOT_date2timeline (d_, t_)

void
GuiRptDsrPlt::refresh (bool forced)
{
	MDLD_TREE_ITEM	*item;
	SPWAW_DOSSIER	*ptr = NULL;
	bool		skip;
	int		cnt;
	MDLPT_DATA	data;
	int		i, j;

	item = (d.parent != NULL) ? d.parent->current() : NULL;

	DBG_TRACE_FENTER;

	skip =  !d.reftrack.changed (item);
	skip &= !GUIVALCHANGED (idx);
	skip &= !d.ptr->changed();
	skip &= !forced;
	if (skip || !item) goto skip_data_update;

	DBG_TRACE_UPDATE;

	ptr = item->data.d;

	switch (ptr->gametype) {
		case SPWAW_GAME_TYPE_WINSPWW2:
		case SPWAW_GAME_TYPE_WINSPMBT:
			d.axis_timeline->setCheckState (Qt::Unchecked);
			d.axis_timeline->setHidden (true);
			break;
		default:
			d.axis_timeline->setHidden (false);
			break;
	}

	d.model->setup (&d.ptr->definition);
	cnt = d.ptr->definition.col_cnt;

	if (cnt) {
		memset (&data, 0, sizeof (data));

		MDLPT_alloc_data (data, ptr->bcnt, cnt);

		for (i=0; i<data.row_cnt; i++) {
			data.row[i].data[0].type = MDLPT_DATA_BDATE;
			data.row[i].data[0].u.bdate = ptr->blist[i]->snap->game.battle.data.bdate;

			for (j=0; j<(data.col_cnt-1); j++) {
				data.row[i].data[j+1].type = MDLPT_DATA_INT;
				data.row[i].data[j+1].u.i = d.ptr->battle_value (ptr->blist[i], j);
			}
		}
		d.model->load (&data);
		MDLPT_free_data (data);
	}

	d.model->commit();

	// Update GUI state elements
	d.plot_type->setCurrentIndex ((int)d.ptr->definition.plot_type);
	d.plot_stacked->setCheckState (d.ptr->definition.plot_stacked ? Qt::Checked : Qt::Unchecked);
	if (!d.axis_timeline->isHidden()) {
		d.axis_timeline->setCheckState ((d.ptr->definition.axis_type == AXIS_TIME) ? Qt::Checked : Qt::Unchecked);
	}

	/* Force the QScrollArea to resize the plot widget */
	d.scroller->setWidgetResizable(false); d.scroller->setWidgetResizable(true);

skip_data_update:
	DBG_TRACE_FLEAVE;
}

void
GuiRptDsrPlt::select_graph (int idx)
{
	d.idx = idx;
	d.ptr = d.list[d.idx];
	
	refresh();
}

void
GuiRptDsrPlt::selected (GuiPlotTableView *who, const QModelIndex &index)
{
	if (who != d.hdr_table) d.hdr_table->select (index);
	if (who != d.bdy_table) d.bdy_table->select (index);
	d.hdr_table->scrollTo (index);
	d.hdr_table->refresh();
}

void
GuiRptDsrPlt::set_type (int idx)
{
	if ((d.idx < 0) || (d.idx >= d.cnt)) return;

	d.ptr->set_plot_type ((PLOT_TYPE)idx);
	
	refresh();
}

void
GuiRptDsrPlt::stacked_change (int state)
{
	d.ptr->set_plot_stacked (state == Qt::Checked);

	refresh();
}

void
GuiRptDsrPlt::timeline_change (int state)
{
	AXIS_TYPE	type = (state == Qt::Checked) ? AXIS_TIME : AXIS_INDEX;

	d.ptr->set_axis_type (type);

	refresh();
}
