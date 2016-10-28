/** \file
 * The SPWaW war cabinet - GUI - unit history view.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_HISTORY_VIEW_H
#define	GUI_HISTORY_VIEW_H	1

#include "gui_private.h"

class GuiHistoryView	: public QTableView
{
	Q_OBJECT

public:
	GuiHistoryView	(bool hdr, GuiHistory *history, QWidget *P = 0);
	~GuiHistoryView	(void);

	SL_ERROR	error_code;

public:
	void	reload		(void);
	void	refresh		(void);

public:
	void	wheelEvent	(QWheelEvent *e);

signals:
	void	scrolled	(int dx, int dy);
	void	wheeled		(QWheelEvent *e);
	void	wantscroll	(int d);
	void	selected	(GuiHistoryView *who, const QModelIndex &index);

public slots:
	void	scroll		(int d);
	void	wheel		(QWheelEvent *e);
	void	select		(const QModelIndex &index);
	void	currentChanged	(const QModelIndex &current, const QModelIndex &previous);
	void	battleview	(bool set);

private:
	struct s_data {
		bool		ishdr;
		GuiHistory	*parent;
		QFont		*font;
	} d;
};

#endif	/* GUI_HISTORY_VIEW_H */
