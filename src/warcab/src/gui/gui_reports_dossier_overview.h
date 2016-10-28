/** \file
 * The SPWaW war cabinet - GUI - dossier report - overview.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORTS_DOSSIER_OVERVIEW_H
#define GUI_REPORTS_DOSSIER_OVERVIEW_H	1

#include "gui_private.h"
#include "model/model_dossier.h"
#include "model/model_roster.h"
#include "util/util_mdltree_reftrack.h"

class GuiRptDsrOvr	: public QScrollArea
{
	Q_OBJECT

public:
	GuiRptDsrOvr	(QWidget *P = 0);
	~GuiRptDsrOvr	(void);

	SL_ERROR	error_code;

public:
	void	set_parent	(GuiRptDsr *p);
	void	refresh		(bool forced);

public:
	void	resizeEvent	(QResizeEvent * event);

signals:
	void	cmpcurr		(MDLD_TREE_ITEM *base);
	void	cmpbase		(MDLD_TREE_ITEM *base);

private:
	struct s_data {
		ModelRoster		*model;
		
		QFont			*font;
		QFrame			*frame;
		QGridLayout		*layout;
		QLabel			*flag;
		QLabel			*overview;
		struct s_losses {
			QGridLayout	*layout;
			QLabel		*plr;
			QLabel		*opp;
			QSpacerItem	*spacer;
		}			losses;
		QLabel			*changes;
		QSpacerItem		*spacer;
		
		GuiRptDsr		*parent;

		UtilMdlTreeReftrack	reftrack;
	} d;

private:
	void	list_promotions	(SPWAW_DOSSIER *d, bool reverse, char *buf, unsigned int size, int &icnt);
	void	list_upgrades	(SPWAW_DOSSIER *d, bool reverse, char *buf, unsigned int size, int &icnt);

};

#endif	/* GUI_REPORTS_DOSSIER_OVERVIEW_H */
