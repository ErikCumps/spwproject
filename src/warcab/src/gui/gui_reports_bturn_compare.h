/** \file
 * The SPWaW war cabinet - GUI - battle turn report - comparisons.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORTS_BTURN_COMPARE_H
#define GUI_REPORTS_BTURN_COMPARE_H	1

#include "gui_private.h"
#include "model/model_dossier.h"
#include "gui_roster.h"

class GuiRptTrnCmp	: public QScrollArea
{
	Q_OBJECT

public:
	GuiRptTrnCmp	(QWidget *P = 0);
	~GuiRptTrnCmp	(void);

	SL_ERROR	error_code;

public:
	void	set_parent (GuiRptTrn *p);
	void	refresh	(void);

signals:
	void	cmpcurr		(MDLD_TREE_ITEM *base);
	void	cmpbase		(MDLD_TREE_ITEM *base);

private:
	struct s_data {
		GuiRptTrn	*parent;
		QFont		*font;
		QFrame		*frame;
		QGridLayout	*layout;
		QLabel		*label;
	} d;
};

#endif	/* GUI_REPORTS_BTURN_COMPARE_H */
