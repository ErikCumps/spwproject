/** \file
 * The SPWaW war cabinet - GUI - force losses report.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
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
	void	refresh		(bool forced = false);

signals:
	void	cmpcurr		(MDLD_TREE_ITEM *base);
	void	cmpbase		(MDLD_TREE_ITEM *base);

public slots:
	void	intel_mode_set	(INTEL_MODE mode);

private:
	struct s_data {
		ModelRoster		*model;

		QFont			*font;
		QFrame			*frame;
		QGridLayout		*layout;
		QLabel			*label_nodata;
		QLabel			*label_intel;
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
		GUIVALTRACK (INTEL_MODE, Vintel_mode);
	} d;

private:
	bool	update			(bool forced);
	void	list_killed_dossier	(bool sort);
	void	list_abandoned_dossier	(bool sort);
	void	list_damaged_dossier	(bool sort);
	void	list_killed		(void);
	void	list_abandoned		(void);
	void	list_damaged		(void);
};

#endif	/* GUI_REPORT_LOSSES_H */
