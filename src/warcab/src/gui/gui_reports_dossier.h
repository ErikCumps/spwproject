/** \file
 * The SPWaW war cabinet - GUI - dossier report.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORTS_DOSSIER_H
#define GUI_REPORTS_DOSSIER_H	1

#include "gui_private.h"
#include "model/model_dossier.h"
#include "gui_reports_dossier_overview.h"
#include "gui_report_kills.h"
#include "gui_report_losses.h"
#include "gui_report_condition.h"
#include "gui_report_progression.h"
#include "gui_report_oob.h"
#include "gui_report_roster.h"
#include "gui_report_history.h"
#include "gui_report_mmas.h"
#include "gui_reports_dossier_graphs.h"
#include "util/util_mdltree_reftrack.h"

class GuiRptDsr	: public QTabWidget
{
	Q_OBJECT

friend class GuiRosterView;

public:
	GuiRptDsr	(QWidget *P);
	~GuiRptDsr	(void);

	SL_ERROR	error_code;

public:
	void		enable		(bool flag);
	void		select		(MDLD_TREE_ITEM *item);
	void		refresh		(bool forced);
	MDLD_TREE_ITEM *current		(void);
	void		will_delete	(MDLD_TREE_ITEM *item);

public slots:
	void	selectedCurrentIndex	(int index);

private:
	struct s_data {
		bool			enabled;
		MDLD_TREE_ITEM		*item;

		QLabel			*disabled_label;

		GuiRptDsrOvr		*overview;
		GuiRptCnd		*cnd;
		GuiRptPrg		*prg;
		GuiRptKill		*kill;
		GuiRptLoss		*loss;
		GuiRptOob		*oob;
		GuiRptRst		*rst;
		GuiRptHst		*hst;
		GuiRptMMAS		*mmas;
		GuiRptDsrPlt		*graphs;

		int			last_index;

		UtilMdlTreeReftrack	reftrack;
	} d;

private:
	void	set_enabled	(bool flag);
};

#endif	/* GUI_REPORTS_DOSSIER_H */
