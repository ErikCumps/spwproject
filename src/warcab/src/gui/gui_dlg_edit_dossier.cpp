/** \file
 * The SPWaW war cabinet - GUI - "dossier edit" dialog box.
 *
 * Copyright (C) 2005-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_dlg_edit_dossier.h"

#define	BOX_WIDTH	600
#define	BOX_HEIGHT	300
#define	BOX_MARGIN	 10

GuiDlgEditDossier::GuiDlgEditDossier (void)
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
	setWindowTitle ("Edit Dossier");
	setWindowIcon (*(RES_pixmap (RID_EDIT_DOSSIER)));

	/* Set dialog font */
	GUINEW (d.dlgf, QFont (font()), ERR_GUI_DLG_EDIT_DOSSIER_INIT_FAILED, "font");
	d.dlgf->setFamily ("Tahoma");
	setFont (*d.dlgf);

	/* Get dialog font metrics */
	GUINEW (d.dlgfm, QFontMetrics (*d.dlgf), ERR_GUI_DLG_EDIT_DOSSIER_INIT_FAILED, "font metrics");

	/* Create button box */
	GUINEW (d.buttons, QDialogButtonBox (this), ERR_GUI_DLG_EDIT_DOSSIER_INIT_FAILED, "button box");
	but_height = d.buttons->height();
	d.buttons->setGeometry(QRect(BOX_MARGIN, BOX_HEIGHT - BOX_MARGIN - but_height, BOX_WIDTH - 2*BOX_MARGIN, but_height));
	d.buttons->setOrientation(Qt::Horizontal);
	d.buttons->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);

	/* Create body widget */
	GUINEW (d.body, QWidget (this), ERR_GUI_DLG_EDIT_DOSSIER_INIT_FAILED, "body widget");
	d.body->setGeometry(QRect(BOX_MARGIN, BOX_MARGIN, BOX_WIDTH - 2*BOX_MARGIN, BOX_HEIGHT - 3*BOX_MARGIN - but_height));

	/* Create "name" ui */
	GUINEW (d.name_label, QLabel (d.body), ERR_GUI_DLG_EDIT_DOSSIER_INIT_FAILED, "name label");
	d.name_label->setText ("Title:");

	GUINEW (d.name_edit, QLineEdit (d.body), ERR_GUI_DLG_EDIT_DOSSIER_INIT_FAILED, "name lineedit");
	d.name_label->setBuddy (d.name_edit);

	/* Create "comment" ui */
	GUINEW (d.comment_label, QLabel (d.body), ERR_GUI_DLG_EDIT_DOSSIER_INIT_FAILED, "comment label");
	d.comment_label->setText ("Comment:");

	GUINEW (d.comment_edit, QTextEdit (d.body), ERR_GUI_DLG_EDIT_DOSSIER_INIT_FAILED, "comment textedit");
	d.comment_edit->setAcceptRichText (false);
	d.comment_label->setBuddy (d.comment_edit);

	/* Create body layout */
	GUINEW (d.layout, QGridLayout (d.body), ERR_GUI_DLG_EDIT_DOSSIER_INIT_FAILED, "body layout");
	d.layout->setContentsMargins(0, 0, 0, 0);
	d.layout->addWidget (d.name_label,	0, 0, 1, 1);
	d.layout->addWidget (d.name_edit,	1, 0, 1, 1);
	d.layout->addWidget (d.comment_label,	2, 0, 1, 1);
	d.layout->addWidget (d.comment_edit,	3, 0, 1, 1);

	/* Finally connect signals and slots */
	connect (d.buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect (d.buttons, SIGNAL(rejected()), this, SLOT(reject()));

	SET_GUICLS_NOERR;
}

GuiDlgEditDossier::~GuiDlgEditDossier (void)
{
	// QT deletes child widgets
	delete d.dlgf;
	delete d.dlgfm;
}

void
GuiDlgEditDossier::data_set (char *name, char *comment)
{
	d.name_edit->setText (name);
	d.comment_edit->setPlainText (comment);
}

void
GuiDlgEditDossier::data_get (char **name, char **comment)
{
	if (!name || !comment) return;
	*name = *comment = NULL;

	if (d.name_edit->isModified()) {
		SL_SAFE_STRDUP (*name, qPrintable (d.name_edit->text()));
	}
	if (d.comment_edit->document()->isModified()) {
		SL_SAFE_STRDUP (*comment, qPrintable (d.comment_edit->toPlainText()));
	}
}
