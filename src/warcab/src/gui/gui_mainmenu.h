/** \file
 * The SPWaW war cabinet - GUI - main menu.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_MAINMENU_H
#define GUI_MAINMENU_H	1

#include "gui_private.h"

class GuiMainMenu	:public QObject
{
	Q_OBJECT

public:
	GuiMainMenu	(QFont &font);
	~GuiMainMenu	(void);

	SL_ERROR	error_code;

public slots:
	virtual void MenuStatus (QAction *action);

private:
	struct s_data {
		QMenuBar	*menu;
		QMenu		*app;
		QMenu		*dossier;
		QMenu		*game;
		QMenu		*nav;
		QMenu		*help;
	} d;
};

#endif	/* GUI_MAINMENU_H */
