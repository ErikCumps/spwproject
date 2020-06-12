/** \file
 * The SPWaW war cabinet - GUI - unit history view.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_HISTORY_VIEW_H
#define	GUI_HISTORY_VIEW_H	1

#include "gui_private.h"
#include "model/model_history_data.h"

typedef enum e_GHV_MODE {
	GHV_MODE_DOSSIER = 0,
	GHV_MODE_BATTLE,
	GHV_MODE_LIMIT
} GHV_MODE;

class GuiHistoryView	: public QTableView
{
	Q_OBJECT

public:
	GuiHistoryView	(bool hdr, GuiHistory *history, QWidget *P = 0);
	~GuiHistoryView	(void);

	SL_ERROR	error_code;

public:
	void	wheelEvent	(QWheelEvent *e);

public:
	void	reload		(GHV_MODE ghvm, bool mflag, bool pflag, INTEL_MODE mode);
	void	refresh		(void);

signals:
	void	scrolled	(int dx, int dy);
	void	wheeled		(QWheelEvent *e);
	void	wantscroll	(int d);
	void	selected	(GuiHistoryView *who, const QModelIndex &index);

public slots:
	void	scroll		(int d);
	void	wheel		(QWheelEvent *e);
	void	select		(const QModelIndex &index);
	void	currentChanged	(const QModelIndex &current, const QModelIndex &previous);
	void	intel_mode_set	(INTEL_MODE mode);

private:
	struct s_data {
		bool		ishdr;
		GuiHistory	*parent;
		QFont		*font;

		int		rlayout[MDLH_COLUMN_CNT];
		int		mlayout[MDLH_COLUMN_CNT];

		bool		pflag;
		GHV_MODE	ghvm;
		bool		mflag;
		INTEL_MODE	intel_mode;
	} d;

private:
	void	apply_ghvmode	(void);
	void	apply_imdmode	(void);
	void	apply_mode	(void);
	void	build_rlayout	(void);
	void	build_mlayout	(void);
	void	apply_layout	(void);
};

#endif	/* GUI_HISTORY_VIEW_H */
