/** \file
 * The SPWaW war cabinet - GUI - "dossier edit" dialog box.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_DLG_EDIT_DOSSIER_H
#define	GUI_DLG_EDIT_DOSSIER_H	1

#include "gui_private.h"

class GuiDlgEditDossier	: public QDialog
{
	Q_OBJECT

public:
	GuiDlgEditDossier	(void);
	~GuiDlgEditDossier	(void);

	SL_ERROR	error_code;

public:
	void	data_set	(char *name, char *comment);
	void	data_get	(char **name, char **comment);

private:
	struct s_data {
		QFont			*dlgf;
		QFontMetrics		*dlgfm;
		QDialogButtonBox	*buttons;
		QWidget			*body;
		QLabel			*name_label;
		QLineEdit		*name_edit;
		QLabel			*comment_label;
		QTextEdit		*comment_edit;
		QGridLayout		*layout;
	} d;
};

#endif	/* GUI_DLG_EDIT_DOSSIER_H */
