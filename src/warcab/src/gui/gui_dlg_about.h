/** \file
 * The SPWaW war cabinet - GUI - "about" dialog box.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_DLG_ABOUT_H
#define	GUI_DLG_ABOUT_H	1

#include "gui_private.h"

class GuiDlgAbout	: public QDialog
{
	Q_OBJECT

public:
	GuiDlgAbout	(void);
	~GuiDlgAbout	(void);

	SL_ERROR	error_code;

private:
	struct s_data {
		QFont			*dlgf;
		QFontMetrics		*dlgfm;
		QWidget			*body;
		QBrush			*body_brush;
		QPalette		*body_palette;
		QDialogButtonBox	*buttons;
		QLabel			*app_logo;
		QLabel			*app_info;
		QLabel			*message;
		QGridLayout		*layout;
	} d;
};

#endif	/* GUI_DLG_ABOUT_H */
