/** \file
 * The SPWaW war cabinet - GUI - tool bar.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_TOOLBAR_H
#define GUI_TOOLBAR_H	1

#include "gui_private.h"

class GuiToolbar	: public QObject
{
	Q_OBJECT

public:
	GuiToolbar	(void);
	~GuiToolbar	(void);

	SL_ERROR	error_code;

private:
	struct s_data {
		QToolBar	*toolbar;
	} d;
};

#endif	/* GUI_TOOLBAR_H */
