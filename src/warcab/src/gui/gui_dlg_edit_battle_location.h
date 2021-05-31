/** \file
 * The SPWaW war cabinet - GUI - "battle location edit" dialog box.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_DLG_EDIT_BATTLE_LOCATION_H
#define	GUI_DLG_EDIT_BATTLE_LOCATION_H	1

#include "gui_private.h"

class GuiDlgEditBattleLocation	: public QDialog
{
	Q_OBJECT

public:
	GuiDlgEditBattleLocation	(char *location);
	~GuiDlgEditBattleLocation	(void);

	SL_ERROR	error_code;

public:
	void	get_data		(char **location);

private:
	struct s_data {
		QFont			*dlgf;
		QFontMetrics		*dlgfm;
		QDialogButtonBox	*buttons;
		QPushButton		*but_ok;
		QPushButton		*but_cancel;
		QWidget			*body;
		QLabel			*old_label;
		QLabel			*old_value;
		QLabel			*location_label;
		QLineEdit		*location_edit;
		QGridLayout		*layout;
	} d;

private slots:
	void	location_changed	(const QString& text);

private:
	void	refresh_button_status	(void);
};

#endif	/* GUI_DLG_EDIT_BATTLE_LOCATION_H */
