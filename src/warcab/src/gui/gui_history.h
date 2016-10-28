/** \file
 * The SPWaW war cabinet - GUI - unit history widget.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_HISTORY_H
#define GUI_HISTORY_H	1

#include "gui_private.h"
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
	void	set_parent	(GuiRptTrn *parent, bool player);
	void	refresh		(void);

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
		ModelUnitlist		*lmodel;
		ModelHistory		*hmodel;

		QFont			*font;
		QGridLayout		*layout;
		QComboBox		*highlight;
		QCheckBox		*prevcmp;
		QSplitter		*split;
		GuiUnitlistView		*unitlist;
		GuiHistoryView		*hdr_history;
		GuiHistoryView		*bdy_history;

		MDLD_TREE_TYPE		ptype;
		union u_pptr {
			GuiRptDsr	*d;
			GuiRptBtl	*b;
			GuiRptTrn	*t;
		}	pptr;
		MDLD_TREE_ITEM		*pdata;
		bool			pflag;
		bool			cflag;
		MDLH_HILITE		*hl_array;
		int			hl_count;

		UtilMdlTreeReftrack	reftrack;
		GUIVALTRACK (int, uidx);
		GUIVALTRACK (bool, Vprevcmp);
	} d;

private:
	void	setup_highlight	(void);
	void	update	(void);
};

#endif	/* GUI_HISTORY_H */
