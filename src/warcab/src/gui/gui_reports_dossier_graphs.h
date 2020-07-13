/** \file
 * The SPWaW war cabinet - GUI - dossier report - graphs.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORTS_DOSSIER_GRAPHS_H
#define GUI_REPORTS_DOSSIER_GRAPHS_H	1

#include "gui_private.h"
#include "plot/plot.h"
#include "util/util_mdltree_reftrack.h"
#include "util/util_valtrack.h"
#include "gui_battle_graph.h"
#include "gui_plot_table_view.h"

class GuiRptDsrPlt	: public QScrollArea
{
	Q_OBJECT

public:
	GuiRptDsrPlt	(QWidget *P = 0);
	~GuiRptDsrPlt	(void);

	SL_ERROR	error_code;

public:
	void	set_parent	(GuiRptDsr *parent);
	void	refresh		(bool forced = false);

public slots:
	void	selected	(GuiPlotTableView *who, const QModelIndex &index);
	void	set_type	(int idx);
	void	stacked_change	(int state);
	void	timeline_change	(int state);

private:
	struct s_data {
		ModelPlotTable		*model;
		
		QFrame			*frame;
		QGridLayout		*layout;
		QComboBox		*graph_type;
		QComboBox		*plot_type;
		QCheckBox		*plot_stacked;
		QCheckBox		*axis_timeline;
		PlotTable		*plot;
		QSplitter		*split;
		GuiPlotTableView	*hdr_table;
		GuiPlotTableView	*bdy_table;

		GuiRptDsr		*parent;

		int			cnt;
		GuiBattleGraph		**list;
		GuiBattleGraph		*ptr;

		UtilMdlTreeReftrack	reftrack;
		GUIVALTRACK (int, idx);
	} d;

private slots:
	void	select_graph	(int idx);

private:
	void	graphs_setup	(void);
	void	graphs_destroy	(void);
};

#endif	/* GUI_REPORTS_DOSSIER_GRAPHS_H */
