/** \file
 * The SPWaW war cabinet - GUI - unit roster report.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORT_ROSTER_H
#define GUI_REPORT_ROSTER_H	1

#include "gui_private.h"
#include "gui_roster.h"

class GuiRptRst	: public QScrollArea
{
	Q_OBJECT

public:
	GuiRptRst	(QWidget *P = 0);
	~GuiRptRst	(void);

	SL_ERROR	error_code;

public:
	void	set_parent	(GuiRptDsr *parent, bool pflag);
	void	set_parent	(GuiRptBtl *parent, bool pflag, bool core = false);
	void	set_parent	(GuiRptTrn *parent, bool pflag, bool core = false);
	void	refresh		(void);

private:
	struct s_data {
		GuiRoster	*roster;
	} d;
};

#endif	/* GUI_REPORT_ROSTER_H */
