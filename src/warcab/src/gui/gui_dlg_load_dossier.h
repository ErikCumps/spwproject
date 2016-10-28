/** \file
 * The SPWaW war cabinet - GUI - "dossier load" dialog box.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_DLG_LOAD_DOSSIER_H
#define	GUI_DLG_LOAD_DOSSIER_H	1

#include "gui_private.h"
#include "model/model_dosslist.h"

class GuiDlgLoadDossier	: public QDialog
{
	Q_OBJECT

public:
	GuiDlgLoadDossier	(char *path, SPWAW_DOSSLIST *ignore);
	~GuiDlgLoadDossier	(void);

	SL_ERROR	error_code;

public:
	void	data_get	(SPWAW_DOSSLIST *data);

private:
	struct s_data {
		QFont			*dlgf;
		QFontMetrics		*dlgfm;
		QDialogButtonBox	*buttons;
		QWidget			*body;
		ModelDossList		*model;
		QTreeView		*view;
		QGridLayout		*layout;

		SPWAW_DOSSLIST		*data;
	} d;

private slots:
	void	clicked	(const QModelIndex& index);
	void	accept	(void);
};

#endif	/* GUI_DLG_LOAD_DOSSIER_H */
