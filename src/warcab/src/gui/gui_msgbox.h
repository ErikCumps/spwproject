/** \file
 * The SPWaW war cabinet - GUI - simple message dialog box.
 *
 * Copyright (C) 2017-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_MSGBOX_H
#define	GUI_MSGBOX_H	1

typedef enum e_MSGBOX_TYPE {
	MSGBOX_ERROR = 0,
	MSGBOX_WARNING,
	MSGBOX_INFO,
	MSGBOX_CONFIRM
} MSGBOX_TYPE;

class GuiMsgbox	: public QDialog
{
	Q_OBJECT

public:
	GuiMsgbox	(MSGBOX_TYPE type, char *title, char *msg);
	~GuiMsgbox	(void);

private:
	struct s_data {
		char			*title;
		char			*msg;
	        QPixmap			*dlg_icon;
		QPixmap			*msg_icon;
		QFont			*dlg_font;
		QFont			*msg_font;
		QGridLayout		*body_layout;
		QLabel			*body_icon;
		QLabel			*body_msg;
		QDialogButtonBox	*dlg_buttons;
		QPushButton		*but_ok;
		QPushButton		*but_cancel;
	} d;

private slots:
	void	clicked_ok	(bool);
	void	clicked_cancel	(bool);
};

extern int	GUI_msgbox	(MSGBOX_TYPE type, char *title, char *msg);

#endif	/* GUI_MSGBOX_H */

