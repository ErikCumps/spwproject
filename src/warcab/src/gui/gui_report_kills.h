/** \file
 * The SPWaW war cabinet - GUI - force kills report.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORT_KILLS_H
#define GUI_REPORT_KILLS_H	1

#include "gui_private.h"
#include "model/model_roster.h"

class GuiRptKill	: public QScrollArea
{
	Q_OBJECT

public:
	GuiRptKill	(QWidget *P = 0);
	~GuiRptKill	(void);

	SL_ERROR	error_code;

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
		QLabel			*label_khdr;
		QLabel			*label_klist;
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
	void	list_kills	(char *buf, unsigned int size, int &kcnt, bool dossier);
};

#endif	/* GUI_REPORT_KILLS_H */
