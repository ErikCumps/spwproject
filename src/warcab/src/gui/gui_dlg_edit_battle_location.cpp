/** \file
 * The SPWaW war cabinet - GUI - "battle location edit" dialog box.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_dlg_edit_battle_location.h"

#define	BOX_WIDTH	300
#define	BOX_HEIGHT	100
#define	BOX_MARGIN	 10

GuiDlgEditBattleLocation::GuiDlgEditBattleLocation (char *location)
	: QDialog (0, Qt::Dialog)
{
	int	but_height = 0;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Set dialog modality */
	setModal (true);

	/* Set dialog size */
	setFixedSize (BOX_WIDTH, BOX_HEIGHT);

	/* Set dialog caption and icon */
	setWindowTitle ("Edit Battle Location");
	setWindowIcon (*(RES_pixmap (RID_EDIT_DOSSIER)));

	/* Set dialog font */
	GUINEW (d.dlgf, QFont (font()), ERR_GUI_DLG_EDIT_BATTLE_LOCATION_INIT_FAILED, "font");
	d.dlgf->setFamily ("Tahoma");
	setFont (*d.dlgf);

	/* Get dialog font metrics */
	GUINEW (d.dlgfm, QFontMetrics (*d.dlgf), ERR_GUI_DLG_EDIT_BATTLE_LOCATION_INIT_FAILED, "font metrics");

	/* Create button box */
	GUINEW (d.buttons, QDialogButtonBox (this), ERR_GUI_DLG_EDIT_BATTLE_LOCATION_INIT_FAILED, "button box");
	but_height = d.buttons->height();
	d.buttons->setGeometry(QRect(BOX_MARGIN, BOX_HEIGHT - BOX_MARGIN - but_height, BOX_WIDTH - 2*BOX_MARGIN, but_height));
	d.buttons->setOrientation(Qt::Horizontal);

	/* Create and add buttons */
	GUINEW (d.but_ok, QPushButton (d.buttons), ERR_GUI_DLG_EDIT_BATTLE_LOCATION_INIT_FAILED, "ok button");
	d.but_ok->setText ("&OK");
	d.buttons->addButton (d.but_ok, QDialogButtonBox::AcceptRole);
	d.but_ok->setDefault (true);

	GUINEW (d.but_cancel, QPushButton (d.buttons), ERR_GUI_DLG_EDIT_BATTLE_LOCATION_INIT_FAILED, "cancel button");
	d.but_cancel->setText ("&Cancel");
	d.buttons->addButton (d.but_cancel, QDialogButtonBox::RejectRole);

	/* Create body widget */
	GUINEW (d.body, QWidget (this), ERR_GUI_DLG_EDIT_BATTLE_LOCATION_INIT_FAILED, "body widget");
	d.body->setGeometry(QRect(BOX_MARGIN, BOX_MARGIN, BOX_WIDTH - 2*BOX_MARGIN, BOX_HEIGHT - 3*BOX_MARGIN - but_height));

	/* Create "old" ui */
	GUINEW (d.old_label, QLabel (d.body), ERR_GUI_DLG_EDIT_BATTLE_LOCATION_INIT_FAILED, "old label");
	d.old_label->setText ("Old battle location:");

	GUINEW (d.old_value, QLabel (d.body), ERR_GUI_DLG_EDIT_BATTLE_LOCATION_INIT_FAILED, "old value");
	d.old_value->setText (QString("<b>%1</b>").arg(location));

	/* Create "location" ui */
	GUINEW (d.location_label, QLabel (d.body), ERR_GUI_DLG_EDIT_BATTLE_LOCATION_INIT_FAILED, "location label");
	d.location_label->setText ("New battle location:");

	GUINEW (d.location_edit, QLineEdit (d.body), ERR_GUI_DLG_EDIT_BATTLE_LOCATION_INIT_FAILED, "location lineedit");
	d.location_label->setBuddy (d.location_edit);

	/* Create body layout */
	GUINEW (d.layout, QGridLayout (d.body), ERR_GUI_DLG_EDIT_BATTLE_LOCATION_INIT_FAILED, "body layout");
	d.layout->setContentsMargins(0, 0, 0, 0);
	d.layout->addWidget (d.old_label,	0, 0, 1, 1);
	d.layout->addWidget (d.old_value,	0, 1, 1, 1);
	d.layout->addWidget (d.location_label,	1, 0, 1, 1);
	d.layout->addWidget (d.location_edit,	1, 1, 1, 1);

	/* Finally connect signals and slots */
	connect (d.buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect (d.buttons, SIGNAL(rejected()), this, SLOT(reject()));
	connect (d.location_edit, SIGNAL(textChanged(const QString&)), this, SLOT(location_changed(const QString&)));

	/* And set the focus */
	d.location_edit->setFocus(Qt::ActiveWindowFocusReason);

	refresh_button_status();

	SET_GUICLS_NOERR;
}

GuiDlgEditBattleLocation::~GuiDlgEditBattleLocation (void)
{
	// QT deletes child widgets
	delete d.dlgf;
	delete d.dlgfm;
}

void
GuiDlgEditBattleLocation::get_data (char **location)
{
	if (!location) return;

	if (d.location_edit->isModified()) {
		SL_SAFE_STRDUP (*location, qPrintable (d.location_edit->text()));
	} else {
		SL_SAFE_STRDUP (*location, qPrintable (d.old_value->text()));
	}
}

void
GuiDlgEditBattleLocation::refresh_button_status (void)
{
	bool	enable = !d.location_edit->text().isEmpty();

	d.but_ok->setEnabled(enable);
}

void
GuiDlgEditBattleLocation::location_changed (const QString& /*text*/)
{
	refresh_button_status();
}
