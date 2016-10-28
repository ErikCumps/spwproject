/** \file
 * The SPWaW war cabinet - GUI - force history report.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORT_HISTORY_H
#define GUI_REPORT_HISTORY_H	1

#include "gui_private.h"
#include "gui_history.h"

class GuiRptHst	: public QScrollArea
{
	Q_OBJECT

public:
	GuiRptHst	(QWidget *P = 0);
	~GuiRptHst	(void);

	SL_ERROR	error_code;

public:
	void	set_parent	(GuiRptDsr *parent, bool pflag);
	void	set_parent	(GuiRptBtl *parent, bool pflag, bool core = false);
	void	set_parent	(GuiRptTrn *parent, bool pflag);
	void	refresh		(void);

private:
	struct s_data {
		GuiHistory	*history;
	} d;
};

#endif	/* GUI_REPORT_HISTORY_H */
