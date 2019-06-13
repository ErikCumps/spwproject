/** \file
 * The SPWaW war cabinet - GUI - "dossier new" dialog box.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_DLG_NEW_DOSSIER_H
#define	GUI_DLG_NEW_DOSSIER_H	1

#include "gui_private.h"

class GuiDlgNewDossier	: public QDialog
{
	Q_OBJECT

public:
	GuiDlgNewDossier	(QList<CfgGameType> &gametypes, SPWAW_GAME_TYPE def_gametype);
	~GuiDlgNewDossier	(void);

	SL_ERROR	error_code;

public:
	void	data_get	(SPWAW_GAME_TYPE &gametype, QString &name, QString &comment);

private:
	struct s_data {
		QFont			*dlgf;
		QFontMetrics		*dlgfm;
		QDialogButtonBox	*buttons;
		QWidget			*body;
		QLabel			*gametype_label;
		QComboBox		*gametype_select;
		QLabel			*name_label;
		QLineEdit		*name_edit;
		QLabel			*comment_label;
		QTextEdit		*comment_edit;
		QGridLayout		*layout;
	} d;
};

#endif	/* GUI_DLG_NEW_DOSSIER_H */
