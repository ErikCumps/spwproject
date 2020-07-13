/** \file
 * The SPWaW war cabinet - GUI - battle report.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORTS_BATTLE_H
#define GUI_REPORTS_BATTLE_H	1

#include "gui_private.h"
#include "model/model_dossier.h"
#include "gui_reports_battle_overview.h"
#include "gui_reports_battle_force.h"
#include "gui_reports_battle_compare.h"

class GuiRptBtl	: public QTabWidget
{
	Q_OBJECT

public:
	GuiRptBtl	(QWidget *P);
	~GuiRptBtl	(void);

	SL_ERROR	error_code;

public:
	void		enable		(bool flag);
	void		select		(MDLD_TREE_ITEM *item);
	void		refresh		(bool forced = false);
	MDLD_TREE_ITEM *current		(void);
	void		will_delete	(MDLD_TREE_ITEM *item);

public slots:
	void	selectedCurrentIndex	(int index);

private:
	struct s_data {
		bool		enabled;
		MDLD_TREE_ITEM	*item;

		QLabel		*disabled_label;

		GuiRptBtlOvr	*overview;
		GuiRptBtlFrc	*force_core;
		GuiRptBtlFrc	*force_spt;
		GuiRptBtlFrc	*force_opp;
#if	EXPERIMENTAL
		GuiRptBtlCmp	*compare;
#endif	/* EXPERIMENTAL */

		int		last_index;
	} d;

private:
	void	set_enabled	(bool flag);
};

#endif	/* GUI_REPORTS_BATTLE_H */
