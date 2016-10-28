/** \file
 * The SPWaW war cabinet - GUI - table view.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_TABLE_VIEW_H
#define	GUI_TABLE_VIEW_H	1

#include "gui_private.h"
#include "model/model_table.h"

class GuiTableView	: public QTableView
{
	Q_OBJECT

public:
	GuiTableView	(bool hdr, ModelTable *model, QWidget *P = 0);
	~GuiTableView	(void);

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
	void	selected	(GuiTableView *who, const QModelIndex &index);

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
		bool			hdr;
		ModelTable		*model;
		QFont			*font;
	} d;
};

#endif	/* GUI_TABLE_VIEW_H */
