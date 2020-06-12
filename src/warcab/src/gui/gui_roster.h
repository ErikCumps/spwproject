/** \file
 * The SPWaW war cabinet - GUI - unit roster widget.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_ROSTER_H
#define GUI_ROSTER_H	1

#include "gui_private.h"
#include "gui_roster_view.h"
#include "model/model_filter.h"
#include "model/model_dossier.h"
#include "model/model_roster.h"

class GuiRoster	: public QFrame
{
	Q_OBJECT

friend class GuiRosterView;

public:
	GuiRoster	(QWidget *P = 0);
	~GuiRoster	(void);

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
	void	dltsort_change	(int state);
	void	prevcmp_change	(int state);
	void	autosort_change	(int state);
	void	set_highlight	(int idx);
#if	EXPERIMENTAL
	void	set_filter_spec	(int idx);
	void	set_filter_tgt	(int idx);
#endif	/* EXPERIMENTAL */
	void	sort		(GuiRosterView *who, int idx, Qt::SortOrder order);
	void	selected	(GuiRosterView *who, const QModelIndex &index);
	void	intel_mode_set	(INTEL_MODE mode);

private:
	struct s_data {
		QFont			*rgfont;
		QFont			*dcfont;
		ModelRoster		*model;

		QGridLayout		*layout;
		QLabel			*intel;
#if	EXPERIMENTAL
		QComboBox		*filter_spec;
		QComboBox		*filter_tgt;
#endif	/* EXPERIMENTAL */
		QComboBox		*highlight;
		QCheckBox		*dltsort;
		QCheckBox		*prevcmp;
		QCheckBox		*autosort;
		QSplitter		*split;
		GuiRosterView		*hdr_roster;
		GuiRosterView		*bdy_roster;

		MDLD_TREE_TYPE		ptype;
		union u_pptr {
			GuiRptDsr	*d;
			GuiRptBtl	*b;
			GuiRptTrn	*t;
		}	pptr;
		MDLD_TREE_ITEM		*pdata;
		MDLD_TREE_ITEM		*pcurr;
		MDLD_TREE_ITEM		*pbase;
		bool			pflag;
		bool			cflag;
		bool			mflag;
		bool			psort;
		GRV_MODE		grvmode;

		MDLR_HILITE		*hl_array;
		int			hl_count;

		UtilMdlTreeReftrack	reftrack;
		GUIVALTRACK (bool, Vdltsort);
		GUIVALTRACK (bool, Vprevcmp);
		GUIVALTRACK (bool, Vautosort);
		GUIVALTRACK (INTEL_MODE, Vintel_mode);

#if	EXPERIMENTAL
		MDLR_FILTER		Vfspec;
		struct s_Vftgts {
			xMDL_FILTER_LIST*list;
			unsigned int	curr;
		}	Vftgts[MDLR_FILTER_LIMIT+1];
#endif	/* EXPERIMENTAL */
	} d;

private:
	void	setup_highlight	(void);
	bool	update		(bool forced);
};

#endif	/* GUI_ROSTER_H */
