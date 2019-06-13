/** \file
 * The SPWaW war cabinet - GUI - "dossier new" dialog box.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_dlg_new_dossier.h"

#define	BOX_WIDTH	600
#define	BOX_HEIGHT	300
#define	BOX_MARGIN	 10

GuiDlgNewDossier::GuiDlgNewDossier (QList<CfgGameType> &gametypes, SPWAW_GAME_TYPE def_gametype)
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
	setWindowTitle ("New Dossier");
	setWindowIcon (*(RES_pixmap (RID_GUI_ICONS_1_New)));

	/* Set dialog font */
	GUINEW (d.dlgf, QFont (font()), ERR_GUI_DLG_NEW_DOSSIER_INIT_FAILED, "font");
	d.dlgf->setFamily ("Tahoma");
	setFont (*d.dlgf);

	/* Get dialog font metrics */
	GUINEW (d.dlgfm, QFontMetrics (*d.dlgf), ERR_GUI_DLG_NEW_DOSSIER_INIT_FAILED, "font metrics");

	/* Create button box */
	GUINEW (d.buttons, QDialogButtonBox (this), ERR_GUI_DLG_NEW_DOSSIER_INIT_FAILED, "button box");
	but_height = d.buttons->height();
	d.buttons->setGeometry(QRect(BOX_MARGIN, BOX_HEIGHT - BOX_MARGIN - but_height, BOX_WIDTH - 2*BOX_MARGIN, but_height));
	d.buttons->setOrientation(Qt::Horizontal);
	d.buttons->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);

	/* Create body widget */
	GUINEW (d.body, QWidget (this), ERR_GUI_DLG_NEW_DOSSIER_INIT_FAILED, "body widget");
	d.body->setGeometry(QRect(BOX_MARGIN, BOX_MARGIN, BOX_WIDTH - 2*BOX_MARGIN, BOX_HEIGHT - 3*BOX_MARGIN - but_height));

	/* Create "gametype" ui */
	GUINEW (d.gametype_label, QLabel (d.body), ERR_GUI_DLG_NEW_DOSSIER_INIT_FAILED, "gametype label");
	d.gametype_label->setText ("Game type:");

	GUINEW (d.gametype_select, QComboBox (d.body), ERR_GUI_DLG_NEW_DOSSIER_INIT_FAILED, "gametype select");
	d.gametype_select->setEditable (false);
	for (int i=0; i<gametypes.size(); i++) {
		d.gametype_select->addItem (gametypes[i].name, (uint)gametypes[i].type);
		if (gametypes[i].type == def_gametype) d.gametype_select->setCurrentIndex(i);
	}

	/* Create "name" ui */
	GUINEW (d.name_label, QLabel (d.body), ERR_GUI_DLG_NEW_DOSSIER_INIT_FAILED, "name label");
	d.name_label->setText ("Title:");

	GUINEW (d.name_edit, QLineEdit (d.body), ERR_GUI_DLG_NEW_DOSSIER_INIT_FAILED, "name lineedit");
	d.name_label->setBuddy (d.name_edit);
	d.name_edit->setText (DEFAULT_NEW_NAME);

	/* Create "comment" ui */
	GUINEW (d.comment_label, QLabel (d.body), ERR_GUI_DLG_NEW_DOSSIER_INIT_FAILED, "comment label");
	d.comment_label->setText ("Comment:");

	GUINEW (d.comment_edit, QTextEdit (d.body), ERR_GUI_DLG_NEW_DOSSIER_INIT_FAILED, "comment textedit");
	d.comment_edit->setAcceptRichText (false);
	d.comment_label->setBuddy (d.comment_edit);
	d.comment_edit->setText (DEFAULT_NEW_COMMENT);

	/* Create body layout */
	GUINEW (d.layout, QGridLayout (d.body), ERR_GUI_DLG_NEW_DOSSIER_INIT_FAILED, "body layout");
	d.layout->setContentsMargins(0, 0, 0, 0);
	d.layout->addWidget (d.gametype_label,	0, 0, 1, 1);
	d.layout->addWidget (d.gametype_select,	0, 1, 1, 1);
	d.layout->addWidget (d.name_label,	1, 0, 1, 2);
	d.layout->addWidget (d.name_edit,	2, 0, 1, 2);
	d.layout->addWidget (d.comment_label,	3, 0, 1, 2);
	d.layout->addWidget (d.comment_edit,	4, 0, 1, 2);

	/* Finally connect signals and slots */
	connect (d.buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect (d.buttons, SIGNAL(rejected()), this, SLOT(reject()));

	SET_GUICLS_NOERR;
}

GuiDlgNewDossier::~GuiDlgNewDossier (void)
{
	// QT deletes child widgets
	delete d.dlgf;
	delete d.dlgfm;
}

void
GuiDlgNewDossier::data_get (SPWAW_GAME_TYPE &gametype, QString &name, QString &comment)
{
	gametype = (SPWAW_GAME_TYPE)(d.gametype_select->itemData (d.gametype_select->currentIndex()).toUInt());
	name	 = d.name_edit->text();
	comment	 = d.comment_edit->toPlainText();
}
