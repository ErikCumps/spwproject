/** \file
 * The SPWaW war cabinet - GUI - battle turn report - overview.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORTS_BTURN_OVERVIEW_H
#define GUI_REPORTS_BTURN_OVERVIEW_H	1

#include "gui_private.h"
#include "gui_roster.h"
#include "model/model_dossier.h"
#include "util/util_mdltree_reftrack.h"

class GuiRptTrnOvr	: public QScrollArea
{
	Q_OBJECT

public:
	GuiRptTrnOvr	(QWidget *P = 0);
	~GuiRptTrnOvr	(void);

	SL_ERROR	error_code;

public:
	void	set_parent (GuiRptTrn *p);
	void	refresh	(void);

public:
	void	resizeEvent	(QResizeEvent * event);

signals:
	void	cmpcurr		(MDLD_TREE_ITEM *base);
	void	cmpbase		(MDLD_TREE_ITEM *base);

public slots:
	void	intel_mode_set	(INTEL_MODE mode);

private:
	struct s_data {
		QFont			*font;
		QFrame			*frame;
		QGridLayout		*layout;
		QLabel			*name;
		QLabel			*intel;
		QLabel			*player1;
		QLabel			*mission;
		QLabel			*player2;
		QSpacerItem		*spacer;
		QLabel			*label;
		QSpacerItem		*bspacer;

		GuiRptTrn		*parent;
		
		UtilMdlTreeReftrack	reftrack;
		GUIVALTRACK (INTEL_MODE, Vintel_mode);
	} d;
};

#endif	/* GUI_REPORTS_BTURN_OVERVIEW_H */
