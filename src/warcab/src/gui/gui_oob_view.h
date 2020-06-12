/** \file
 * The SPWaW war cabinet - GUI - order of battle view.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_OOB_VIEW_H
#define	GUI_OOB_VIEW_H	1

#include "gui_private.h"
#include "model/model_oob_data.h"

typedef enum e_GOV_MODE {
	GOV_MODE_DOSSIER = 0,
	GOV_MODE_BATTLE,
	GOV_MODE_TURN,
	GOV_MODE_LIMIT
} GOV_MODE;

class GuiOobView	: public QTreeView
{
	Q_OBJECT

public:
	GuiOobView	(bool hdr, GuiOob *oob, QWidget *P = 0);
	~GuiOobView	(void);

	SL_ERROR	error_code;

public:
	void	reparented	(void);
	void	reload		(GOV_MODE govm, bool sort, bool pflag, INTEL_MODE mode);
	void	resort		(void);
	void	refresh		(void);

public:
	void	wheelEvent		(QWheelEvent *e);
	void	mouseDoubleClickEvent	(QMouseEvent *event);

#if	EXPERIMENTAL
	void	contextMenuEvent	(QContextMenuEvent *event);
#endif	/* EXPERIMENTAL */

signals:
	void	scrolled	(int dx, int dy);
	void	wheeled		(QWheelEvent *e);
	void	sorted		(GuiOobView *who, int idx, Qt::SortOrder order);
	void	was_collapsed	(GuiOobView *who, const QModelIndex &index);
	void	was_expanded	(GuiOobView *who, const QModelIndex &index);
	void	wantscroll	(int d);
	void	selected	(GuiOobView *who, const QModelIndex &index);

public slots:
	void	scroll		(int d);
	void	wheel		(QWheelEvent *e);
	void	savesort	(int idx, Qt::SortOrder order);
	void	sort		(int idx, Qt::SortOrder order);
	void	collapsed	(const QModelIndex &index);
	void	expanded	(const QModelIndex &index);
	void	select		(const QModelIndex &index);
	void	currentChanged	(const QModelIndex &current, const QModelIndex &previous);
	void	intel_mode_set	(INTEL_MODE mode);

private:
	struct s_data {
		bool		ishdr;
		GuiOob		*parent;
		QFont		*font;

		int		rlayout[MDLO_COLUMN_CNT];
		int		dlayout[MDLO_COLUMN_CNT];

		int		sidx;
		Qt::SortOrder	sord;

		bool		pflag;
		GOV_MODE	govm;
		INTEL_MODE	intel_mode;
	} d;

private:
	void	apply_govmode	(void);
	void	apply_imdmode	(void);
	void	apply_mode	(void);
	void	build_rlayout	(void);
	void	build_dlayout	(void);
	void	apply_layout	(void);
};

#endif	/* GUI_OOB_VIEW_H */
