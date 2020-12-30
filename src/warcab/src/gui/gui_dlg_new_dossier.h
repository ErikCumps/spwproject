/** \file
 * The SPWaW war cabinet - GUI - "dossier new" dialog box.
 *
 * Copyright (C) 2019-2020 Erik Cumps <erik.cumps@gmail.com>
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
	GuiDlgNewDossier	(GameCfgList &gamecfg_list, int def_gamecfg);
	~GuiDlgNewDossier	(void);

	SL_ERROR	error_code;

public:
	void	data_get	(SPWAW_GAME_TYPE &gametype, QString &gamepath, QString &name, QString &comment);

private:
	struct s_data {
		QFont			*dlgf;
		QFontMetrics		*dlgfm;
		QDialogButtonBox	*buttons;
		QWidget			*body;
		QLabel			*gamecfg_label;
		QComboBox		*gamecfg_select;
		QLabel			*name_label;
		QLineEdit		*name_edit;
		QLabel			*comment_label;
		QTextEdit		*comment_edit;
		QGridLayout		*layout;
	} d;
	struct s_objects {
		GameCfgList		gamecfgs;
	} o;
};

#endif	/* GUI_DLG_NEW_DOSSIER_H */
