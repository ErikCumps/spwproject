/** \file
 * The SPWaW war cabinet - GUI - "dossier new" dialog box.
 *
 * Copyright (C) 2019-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_dlg_new_dossier.h"

#define	BOX_WIDTH	600
#define	BOX_HEIGHT	300
#define	BOX_MARGIN	 10

GuiDlgNewDossier::GuiDlgNewDossier (GameCfgList &gamecfg_list, int def_gamecfg)
	: QDialog (0, Qt::Dialog)
{
	int	but_height = 0;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Keep track of gamecfgs */
	o.gamecfgs = gamecfg_list;

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

	/* Create "gamecfg" ui */
	GUINEW (d.gamecfg_label, QLabel (d.body), ERR_GUI_DLG_NEW_DOSSIER_INIT_FAILED, "gamecfg label");
	d.gamecfg_label->setText ("Game config:");

	GUINEW (d.gamecfg_select, QComboBox (d.body), ERR_GUI_DLG_NEW_DOSSIER_INIT_FAILED, "gamecfg select");
	d.gamecfg_select->setEditable (false);
	for (int i=0; i<o.gamecfgs.size(); i++) {
		d.gamecfg_select->addItem (QString("%1 (%2)").arg(o.gamecfgs[i].name, SPWAW_gametype2str(o.gamecfgs[i].type)), i);
	}
	if (def_gamecfg != -1) d.gamecfg_select->setCurrentIndex(def_gamecfg);

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
	d.layout->addWidget (d.gamecfg_label,	0, 0, 1, 1);
	d.layout->addWidget (d.gamecfg_select,	0, 1, 1, 1);
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
GuiDlgNewDossier::data_get (SPWAW_GAME_TYPE &gametype, QString &gamepath, QString &name, QString &comment)
{
	int idx = d.gamecfg_select->itemData (d.gamecfg_select->currentIndex()).toInt();

	gametype = o.gamecfgs[idx].type;
	gamepath = o.gamecfgs[idx].path;
	name	 = d.name_edit->text();
	comment	 = d.comment_edit->toPlainText();
}
