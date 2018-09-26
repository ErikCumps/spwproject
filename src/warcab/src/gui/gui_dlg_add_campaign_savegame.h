/** \file
 * The SPWaW war cabinet - GUI - "add campaign savegame" dialog box.
 *
 * Copyright (C) 2005-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_DLG_ADD_CAMPAIGN_SAVEGAME_H
#define	GUI_DLG_ADD_CAMPAIGN_SAVEGAME_H	1

#include "gui_private.h"
#include "model/model_savelist.h"

#if	ALLOW_SNAPSHOTS_LOAD
#include "model/model_snaplist.h"
#endif	/* ALLOW_SNAPSHOTS_LOAD */

class GuiDlgAddCampaignSavegame	: public QDialog
{
	Q_OBJECT

public:
	GuiDlgAddCampaignSavegame	(char *path, SPWAW_SAVELIST *ignore);
#if	ALLOW_SNAPSHOTS_LOAD
	GuiDlgAddCampaignSavegame	(char *path, SPWAW_SNAPLIST *ignore);
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	~GuiDlgAddCampaignSavegame	(void);

	SL_ERROR	error_code;

public:
	void	get_data	(SPWAW_SAVELIST *list);
#if	ALLOW_SNAPSHOTS_LOAD
	void	get_data	(SPWAW_SNAPLIST *list);
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
		QTreeView		*view;
		QGridLayout		*layout;

		bool			snapmode;

		ModelSaveList		*savemodel;
		SPWAW_SAVELIST		*savelist;
#if	ALLOW_SNAPSHOTS_LOAD
		ModelSnapList		*snapmodel;
		SPWAW_SNAPLIST		*snaplist;
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	} d;

private slots:
	void	selection_changed	(const QItemSelection &selected, const QItemSelection &deselected);
	void	accept			(void);

private:
	void	constructor_core		(QString &type, QString &items);
	void	refresh_ok_button_status	(void);
};

#endif	/* GUI_DLG_ADD_CAMPAIGN_SAVEGAME_H */
