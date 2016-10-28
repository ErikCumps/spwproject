/** \file
 * The SPWaW war cabinet - GUI - order of battle report.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORT_OOB_H
#define GUI_REPORT_OOB_H	1

#include "gui_private.h"
#include "gui_oob.h"

class GuiRptOob	: public QScrollArea
{
	Q_OBJECT

public:
	GuiRptOob	(QWidget *P = 0);
	~GuiRptOob	(void);

	SL_ERROR	error_code;

public:
	void	set_parent	(GuiRptDsr *parent, bool pflag);
	void	set_parent	(GuiRptBtl *parent, bool pflag, bool core = false);
	void	set_parent	(GuiRptTrn *parent, bool pflag, bool core = false);
	void	refresh		(void);

private:
	struct s_data {
		GuiOob	*oob;
	} d;
};

#endif	/* GUI_REPORT_OOB_H */
