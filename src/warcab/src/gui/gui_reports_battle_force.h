/** \file
 * The SPWaW war cabinet - GUI - battle report - force.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORTS_BATTLE_FORCE_H
#define GUI_REPORTS_BATTLE_FORCE_H	1

#include "gui_private.h"
#include "gui_report_kills.h"
#include "gui_report_losses.h"
#include "gui_report_condition.h"
#include "gui_report_progression.h"
#include "gui_report_oob.h"
#include "gui_report_roster.h"
#include "gui_report_history.h"
#include "gui_report_mmas.h"

class GuiRptBtlFrc	: public QTabWidget
{
	Q_OBJECT

public:
	GuiRptBtlFrc	(QWidget *P = 0);
	~GuiRptBtlFrc	(void);

	SL_ERROR	error_code;

public:
	void	set_parent	(GuiRptBtl *parent, bool pflag, bool cflag);
	void	refresh		(void);

public slots:
	void	selectedCurrentIndex	(int index);

private:
	struct s_data {
		GuiRptBtl	*parent;
		bool		pflag;
		bool		cflag;

		bool		enabled;
		QLabel		*disabled_label;
		
		GuiRptCnd	*cnd;
		GuiRptPrg	*prg;
		GuiRptKill	*kill;
		GuiRptLoss	*loss;
		GuiRptOob	*oob;
		GuiRptRst	*rst;
		GuiRptHst	*hst;
		GuiRptMMAS	*mmas;

		int		last_index;
	} d;

private:
	void	set_enabled	(bool flag);
};

#endif	/* GUI_REPORTS_BATTLE_FORCE_H */
