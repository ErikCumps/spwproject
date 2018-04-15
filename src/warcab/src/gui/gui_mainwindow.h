/** \file
 * The SPWaW war cabinet - GUI - main window.
 *
 * Copyright (C) 2005-2017 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H	1

#include "gui_private.h"

class GuiMainWindow	: public QMainWindow
{
	Q_OBJECT

public:
	GuiMainWindow	(void);
	~GuiMainWindow	(void);

	SL_ERROR	error_code;

public:
	void		caption		(void);
	void		caption		(char *str);
	GuiActions *	get_actions	(void);
	GuiStatus *	get_status	(void);
	GuiDossier *	get_dossier	(void);
	GuiMainRpt *	get_report	(void);

public slots:
	void	action_app_exit		(void);
	void	action_app_prefs	(void);

	void	action_dossier_new	(void);
	void	action_dossier_open	(void);
	void	action_dossier_close	(void);
	void	action_dossier_save	(void);
	void	action_dossier_saveAs	(void);
	bool	action_dossier_edit	(void);

	void	action_file_add_game	(void);
#if	ALLOW_SNAPSHOTS_LOAD
	void	action_file_add_snap	(void);
#endif	/* ALLOW_SNAPSHOTS_LOAD */

	void	action_delete_turn	(void);
	void	action_delete_battle	(void);
	void	action_delete_dossier	(void);

	void	action_nav_raise	(void);
	void	action_nav_first	(void);
	void	action_nav_previous	(void);
	void	action_nav_next		(void);
	void	action_nav_last		(void);
	void	action_nav_lower	(void);

	void	action_help_reset_gui	(void);
	void	action_help_about	(void);

private:
	struct s_data {
		QPixmap		*main_icon;
		QFont		*main_font;
		GuiActions	*actions;
		GuiMainMenu	*menu;
		GuiToolbar	*toolbar;
		GuiStatus	*status;
		QWidget		*body;
		QVBoxLayout	*body_layout;
		QSplitter	*body_panes;
		GuiDossier	*dossier;
		GuiMainRpt	*report;

		GUI_STATE	state;
	} d;

private:
	bool	state_restore	(void);
	void	state_save	(void);
	bool	eventFilter	(QObject *obj, QEvent *event);
};

#endif	/* GUI_MAINWINDOW_H */
