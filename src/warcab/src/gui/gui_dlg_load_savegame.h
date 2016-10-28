/** \file
 * The SPWaW war cabinet - GUI - "savegame load" dialog box.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_DLG_LOAD_SAVEGAME_H
#define	GUI_DLG_LOAD_SAVEGAME_H	1

#include "gui_private.h"
#include "model/model_savelist.h"

class GuiDlgLoadSavegame	: public QDialog
{
	Q_OBJECT

public:
	GuiDlgLoadSavegame	(char *path, SPWAW_SAVELIST *ignore);
	~GuiDlgLoadSavegame	(void);

	SL_ERROR	error_code;

public:
	void	data_get	(SPWAW_SAVELIST *data);

private:
	struct s_data {
		QFont			*dlgf;
		QFontMetrics		*dlgfm;
		QDialogButtonBox	*buttons;
		QWidget			*body;
		ModelSaveList		*model;
		QTreeView		*view;
		QGridLayout		*layout;

		SPWAW_SAVELIST		*data;
	} d;

private slots:
	void	clicked	(const QModelIndex& index);
	void	accept	(void);
};

#endif	/* GUI_DLG_LOAD_SAVEGAME_H */
