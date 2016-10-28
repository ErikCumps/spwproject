/** \file
 * The SPWaW war cabinet - GUI - force losses report.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORT_LOSSES_H
#define GUI_REPORT_LOSSES_H	1

#include "gui_private.h"

class GuiRptLoss	: public QScrollArea
{
	Q_OBJECT

public:
	GuiRptLoss	(QWidget *P = 0);
	~GuiRptLoss	(void);

	SL_ERROR	error_code;

public:
	void	set_parent	(GuiRptDsr *parent, bool player);
	void	set_parent	(GuiRptBtl *parent, bool player, bool core = false);
	void	set_parent	(GuiRptTrn *parent, bool player, bool core = false);
	void	refresh		(void);

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
		QLabel			*label_ahdr;
		QLabel			*label_alist;
		QLabel			*label_dhdr;
		QLabel			*label_dlist;
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
	void	update			(void);
	void	list_killed_dossier	(char *buf, unsigned int size, int &icnt);
	void	list_abandoned_dossier	(char *buf, unsigned int size, int &icnt);
	void	list_damaged_dossier	(char *buf, unsigned int size, int &icnt);
	void	list_killed		(char *buf, unsigned int size, int &icnt);
	void	list_abandoned		(char *buf, unsigned int size, int &icnt);
	void	list_damaged		(char *buf, unsigned int size, int &icnt);
};

#endif	/* GUI_REPORT_LOSSES_H */
