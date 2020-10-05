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

public:
	void		status	(const QString &status);
	void		message	(const QString &message, int ms);
	void		clear	(void);

private:
	struct s_data {
		QStatusBar	*bar;
		QFrame		*infoFrame;
		QLabel		*info;
		QTimer		*msgtimer;

		QString		*status;
	} d;

private slots:
	void	msgdone	(void);

private:
	void	show_status	(void);
};

#endif	/* GUI_STATUS_H */
