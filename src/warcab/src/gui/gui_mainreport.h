/** \file
 * The SPWaW war cabinet - GUI - main report widget.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_MAINREPORT_H
#define GUI_MAINREPORT_H	1

#include "gui_private.h"
#include "model/model_dossier.h"

class GuiMainRpt	: public QTabWidget
{
	Q_OBJECT

public:
	GuiMainRpt	(QWidget *P);
	~GuiMainRpt	(void);

	SL_ERROR	error_code;

public:
	void	report		(MDLD_TREE_ITEM *item);
	void	update		(void);
	void	clear		(void);
	void	will_delete	(MDLD_TREE_ITEM *item);

public slots:
	void	selectedCurrentIndex	(int index);

private:
	struct s_data {
		GuiRptDsr	*dossier;
		GuiRptBtl	*battle;
		GuiRptTrn	*bturn;

		int		dossier_idx;
		int		battle_idx;
		int		bturn_idx;
	} d;

private:
	void		select_dossier	(MDLD_TREE_ITEM *item);
	void		select_battle	(MDLD_TREE_ITEM *item);
	void		select_bturn	(MDLD_TREE_ITEM *item);
	MDLD_TREE_ITEM *prepare_dossier	(MDLD_TREE_ITEM *item);
	MDLD_TREE_ITEM *prepare_battle	(MDLD_TREE_ITEM *item);
	MDLD_TREE_ITEM *prepare_bturn	(MDLD_TREE_ITEM *item);

};

#endif	/* GUI_MAINREPORT_H */
