/** \file
 * The SPWaW war cabinet - GUI - battle report - comparisons.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORTS_BATTLE_COMPARE_H
#define GUI_REPORTS_BATTLE_COMPARE_H	1

#include "gui_private.h"
#include "model/model_dossier.h"
#include "gui_roster.h"

class GuiRptBtlCmp	: public QScrollArea
{
	Q_OBJECT

public:
	GuiRptBtlCmp	(QWidget *P = 0);
	~GuiRptBtlCmp	(void);

	SL_ERROR	error_code;

public:
	void	set_parent	(GuiRptBtl *p);
	void	refresh		(bool forced = false);

signals:
	void	cmpcurr		(MDLD_TREE_ITEM *base);
	void	cmpbase		(MDLD_TREE_ITEM *base);

private:
	struct s_data {
		GuiRptBtl	*parent;
		QFont		*font;
		QFrame		*frame;
		QGridLayout	*layout;
		QLabel		*label;
	} d;
};

#endif	/* GUI_REPORTS_BATTLE_COMPARE_H */
