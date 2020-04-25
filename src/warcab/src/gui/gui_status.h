/** \file
 * The SPWaW war cabinet - GUI - status bar.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_STATUS_H
#define GUI_STATUS_H	1

#include "gui_private.h"

class GuiStatus		: public QObject
{
	Q_OBJECT

public:
	GuiStatus	(QFont &font);
	~GuiStatus	(void);

	SL_ERROR	error_code;

public slots:
	void message (const QString &message, int ms);
	void infodone (void);

private:
	struct s_data {
		QStatusBar	*status;
		QFrame		*infoFrame;
		QLabel		*info;
		QFrame		*selinfFrame;
		QLabel		*selinf;
		QTimer		*infotimer;
	} d;
};

#endif	/* GUI_STATUS_H */
