/** \file
 * The SPWaW war cabinet - GUI - "add standalone battle savegame" dialog box.
 *
 * Copyright (C) 2005-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_DLG_ADD_BATTLE_SAVEGAME_H
#define	GUI_DLG_ADD_BATTLE_SAVEGAME_H	1

#include "gui_private.h"
#include "model/model_savelist.h"

#if	ALLOW_SNAPSHOTS_LOAD
#include "model/model_snaplist.h"
#endif	/* ALLOW_SNAPSHOTS_LOAD */

class GuiDlgAddBattleSavegame	: public QDialog
{
	Q_OBJECT

public:
	GuiDlgAddBattleSavegame		(char *path, SPWAW_SAVELIST *ignore, char *name = NULL);
#if	ALLOW_SNAPSHOTS_LOAD
	GuiDlgAddBattleSavegame		(char *path, SPWAW_SNAPLIST *ignore, char *name = NULL);
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	~GuiDlgAddBattleSavegame	(void);

	SL_ERROR	error_code;

public:
	char	*get_data	(SPWAW_SAVELIST *list);
#if	ALLOW_SNAPSHOTS_LOAD
	char	*get_data	(SPWAW_SNAPLIST *list);
#endif	/* ALLOW_SNAPSHOTS_LOAD */

private:
	struct s_data {
		QFont			*dlgf;
		QFontMetrics		*dlgfm;
		QDialogButtonBox	*buttons;
		QPushButton		*but_ok;
		QPushButton		*but_cancel;
		QWidget			*body;
		QLabel			*info;
		QLabel			*name_label;
		QLineEdit		*name_edit;
		QTreeView		*view;
		QGridLayout		*layout;

		bool			needs_name;
		
		bool			snapmode;

		ModelSaveList		*savemodel;
		SPWAW_SAVELIST		*savelist;
#if	ALLOW_SNAPSHOTS_LOAD
		ModelSnapList		*snapmodel;
		SPWAW_SNAPLIST		*snaplist;
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	} d;

private slots:
	void	name_changed	(const QString& text);
	void	tree_clicked	(const QModelIndex& index);
	void	accept		(void);

private:
	void	constructor_core		(char *name, QString &type, QString &items);
	void	refresh_activation_status	(void);
};

#endif	/* GUI_DLG_ADD_BATTLE_SAVEGAME_H */
