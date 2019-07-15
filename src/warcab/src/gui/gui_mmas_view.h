/** \file
 * The SPWaW war cabinet - GUI - min-max-average-spread view.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_MMAS_VIEW_H
#define	GUI_MMAS_VIEW_H	1

#include "gui_private.h"

class GuiMMASView	: public QTableView
{
	Q_OBJECT

public:
	GuiMMASView	(bool hdr, GuiMMAS *mmas, QWidget *P = 0);
	~GuiMMASView	(void);

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
	void	selected	(GuiMMASView *who, const QModelIndex &index);

public slots:
	void	scroll		(int d);
	void	wheel		(QWheelEvent *e);
	void	select		(const QModelIndex &index);
	void	currentChanged	(const QModelIndex &current, const QModelIndex &previous);

private:
	struct s_data {
		bool	ishdr;
		GuiMMAS	*parent;
		QFont	*font;
	} d;
};

#endif	/* GUI_MMAS_VIEW_H */
