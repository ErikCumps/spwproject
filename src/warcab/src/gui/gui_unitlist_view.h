/** \file
 * The SPWaW war cabinet - GUI - unit list view.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_UNITLIST_VIEW_H
#define	GUI_UNITLIST_VIEW_H	1

#include "gui_private.h"

class GuiUnitlistView	: public QListView
{
	Q_OBJECT

public:
	GuiUnitlistView		(GuiHistory *history, QWidget *P = 0);
	~GuiUnitlistView	(void);

	SL_ERROR	error_code;

public:
	void	reload		(void);
	void	refresh		(void);

signals:
	void	selected	(int idx);

public slots:
	void	select		(const QModelIndex &index);
	void	currentChanged	(const QModelIndex &current, const QModelIndex &previous);

private:
	struct s_data {
		GuiHistory	*parent;
		QFont		*font;
	} d;
};

#endif	/* GUI_UNITLIST_VIEW_H */
