/** \file
 * The SPWaW war cabinet - GUI - order of battle view.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_OOB_VIEW_H
#define	GUI_OOB_VIEW_H	1

#include "gui_private.h"

class GuiOobView	: public QTreeView
{
	Q_OBJECT

public:
	GuiOobView	(bool hdr, GuiOob *oob, QWidget *P = 0);
	~GuiOobView	(void);

	SL_ERROR	error_code;

public:
	void	reparented	(void);
	void	reload		(bool sort);
	void	resort		(void);
	void	refresh		(void);

public:
	void	wheelEvent		(QWheelEvent *e);
	void	mouseDoubleClickEvent	(QMouseEvent *event);

#if	EXPERIMENTAL
	void	contextMenuEvent	(QContextMenuEvent *event);
#endif	/* EXPERIMENTAL */

signals:
	void	scrolled	(int dx, int dy);
	void	wheeled		(QWheelEvent *e);
	void	sorted		(GuiOobView *who, int idx, Qt::SortOrder order);
	void	was_collapsed	(GuiOobView *who, const QModelIndex &index);
	void	was_expanded	(GuiOobView *who, const QModelIndex &index);
	void	wantscroll	(int d);
	void	selected	(GuiOobView *who, const QModelIndex &index);

public slots:
	void	scroll		(int d);
	void	wheel		(QWheelEvent *e);
	void	savesort	(int idx, Qt::SortOrder order);
	void	sort		(int idx, Qt::SortOrder order);
	void	collapsed	(const QModelIndex &index);
	void	expanded	(const QModelIndex &index);
	void	select		(const QModelIndex &index);
	void	currentChanged	(const QModelIndex &current, const QModelIndex &previous);

private:
	struct s_data {
		bool		ishdr;
		GuiOob		*parent;
		QFont		*font;

		int		sidx;
		Qt::SortOrder	sord;
	} d;
};

#endif	/* GUI_OOB_VIEW_H */
