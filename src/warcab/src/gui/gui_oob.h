/** \file
 * The SPWaW war cabinet - GUI - order of battle widget.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_OOB_H
#define GUI_OOB_H	1

#include "gui_private.h"
#include "gui_oob_view.h"
#include "model/model_dossier.h"

class GuiOob	: public QFrame
{
	Q_OBJECT

friend class GuiOobView;

public:
	GuiOob	(QWidget *P = 0);
	~GuiOob	(void);

	SL_ERROR	error_code;

public:
	void	set_parent	(GuiRptDsr *parent, bool player);
	void	set_parent	(GuiRptBtl *parent, bool player, bool core = false);
	void	set_parent	(GuiRptTrn *parent, bool player, bool core = false);
	void	refresh		(void);

signals:
	void	cmpcurr		(MDLD_TREE_ITEM *base);
	void	cmpbase		(MDLD_TREE_ITEM *base);

public slots:
	void	dltsort_change	(int state);
	void	prevcmp_change	(int state);
	void	autosort_change	(int state);
	void	set_highlight	(int idx);
	void	sort		(GuiOobView *who, int idx, Qt::SortOrder order);
	void	collapse	(GuiOobView *who, const QModelIndex &index);
	void	expand		(GuiOobView *who, const QModelIndex &index);
	void	selected	(GuiOobView *who, const QModelIndex &index);
	void	intel_mode_set	(INTEL_MODE mode);

private:
	struct s_data {
		ModelOob		*model;

		QFont			*font;
		QGridLayout		*layout;
		QLabel			*intel;
		QComboBox		*highlight;
		QCheckBox		*dltsort;
		QCheckBox		*prevcmp;
		QCheckBox		*autosort;
		QSplitter		*split;
		GuiOobView		*hdr_oob;
		GuiOobView		*bdy_oob;

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
		bool			psort;
		GOV_MODE		govmode;

		UtilMdlTreeReftrack	reftrack;
		GUIVALTRACK (bool, Vdltsort);
		GUIVALTRACK (bool, Vprevcmp);
		GUIVALTRACK (bool, Vautosort);
		GUIVALTRACK (INTEL_MODE, Vintel_mode);
	} d;

private:
	void	update	(void);
};

#endif	/* GUI_OOB_H */
