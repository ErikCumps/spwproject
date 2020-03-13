/** \file
 * The SPWaW war cabinet - GUI - unit history widget.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_HISTORY_H
#define GUI_HISTORY_H	1

#include "gui_private.h"
#include "gui_history_view.h"
#include "model/model_dossier.h"
#include "model/model_history.h"

class GuiHistory	: public QFrame
{
	Q_OBJECT

friend class GuiUnitlistView;
friend class GuiHistoryView;

public:
	GuiHistory	(QWidget *P = 0);
	~GuiHistory	(void);

	SL_ERROR	error_code;

public:
	void	set_parent	(GuiRptDsr *parent, bool player);
	void	set_parent	(GuiRptBtl *parent, bool player, bool core = false);
	void	refresh		(bool forced = false);

signals:
	void	cmpcurr		(MDLD_TREE_ITEM *base);
	void	cmpbase		(MDLD_TREE_ITEM *base);

public slots:
	void	prevcmp_change	(int state);
	void	set_select	(int idx);
	void	set_highlight	(int idx);
	void	selected	(GuiHistoryView *who, const QModelIndex &index);

private:
	struct s_data {
		QFont			*rgfont;
		QFont			*dcfont;
		ModelUnitlist		*lmodel;
		ModelHistory		*hmodel;

		QGridLayout		*layout;
		QComboBox		*highlight;
		QCheckBox		*prevcmp;
		QSpacerItem		*spacer;
		QFrame			*separator;
		QFont			*dfont;
		QLabel			*detail;
		QSplitter		*split;
		GuiUnitlistView		*unitlist;
		GuiHistoryView		*hdr_history;
		GuiHistoryView		*bdy_history;

		MDLD_TREE_TYPE		ptype;
		union u_pptr {
			GuiRptDsr	*d;
			GuiRptBtl	*b;
		}	pptr;
		MDLD_TREE_ITEM		*pdata;
		bool			pflag;
		bool			cflag;
		bool			mflag;
		GHV_MODE		ghvmode;

		MDLH_HILITE		*hl_array;
		int			hl_count;

		UtilMdlTreeReftrack	reftrack;
		GUIVALTRACK (int, uidx);
		GUIVALTRACK (bool, Vprevcmp);
	} d;

private:
	void	setup_highlight	(void);
	bool	update		(bool forced);
};

#endif	/* GUI_HISTORY_H */
