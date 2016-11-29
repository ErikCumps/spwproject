/** \file
 * The SPWaW war cabinet - GUI - unit roster view.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_ROSTER_VIEW_H
#define	GUI_ROSTER_VIEW_H	1

#include "gui_private.h"

class GuiRosterView	: public QTableView
{
	Q_OBJECT

public:
	GuiRosterView	(bool hdr, GuiRoster *roster, QWidget *P = 0);
	~GuiRosterView	(void);

	SL_ERROR	error_code;

public:
	void	wheelEvent	(QWheelEvent *e);
#if	EXPERIMENTAL
	void	contextMenuEvent(QContextMenuEvent *event);
#endif	/* EXPERIMENTAL */

public:
	void	reload		(bool sort);
	void	resort		(void);
	void	refresh		(void);

signals:
	void	scrolled	(int dx, int dy);
	void	wheeled		(QWheelEvent *e);
	void	sorted		(GuiRosterView *who, int idx, Qt::SortOrder order);
	void	wantscroll	(int d);
	void	selected	(GuiRosterView *who, const QModelIndex &index);

public slots:
	void	scroll		(int d);
	void	wheel		(QWheelEvent *e);
	void	savesort	(int idx, Qt::SortOrder order);
	void	sort		(int idx, Qt::SortOrder order);
	void	select		(const QModelIndex &index);
	void	currentChanged	(const QModelIndex &current, const QModelIndex &previous);

private:
	struct s_data {
		bool		ishdr;
		GuiRoster	*parent;
		QFont		*font;

		int		sidx;
		Qt::SortOrder	sord;
	} d;
};

#endif	/* GUI_ROSTER_VIEW_H */