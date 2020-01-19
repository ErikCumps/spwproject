/** \file
 * The SPWaW war cabinet - GUI - battle report - overview.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORTS_BATTLE_OVERVIEW_H
#define GUI_REPORTS_BATTLE_OVERVIEW_H	1

#include "gui_private.h"
#include "model/model_dossier.h"
#include "util/util_mdltree_reftrack.h"

class GuiRptBtlOvr	: public QScrollArea
{
	Q_OBJECT

public:
	GuiRptBtlOvr	(QWidget *P = 0);
	~GuiRptBtlOvr	(void);

	SL_ERROR	error_code;

public:
	void	set_parent (GuiRptBtl *p);
	void	refresh	(void);

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
		QLabel			*name;
		QLabel			*player1;
		QLabel			*mission;
		QLabel			*player2;
		QSpacerItem		*rspacer;
		QLabel			*overview;
		struct s_losses {
			QGridLayout	*layout;
			QLabel		*plr;
			QLabel		*opp;
			QSpacerItem	*spacer;
		}			losses;
		QLabel			*changes;
		QSpacerItem		*bspacer;
		
		GuiRptBtl		*parent;

		UtilMdlTreeReftrack	reftrack;
	} d;

private:
	void	list_replacements	(SPWAW_BATTLE *b, UtilStrbuf &strbuf);
	void	list_reassignments	(SPWAW_BATTLE *b, UtilStrbuf &strbuf);
	void	list_upgrades		(SPWAW_BATTLE *b, UtilStrbuf &strbuf);
	void	list_promotions		(SPWAW_BATTLE *b, UtilStrbuf &strbuf);
	void	list_demotions		(SPWAW_BATTLE *b, UtilStrbuf &strbuf);
	void	list_commissions	(SPWAW_BATTLE *b, UtilStrbuf &strbuf);
	void	list_decommissions	(SPWAW_BATTLE *b, UtilStrbuf &strbuf);
};

#endif	/* GUI_REPORTS_BATTLE_OVERVIEW_H */
