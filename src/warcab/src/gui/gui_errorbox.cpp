/** \file
 * The SPWaW war cabinet - GUI - error dialog box.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "../common.h"
#include "gui_errorbox.h"

#include <builtin/windows_icon_genappl.xpm>
#include <builtin/windows_icon_question.xpm>
#include <builtin/windows_icon_information.xpm>
#include <builtin/windows_icon_warning.xpm>
#include <builtin/windows_icon_critical.xpm>

/* HOWTO: set widget background color:
 *
 *	widget->setPalette (Qt::color);
 *	widget->setAutoFillBackground (true);
 */

#ifdef	_DEBUG

#define	BOX_WIDTH	700
#define	BOX_HEIGHT	400

#else	/* _DEBUG */

#define	BOX_WIDTH	700
#define	BOX_HEIGHT	300

#endif	/* !_DEBUG */


GuiErrorbox::GuiErrorbox (SL_ERROR_REQUEST *rc, SL_ERROR_FATAL fc,  char *msg, char *extra_type, char *extra_data)
	: QDialog (0, Qt::Dialog)
{
	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Set default reply */
	d.rc = rc;
	if (d.rc) *rc = SL_ERR_REQUEST_ACCEPT;

	/* Prep strings */
	d.fc_msg = SL_ERROR_fatality_lookup (fc, 0);

	d.msg = msg;
	d.extra_type = extra_type;
	d.extra_data = extra_data;

	if (!d.msg) d.msg = "No information available.";
	if (!d.extra_type) d.extra_type = "Extra error";
	if (!d.extra_data) d.extra_data = "No additional information available.";

	memset (d.extra_label, 0, sizeof (d.extra_label));
	snprintf (d.extra_label, sizeof (d.extra_label) - 1, "%s information:", d.extra_type);

	/* Create dialog resources */
	d.dlg_icon = RES_pixmap (RID_MAIN_ICON);
	switch (fc) {
		case SL_ERR_FATAL_WARN:
			d.fc_icon = new QPixmap (windows_icon_warning_xpm);
			break;
		case SL_ERR_FATAL_SOFTERR:
			d.fc_icon = new QPixmap (windows_icon_warning_xpm);
			break;
		case SL_ERR_FATAL_ERR:
			d.fc_icon = new QPixmap (windows_icon_critical_xpm);
			break;
		case SL_ERR_FATAL_PANIC:
			d.fc_icon = new QPixmap (windows_icon_critical_xpm);
			break;
		case SL_ERR_FATAL_USAGE:
			d.fc_icon = new QPixmap (windows_icon_information_xpm);
			break;
		default:
			d.fc_icon = new QPixmap (windows_icon_warning_xpm);
			break;
	}

	/* Set dialog modality and size */
	setModal (true);

	/* Set dialog caption and icon */
	setWindowTitle (d.fc_msg);
	setWindowIcon (*d.dlg_icon);

	/* Set dialog font */
	d.dlg_font = new QFont (font());
	d.dlg_font->setFamily ("Tahoma");
	setFont (*d.dlg_font);

	/* Set message font */
	d.msg_font = new QFont (font());
	d.msg_font->setWeight (QFont::Bold);

	/* Create body layout */
	d.body_layout = new QGridLayout (this);
	d.body_layout->setContentsMargins(10, 10, 10, 10);
	d.body_layout->setSpacing (10);
	d.body_layout->setColumnMinimumWidth (0, 32);

	/* Create body icon */
	d.body_icon = new QLabel (this);
	d.body_icon->setPixmap (*d.fc_icon);
	d.body_layout->addWidget (d.body_icon, 0, 0, 1, 1);

	/* Create body text */
	d.body_msg = new QLabel (this);
	d.body_msg->setText (d.msg);
	d.body_msg->setFont (*d.msg_font);
	d.body_layout->addWidget (d.body_msg, 0, 1, 1, 1);

	/* Create extra data label */
	d.body_extra_label = new QLabel (this);
	d.body_extra_label->setText (d.extra_label);
	d.body_layout->addWidget (d.body_extra_label, 1, 1, 1, 1);

	/* Create extra data font */
	d.body_extra_font = new QFont ("Courier", 8, QFont::Normal, false);

	/* Create extra data text */
	d.body_extra_data = new QTextEdit (this);
	d.body_extra_data->setReadOnly (true);
	d.body_extra_data->setLineWrapMode (QTextEdit::NoWrap);
	d.body_extra_data->setCurrentFont (*d.body_extra_font);
	d.body_extra_data->setText (d.extra_data);
	d.body_extra_data->setSizePolicy (QSizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	d.body_layout->addWidget (d.body_extra_data, 2, 1, 1, 1);

	/* Create button box */
	d.dlg_buttons = new QDialogButtonBox (Qt::Horizontal, this);
	d.body_layout->addWidget (d.dlg_buttons, 3, 1, 1, 1);

	/* Create and add buttons */
	d.but_ok = new QPushButton (d.dlg_buttons);
	d.but_ok->setText ("&Ok");
	d.but_ok->setDefault (true);

	d.but_abort = new QPushButton (d.dlg_buttons);
	d.but_abort->setText ("&Abort");

	d.but_retry = new QPushButton (d.dlg_buttons);
	d.but_retry->setText ("&Retry");

	d.but_ignore = new QPushButton (d.dlg_buttons);
	d.but_ignore->setText ("&Ignore");

	/* Add buttons to button box */
	switch (fc) {
		case SL_ERR_FATAL_WARN:
			d.dlg_buttons->addButton (d.but_ok, QDialogButtonBox::AcceptRole);
			d.but_abort->hide();
			d.but_retry->hide();
			d.but_ignore->hide();
			break;
		case SL_ERR_FATAL_SOFTERR:
			d.but_ok->hide();
			d.dlg_buttons->addButton (d.but_abort, QDialogButtonBox::AcceptRole);
			d.dlg_buttons->addButton (d.but_retry, QDialogButtonBox::AcceptRole);
#if	ALLOW_ERROR_IGNORE
			d.dlg_buttons->addButton (d.but_ignore, QDialogButtonBox::AcceptRole);
#else	/* !ALLOW_ERROR_IGNORE */
			d.but_ignore->hide();
#endif	/* !ALLOW_ERROR_IGNORE */
			break;
		case SL_ERR_FATAL_USAGE:
			d.dlg_buttons->addButton (d.but_ok, QDialogButtonBox::AcceptRole);
			d.but_abort->hide();
			d.but_retry->hide();
			d.but_ignore->hide();
			break;
		case SL_ERR_FATAL_ERR:
		case SL_ERR_FATAL_PANIC:
		default:
			d.but_ok->hide();
			d.dlg_buttons->addButton (d.but_abort, QDialogButtonBox::AcceptRole);
			d.but_retry->hide();
			d.but_ignore->hide();
			break;
	}

	/* Finally connect signals and slots */
	connect (d.but_ok, SIGNAL (clicked (bool)), this, SLOT(clicked_ok (bool)));
	connect (d.but_abort, SIGNAL (clicked (bool)), this, SLOT(clicked_abort (bool)));
	connect (d.but_retry, SIGNAL (clicked (bool)), this, SLOT(clicked_retry (bool)));
	connect (d.but_ignore, SIGNAL (clicked (bool)), this, SLOT(clicked_ignore (bool)));
	connect (d.dlg_buttons, SIGNAL(accepted()), this, SLOT(accept()));

	resize (BOX_WIDTH, BOX_HEIGHT);
	setSizeGripEnabled (true);
}

GuiErrorbox::~GuiErrorbox (void)
{
	// QT deletes child widgets
	if (!d.but_ok->parentWidget()) delete (d.but_ok);
	if (!d.but_retry->parentWidget()) delete (d.but_retry);
	if (!d.but_ignore->parentWidget()) delete (d.but_ignore);
	if (d.fc_icon) delete (d.fc_icon);
	if (d.dlg_font) delete (d.dlg_font);
	if (d.msg_font) delete (d.msg_font);
	if (d.body_extra_font) delete (d.body_extra_font);
}

void
GuiErrorbox::clicked_ok (bool)
{
	if (d.rc) *d.rc = SL_ERR_REQUEST_ACCEPT;
	accepted ();
}

void
GuiErrorbox::clicked_abort (bool)
{
	if (d.rc) *d.rc = SL_ERR_REQUEST_ACCEPT;
	accepted ();
}

void
GuiErrorbox::clicked_retry (bool)
{
	if (d.rc) *d.rc = SL_ERR_REQUEST_RETRY;
	accepted ();
}

void
GuiErrorbox::clicked_ignore (bool)
{
	if (d.rc) *d.rc = SL_ERR_REQUEST_IGNORE;
	accepted ();
}

SL_ERROR_REQUEST
GUI_errorbox (SL_ERROR_FATAL fc, char *msg, char *extra_type, char *extra_data)
{
	GuiErrorbox		*box;
	SL_ERROR_REQUEST	rq = SL_ERR_REQUEST_ACCEPT;

	// TODO: handle out of memory case?
	box = new GuiErrorbox (&rq, fc, msg, extra_type, extra_data);
	(void)box->exec (); delete (box);

	return (rq);
}
