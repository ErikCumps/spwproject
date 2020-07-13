/** \file
 * The SPWaW war cabinet - GUI - min-max-average-spread report.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORT_MMAS_H
#define GUI_REPORT_MMAS_H	1

#include "gui_private.h"
#include "gui_mmas.h"

class GuiRptMMAS	: public QScrollArea
{
	Q_OBJECT

public:
	GuiRptMMAS	(QWidget *P = 0);
	~GuiRptMMAS	(void);

	SL_ERROR	error_code;

public:
	void	set_parent	(GuiRptDsr *parent, bool pflag);
	void	set_parent	(GuiRptBtl *parent, bool pflag, bool core = false);
	void	set_parent	(GuiRptTrn *parent, bool pflag);
	void	refresh		(bool forced = false);

private:
	struct s_data {
		GuiMMAS	*mmas;
	} d;
};

#endif	/* GUI_REPORT_ROSTER_H */
