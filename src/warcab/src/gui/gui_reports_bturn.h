/** \file
 * The SPWaW war cabinet - GUI - battle turn report.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORTS_BTURN_H
#define GUI_REPORTS_BTURN_H	1

#include "gui_private.h"
#include "model/model_dossier.h"
#include "gui_reports_bturn_overview.h"
#include "gui_reports_bturn_force.h"
#include "gui_reports_bturn_compare.h"
#include "gui_reports_bturn_stratmap.h"

class GuiRptTrn	: public QTabWidget
{
	Q_OBJECT

public:
	GuiRptTrn	(QWidget *P);
	~GuiRptTrn	(void);

	SL_ERROR	error_code;

public:
	void		enable		(bool flag);
	void		select		(MDLD_TREE_ITEM *item);
	void		refresh		(void);
	MDLD_TREE_ITEM *current		(void);
	void		will_delete	(MDLD_TREE_ITEM *item);

public slots:
	void	selectedCurrentIndex	(int index);

private:
	struct s_data {
		bool		enabled;
		MDLD_TREE_ITEM	*item;

		QLabel		*disabled_label;

		GuiRptTrnOvr	*overview;
		GuiRptTrnFrc	*force_core;
		GuiRptTrnFrc	*force_spt;
		GuiRptTrnFrc	*force_opp;
#if	EXPERIMENTAL
		GuiRptTrnCmp	*compare;
#endif	/* EXPERIMENTAL */
		GuiRptTrnSMap	*stratmap;

		int		last_index;
	} d;

private:
	void	set_enabled	(bool flag);
};

#endif	/* GUI_REPORTS_BTURN_H */
