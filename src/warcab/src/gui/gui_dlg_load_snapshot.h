/** \file
 * The SPWaW war cabinet - GUI - "snapshot load" dialog box.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_DLG_LOAD_SNAPSHOT_H
#define	GUI_DLG_LOAD_SNAPSHOT_H	1

#include "gui_private.h"
#include "model/model_snaplist.h"

class GuiDlgLoadSnapshot	: public QDialog
{
	Q_OBJECT

public:
	GuiDlgLoadSnapshot	(char *path, SPWAW_SNAPLIST *ignore);
	~GuiDlgLoadSnapshot	(void);

	SL_ERROR	error_code;

public:
	void	data_get	(SPWAW_SNAPLIST *data);

private:
	struct s_data {
		QFont			*dlgf;
		QFontMetrics		*dlgfm;
		QDialogButtonBox	*buttons;
		QWidget			*body;
		ModelSnapList		*model;
		QTreeView		*view;
		QGridLayout		*layout;

		SPWAW_SNAPLIST		*data;
	} d;

private slots:
	void	clicked	(const QModelIndex& index);
	void	accept	(void);
};

#endif	/* GUI_DLG_LOAD_SNAPSHOT_H */
