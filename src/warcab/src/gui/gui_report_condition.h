/** \file
 * The SPWaW war cabinet - GUI - force condition report.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORT_CONDITION_H
#define GUI_REPORT_CONDITION_H	1

#include "gui_private.h"
#include "model/model_roster.h"
#include "util/util_mdltree_reftrack.h"

class GuiRptCnd	: public QScrollArea
{
	Q_OBJECT

public:
	GuiRptCnd	(QWidget *P = 0);
	~GuiRptCnd	(void);

	SL_ERROR	error_code;

public:
#if	EXPERIMENTAL
	void	contextMenuEvent(QContextMenuEvent *event);
#endif	/* EXPERIMENTAL */

public:
	void	set_parent	(GuiRptDsr *parent, bool player);
	void	set_parent	(GuiRptBtl *parent, bool player, bool core = false);
	void	set_parent	(GuiRptTrn *parent, bool player, bool core = false);
	void	refresh		(bool forced = false);

signals:
	void	cmpcurr		(MDLD_TREE_ITEM *base);
	void	cmpbase		(MDLD_TREE_ITEM *base);

private:
	struct s_data {
		ModelRoster		*model;

		QFont			*font;
		QFrame			*frame;
		QGridLayout		*layout;
		QLabel			*label_nodata;
		QLabel			*label_exp;
		QLabel			*label_mor;
		QLabel			*label_ral;
		QLabel			*label_inf;
		QLabel			*label_arm;
		QLabel			*label_art;
		QSpacerItem		*spacer;

		MDLD_TREE_TYPE		ptype;
		union u_pptr {
			GuiRptDsr	*d;
			GuiRptBtl	*b;
			GuiRptTrn	*t;
		}	pptr;
		bool			pflag;
		bool			cflag;
		MDLD_TREE_ITEM		*pdata;

		MDLD_TREE_ITEM		*pcurr;
		MDLD_TREE_ITEM		*pbase;

		UtilMdlTreeReftrack	reftrack;
	} d;

private:
	bool	update		(bool forced);
	void	mkshortlist	(char *title, MDLR_COLUMN col, bool up, char *buf, unsigned int size, bool worst);
};

#endif	/* GUI_REPORT_CONDITION_H */
