/** \file
 * The SPWaW war cabinet - GUI - "start megacam tracking" dialog box.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_DLG_START_MEGACAM_TRACKING_H
#define	GUI_DLG_START_MEGACAM_TRACKING_H	1

#include "gui_private.h"
#include "model/model_savelist.h"

class GuiDlgStartMegaCamTracking	: public QDialog
{
	Q_OBJECT

public:
	GuiDlgStartMegaCamTracking	(SPWAW_SAVELIST_TARGET &target, const char *path, SPWAW_SAVELIST *ignore);
	~GuiDlgStartMegaCamTracking	(void);

	SL_ERROR	error_code;

public:
	void	get_data	(SPWAW_SAVELIST *list);


private:
	struct s_data {
		QFont			*dlgf;
		QFontMetrics		*dlgfm;
		QDialogButtonBox	*buttons;
		QPushButton		*but_ok;
		QPushButton		*but_cancel;
		QWidget			*body;
		QLabel			*info;
		QTreeView		*view;
		QGridLayout		*layout;

		ModelSaveList		*savemodel;
		SPWAW_SAVELIST		*savelist;
	} d;

private slots:
	void	selection_changed	(const QItemSelection &selected, const QItemSelection &deselected);
	void	accept			(void);

private:
	void	refresh_ok_button_status	(void);
};

#endif	/* GUI_DLG_START_MEGACAM_TRACKING_H */
