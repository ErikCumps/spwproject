/** \file
 * The SPWaW war cabinet - GUI - battle turn report - strategic map.
 *
 * Copyright (C) 2012-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_REPORTS_BTURN_STRATMAP_H
#define GUI_REPORTS_BTURN_STRATMAP_H	1

#include "gui_private.h"
#include "gui_roster.h"
#include "model/model_dossier.h"
#include "smap/smap_widget.h"
#include "util/util_mdltree_reftrack.h"

class GuiRptTrnSMap	: public QScrollArea
{
	Q_OBJECT

public:
	GuiRptTrnSMap	(QWidget *P = 0);
	~GuiRptTrnSMap	(void);

	SL_ERROR	error_code;

public:
	void	set_parent		(GuiRptTrn *p);
	void	refresh			(void);

public:
	void	resizeEvent		(QResizeEvent * event);

signals:
	void	cmpcurr			(MDLD_TREE_ITEM *base);
	void	cmpbase			(MDLD_TREE_ITEM *base);
	void	askscroll		(void);

public slots:
	void	grid_change		(int state);
	void	vichexes_change		(int state);
	void	influence_change	(int state);
	void	frontline_change	(int state);
	void	zoom2x_change		(int state);
	void	scrollsmap		(int x, int y);
	void	selected		(GuiTableView *who, const QModelIndex &index);

private:
	struct s_data {
		ModelTable		*model;

		//QFont			*font;
		QFrame			*frame;
		QGridLayout		*layout;
		QCheckBox		*grid;
		QCheckBox		*vichexes;
		QCheckBox		*influence;
		QCheckBox		*frontline;
		QCheckBox		*zoom2x;
		QSpacerItem		*spacer1;
		QScrollArea		*scroller;
		SmapWidget		*smap;
		QSplitter		*split;
		GuiTableView		*hdr_table;
		GuiTableView		*bdy_table;

		GuiRptTrn		*parent;

		UtilMdlTreeReftrack	reftrack;
		GUIVALTRACK (MDLD_TREE_ITEM *, battle);
		GUIVALTRACK (bool, Vgrid);
		GUIVALTRACK (bool, Vvichexes);
		GUIVALTRACK (bool, Vinfluence);
		GUIVALTRACK (bool, Vfrontline);
		GUIVALTRACK (bool, Vzoom2x);
	} d;
};

#endif	/* GUI_REPORTS_BTURN_STRATMAP_H */
