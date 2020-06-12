/** \file
 * The SPWaW war cabinet - GUI - min-max-average-spread widget.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_MMAS_H
#define GUI_MMAS_H	1

#include "gui_private.h"
#include "model/model_mmas.h"
#include "plot/plot_mmas.h"

class GuiMMAS	: public QFrame
{
	Q_OBJECT

friend class GuiMMASView;

public:
	GuiMMAS		(QWidget *P = 0);
	~GuiMMAS	(void);

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
	void	set_type	(int idx);
	void	set_filter	(int idx);
	void	set_target	(int idx);
	void	timeline_change	(int state);
	void	prevcmp_change	(int state);
	void	selected	(GuiMMASView *who, const QModelIndex &index);
	void	intel_mode_set	(INTEL_MODE mode);

private:
	struct s_data {
		ModelMMAS		*model;

		QFont			*font;
		QGridLayout		*layout;
		QLabel			*intel;
		QComboBox		*type;
		QComboBox		*filter;
		QComboBox		*target;
		QCheckBox		*timeline;
		QCheckBox		*prevcmp;
		QScrollArea		*scroller;
		PlotMMAS		*plot;
		QSplitter		*split;
		GuiMMASView		*hdr_mmas;
		GuiMMASView		*bdy_mmas;

		MDLD_TREE_TYPE		ptype;
		union u_pptr {
			GuiRptDsr	*d;
			GuiRptBtl	*b;
			GuiRptTrn	*t;
		}	pptr;
		MDLD_TREE_ITEM		*pdata;
		bool			pflag;
		bool			cflag;

		MDL_MFLST		*filters;

		struct s_typemap {
			MDLMMAS_TYPE	*map;
			int		cnt;
		}	typemap;
		int			Vfilter;
		GUI_FILTER_TARGET	*Vtargets;

		UtilMdlTreeReftrack	reftrack;
		GUIVALTRACK (bool, Vtimeline);
		GUIVALTRACK (bool, Vprevcmp);
		GUIVALTRACK (MDLMMAS_TYPE, Vtype);
		GUIVALTRACK (INTEL_MODE, Vintel_mode);
	} d;

private:
	void	update	(void);
};

#endif	/* GUI_MMAS_H */
