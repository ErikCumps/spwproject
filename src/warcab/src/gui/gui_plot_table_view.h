/** \file
 * The SPWaW war cabinet - GUI - plot table view.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_PLOT_TABLE_VIEW_H
#define	GUI_PLOT_TABLE_VIEW_H	1

#include "gui_private.h"
#include "model/model_plot_table.h"

class GuiPlotTableView	: public QTableView
{
	Q_OBJECT

public:
	GuiPlotTableView	(bool hdr, ModelPlotTable *model, QWidget *P = 0);
	~GuiPlotTableView	(void);

	SL_ERROR	error_code;

public:
	void	setup		(void);
	void	reload		(void);
	void	refresh		(void);

public:
	void	wheelEvent	(QWheelEvent *e);

signals:
	void	scrolled	(int dx, int dy);
	void	wheeled		(QWheelEvent *e);
	void	wantscroll	(int d);
	void	selected	(GuiPlotTableView *who, const QModelIndex &index);

public slots:
	void	scroll		(int d);
	void	wheel		(QWheelEvent *e);
	void	select		(const QModelIndex &index);
	void	currentChanged	(const QModelIndex &current, const QModelIndex &previous);

	void	commit		(void);
	void	commit_def	(void);
	void	commit_dat	(void);

private:
	struct s_data {
		bool		hdr;
		ModelPlotTable	*model;
		QFont		*font;
	} d;
};

#endif	/* GUI_PLOT_TABLE_VIEW_H */
