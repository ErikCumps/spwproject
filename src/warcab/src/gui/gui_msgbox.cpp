/** \file
 * The SPWaW war cabinet - GUI - simple message dialog box.
 *
 * Copyright (C) 2017 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "../common.h"
#include "gui_msgbox.h"

#include <builtin/windows_icon_genappl.xpm>
#include <builtin/windows_icon_question.xpm>
#include <builtin/windows_icon_information.xpm>
#include <builtin/windows_icon_warning.xpm>
#include <builtin/windows_icon_critical.xpm>

GuiMsgbox::GuiMsgbox (MSGBOX_TYPE type, char *title, char *msg)
	: QDialog (0, Qt::Dialog)
{
	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Prep strings */
	d.title = title;
	if (!d.title) d.title = "Info";

	d.msg = msg;
	if (!d.msg) d.msg = "Click <Ok> to leave this message box.";

	/* Create dialog resources */
	d.dlg_icon = RES_pixmap (RID_MAIN_ICON);
	switch (type) {
		case MSGBOX_ERROR:
			d.msg_icon = new QPixmap (windows_icon_critical_xpm);
			break;
		case MSGBOX_WARNING:
			d.msg_icon = new QPixmap (windows_icon_warning_xpm);
			break;
		case MSGBOX_INFO:
			d.msg_icon = new QPixmap (windows_icon_information_xpm);
			break;
	}

	/* Set dialog modality and size */
	setModal (true);

	/* Set dialog caption and icon */
	setWindowTitle (d.title);
	setWindowIcon (*d.dlg_icon);

	/* Set dialog font */
	d.dlg_font = new QFont (font());
	d.dlg_font->setFamily ("Tahoma");
	setFont (*d.dlg_font);

	/* Set message font */
	d.msg_font = new QFont (font());
	//d.msg_font->setWeight (QFont::Bold);

	/* Create body layout */
	d.body_layout = new QGridLayout (this);
	d.body_layout->setContentsMargins(10, 10, 10, 10);
	d.body_layout->setSpacing (10);
	d.body_layout->setColumnMinimumWidth (0, 32);

	/* Create body icon */
	d.body_icon = new QLabel (this);
	d.body_icon->setPixmap (*d.msg_icon);
	d.body_layout->addWidget (d.body_icon, 0, 0, 1, 1, Qt::AlignTop);

	/* Create body text */
	d.body_msg = new QLabel (this);
	d.body_msg->setText (d.msg);
	d.body_msg->setFont (*d.msg_font);
	d.body_layout->addWidget (d.body_msg, 0, 1, 1, 1);

	/* Create button box */
	d.dlg_buttons = new QDialogButtonBox (Qt::Horizontal, this);
	d.body_layout->addWidget (d.dlg_buttons, 2, 0, 1, 2, Qt::AlignRight);

	/* Create and add buttons */
	d.but_ok = new QPushButton (d.dlg_buttons);
	d.but_ok->setText ("&Ok");
	d.but_ok->setDefault (true);

	/* Add buttons to button box */
	d.dlg_buttons->addButton (d.but_ok, QDialogButtonBox::AcceptRole);

	/* Finally connect signals and slots */
	connect (d.but_ok, SIGNAL (clicked (bool)), this, SLOT(clicked_ok (bool)));
	connect (d.dlg_buttons, SIGNAL(accepted()), this, SLOT(accept()));
}

GuiMsgbox::~GuiMsgbox (void)
{
	// QT deletes child widgets
	if (d.msg_icon) delete (d.msg_icon);
	if (d.dlg_font) delete (d.dlg_font);
	if (d.msg_font) delete (d.msg_font);
}

void
GuiMsgbox::clicked_ok (bool)
{
	accepted ();
}


void
GUI_msgbox (MSGBOX_TYPE type, char *title, char *msg)
{
	GuiMsgbox	*box;

	// TODO: handle out of memory case?
	box = new GuiMsgbox (type, title, msg);
	(void)box->exec (); delete (box);
}
