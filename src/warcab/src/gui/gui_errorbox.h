/** \file
 * The SPWaW war cabinet - GUI - error dialog box.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_ERRORBOX_H
#define	GUI_ERRORBOX_H	1

class GuiErrorbox	: public QDialog
{
	Q_OBJECT

public:
	GuiErrorbox	(SL_ERROR_REQUEST *rc, SL_ERROR_FATAL fc, char *msg, char *extra_type, char *extra_data);
	~GuiErrorbox	(void);

private:
	struct s_data {
		SL_ERROR_REQUEST	*rc;
		const char		*fc_msg;
		char			*msg;
		char			*extra_type;
		char			*extra_data;
		char			extra_label[256];
	        QPixmap			*dlg_icon;
		QPixmap			*fc_icon;
		QFont			*dlg_font;
		QFont			*msg_font;
		QGridLayout		*body_layout;
		QLabel			*body_icon;
		QLabel			*body_msg;
		QLabel			*body_extra_label;
		QFont			*body_extra_font;
		QTextEdit		*body_extra_data;
		QDialogButtonBox	*dlg_buttons;
		QPushButton		*but_ok;
		QPushButton		*but_abort;
		QPushButton		*but_retry;
		QPushButton		*but_ignore;
	} d;

private slots:
	void	clicked_ok	(bool);
	void	clicked_abort	(bool);
	void	clicked_retry	(bool);
	void	clicked_ignore	(bool);
};

extern SL_ERROR_REQUEST	GUI_errorbox	(SL_ERROR_FATAL fc, char *msg, char *extra_type, char *extra_data);

#endif	/* GUI_ERRORBOX_H */

