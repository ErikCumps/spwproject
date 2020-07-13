/** \file
 * The SPWaW war cabinet - GUI - battle turn report - strategic map.
 *
 * Copyright (C) 2012-2020 Erik Cumps <erik.cumps@gmail.com>
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
	void	refresh			(bool forced = false);

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
	void	hcftype_change		(int state);
	void	save_clicked		(bool checked);
	void	scrollsmap		(int x, int y);
	void	selected		(GuiTableView *who, const QModelIndex &index);
	void	intel_mode_set		(INTEL_MODE mode);

private:
	struct s_data {
		ModelTable		*model;

		QFrame			*frame;
		QGridLayout		*layout;
		QLabel			*intel;
		QCheckBox		*grid;
		QCheckBox		*vichexes;
		QCheckBox		*influence;
		QCheckBox		*frontline;
		QCheckBox		*zoom2x;
		QComboBox		*hcftype;
		QPushButton		*save;
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
		GUIVALTRACK (SMAP_HPMC_TYPE, Vhcftype);
		GUIVALTRACK (INTEL_MODE, Vintel_mode);
	} d;
};

#endif	/* GUI_REPORTS_BTURN_STRATMAP_H */
