/** \file
 * The SPWaW war cabinet - GUI - actions.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef GUI_ACTIONS_H
#define GUI_ACTIONS_H	1

#include "gui_private.h"

class GuiActions	: public QObject
{
	Q_OBJECT

public:
	GuiActions	(void);
	~GuiActions	(void);

	SL_ERROR	error_code;

public:
	void		enable_dossier_actions (bool b, SPWAW_DOSSIER_TYPE t);

public:
	struct s_data {
		QAction		*app_exit;
		QAction		*app_prefs;

		QAction		*dossier_new;
		QAction		*dossier_open;
		QAction		*dossier_close;
		QAction		*dossier_save;
		QAction		*dossier_saveAs;
		QAction		*dossier_edit;

		QAction		*game_add_campaign_savegame;
		QAction		*game_add_battle_savegame;
		QAction		*add_battle_savegame;
#if	ALLOW_SNAPSHOTS_LOAD
		QAction		*game_add_campaign_snapshot;
		QAction		*game_add_battle_snapshot;
		QAction		*add_battle_snapshot;
#endif	/* ALLOW_SNAPSHOTS_LOAD */

		QAction		*delete_turn;
		QAction		*delete_battle;
		QAction		*delete_dossier;

		QAction		*nav_raise;
		QAction		*nav_first;
		QAction		*nav_previous;
		QAction		*nav_next;
		QAction		*nav_last;
		QAction		*nav_lower;

		QAction		*help_reset_gui;
		QAction		*help_about;
	} p;
};

#endif	/* GUI_ACTIONS_H */
