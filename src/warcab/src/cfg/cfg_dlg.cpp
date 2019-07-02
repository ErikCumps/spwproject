/** \file
 * The SPWaW war cabinet - configuration dialog.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "../common.h"
#include "res/res.h"
#include "cfg_dlg.h"

#define	BOX_WIDTH	600
#define	BOX_HEIGHT	400
#define	BOX_MARGIN	 10

#define	STR_LOCPRF_TOOLTIP	"Store preferences locally."
#define	STR_LOCPRF_WHATSTHIS							\
	"Select this to store the preferences locally in a \"warcab.ini\"\n"	\
	"file, allowing installations to have their own preferences.\n"		\
	"\n"									\
	"Disable this to store the preferences globally in the registry,\n"	\
	"so that they can be shared between different installations.\n"
#define	STR_LOCPRF_OVERWRITE							\
	"Unchecking the \"Store preferences locally\" checkbox will\n"		\
	"cause any existing global preferences to be overwritten\n"		\
	"with the local preferences!\n"						\
	"\n"									\
	"If this is not desired, please select <Cancel> to revert\n"		\
	"to storing the preferences locally...\n"

#define	STR_DGT_NAME		"Default game type:"
#define	STR_DGT_TOOLTIP		"Configure the default game type."
#define	STR_DGT_WHATSTHIS	"This sets the default game type for new dossiers."

#define	STR_BOX_NAME		"%1 game support"

#define	STR_OOB_NAME		"OOB files:"
#define	STR_OOB_TOOLTIP		"Configure the folder containing the %1 OOB files."
#define	STR_OOB_WHATSTHIS1	"This should be the folder containing the %1 OOB files."
#define	STR_OOB_WHATSTHIS2	"Press this button to browse to the folder contaning the %1 OOB files."
#define	STR_OOB_BROWSE		"Specify the %1 OOB folder:"

#define	STR_SVE_NAME		"savegames:"
#define	STR_SVE_TOOLTIP		"Configure the folder containg the %1 savegames."
#define	STR_SVE_WHATSTHIS1	"This should be the folder containing the %1 savegames."
#define	STR_SVE_WHATSTHIS2	"Press this button to browse to the folder contaning the %1 savegames."
#define	STR_SVE_BROWSE		"Specify the %1 savegames folder:"

#define	STR_SNP_TOOLTIP		"Configure the Warcab saves folder."
#define	STR_SNP_WHATSTHIS1	"This is the folder where warcab saves dossiers and stratmap images."
#define	STR_SNP_WHATSTHIS2	"Press this button to browse to the Warcab save folder."

#define	STR_COMPRESS_TOOLTIP	"Configure dossier compression."
#define	STR_COMPRESS_WHATSTHIS							\
	"Select this to enable compression when saving files.\n"		\
	"Disable this if you would like easier file inspection\n"		\
	"of *.warcab files."

#define	STR_AUTOLOAD_TOOLTIP	"Configure reloading last dossier at startup."
#define	STR_AUTOLOAD_WHATSTHIS							\
	"Select this to automatically reload the last open dossier at\n"	\
	"startup. Disable this if you would like to start Warcab with\n"	\
	"no open dossier."

CfgDlg::CfgDlg (CfgDlgData *data)
	: QDialog (0, Qt::Dialog)
{
	int	but_height = 0;
	int	row = 0;

	/* Initialize */
	memset (&d, 0, sizeof (d));
	d.dlg_data = data;

	/* Set dialog modality */
	setModal (true);

	/* Set dialog size */
	setFixedSize (BOX_WIDTH, BOX_HEIGHT);

	/* Set dialog caption and icon */
	setWindowTitle ("Warcab preferences");
	d.dlgi = RES_pixmap (RID_MAIN_ICON);
	setWindowIcon (*d.dlgi);

	/* Set dialog font */
	d.dlgf = new QFont (font());
	d.dlgf->setFamily ("Tahoma");
	setFont (*d.dlgf);

	/* Get dialog font metrics */
	d.dlgfm = new QFontMetrics (*d.dlgf);

	/* Calculate browse button dimensions */
	d.bbw = d.dlgfm->width ("...") + 10;
	d.bbh = d.dlgfm->height ();
	if (d.bbw < d.bbh) d.bbw = d.bbh; if (d.bbh < d.bbw) d.bbh = d.bbw;

	/* Create button box */
	d.buttons = new QDialogButtonBox (this);
	but_height = d.buttons->height();
	d.buttons->setGeometry(QRect(BOX_MARGIN, BOX_HEIGHT - BOX_MARGIN - but_height, BOX_WIDTH - 2*BOX_MARGIN, but_height));
	d.buttons->setOrientation(Qt::Horizontal);
	d.buttons->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);

	/* Create body widget */
	d.body = new QWidget (this);
	d.body->setGeometry(QRect(BOX_MARGIN, BOX_MARGIN, BOX_WIDTH - 2*BOX_MARGIN, BOX_HEIGHT - 3*BOX_MARGIN - but_height));

	/* Create body layout */
	d.layout = new QGridLayout (d.body);
	d.layout->setContentsMargins(0, 0, 0, 0);

	/* Create "locprf" config ui */
	d.locprf_label = new QLabel (d.body);
	d.locprf_label->setText ("Store preferences locally:");
	d.locprf_label->setToolTip (STR_LOCPRF_TOOLTIP);

	d.locprf_edit = new QCheckBox (d.body);
	d.locprf_edit->setToolTip (STR_LOCPRF_TOOLTIP);
	d.locprf_edit->setWhatsThis (STR_LOCPRF_WHATSTHIS);

	d.layout->addWidget (d.locprf_label,	row, 0, 1, 1);
	d.layout->addWidget (d.locprf_edit,	row, 1, 1, 1);
	row++;

	/* Add spacer */
	d.layout->setRowStretch (row, 1);
	row++;

	/* Create "snap" config ui */
	d.snp_label = new QLabel (d.body);
	d.snp_label->setText ("Warcab saves folder:");
	d.snp_label->setToolTip (STR_SNP_TOOLTIP);

	d.snp_edit = new QLineEdit (d.body);
	d.snp_label->setBuddy (d.snp_edit);
	d.snp_edit->setToolTip (STR_SNP_TOOLTIP);
	d.snp_edit->setWhatsThis (STR_SNP_WHATSTHIS1);

	d.snp_browse = new QPushButton (d.body);
	d.snp_browse->setText ("...");
	d.snp_browse->setMaximumSize(QSize(d.bbw, d.bbh));
	d.snp_browse->setToolTip (STR_SNP_TOOLTIP);
	d.snp_browse->setWhatsThis (STR_SNP_WHATSTHIS2);

	d.layout->addWidget (d.snp_label,	row, 0, 1, 1);
	d.layout->addWidget (d.snp_edit,	row, 1, 1, 1);
	d.layout->addWidget (d.snp_browse,	row, 2, 1, 1);
	row++;

	/* Create "compress" config ui */
	d.compress_label = new QLabel (d.body);
	d.compress_label->setText ("Dossier compression:");
	d.compress_label->setToolTip (STR_COMPRESS_TOOLTIP);

	d.compress_edit = new QCheckBox (d.body);
	d.compress_edit->setToolTip (STR_COMPRESS_TOOLTIP);
	d.compress_edit->setWhatsThis (STR_COMPRESS_WHATSTHIS);

	d.layout->addWidget (d.compress_label,	row, 0, 1, 1);
	d.layout->addWidget (d.compress_edit,	row, 1, 1, 1);
	row++;

	/* Create "autoload" config ui */
	d.autoload_label = new QLabel (d.body);
	d.autoload_label->setText ("Dossier autoload:");
	d.autoload_label->setToolTip (STR_AUTOLOAD_TOOLTIP);

	d.autoload_edit = new QCheckBox (d.body);
	d.autoload_edit->setToolTip (STR_AUTOLOAD_TOOLTIP);
	d.autoload_edit->setWhatsThis (STR_AUTOLOAD_WHATSTHIS);

	d.layout->addWidget (d.autoload_label,	row, 0, 1, 1);
	d.layout->addWidget (d.autoload_edit,	row, 1, 1, 1);
	row++;

	/* Add spacer */
	d.layout->setRowStretch (row, 1);
	row++;

	/* Create "default game type" config ui */
	d.dgt_label = new QLabel (d.body);
	d.dgt_label->setText (STR_DGT_NAME);
	d.dgt_label->setToolTip (STR_DGT_TOOLTIP);

	d.dgt_select = new QComboBox (d.body);
	d.dgt_select->setEditable (false);
	d.dgt_select->setToolTip (STR_DGT_TOOLTIP);
	d.dgt_select->setWhatsThis (STR_DGT_WHATSTHIS);

	for (int i=0; i<d.dlg_data->types.size(); i++) {
		d.dgt_select->addItem (d.dlg_data->types[i].name, (uint)d.dlg_data->types[i].type);
	}

	d.layout->addWidget (d.dgt_label,	row, 0, 1, 1);
	d.layout->addWidget (d.dgt_select,	row, 1, 1, 1);
	row++;

	/* Create game type specific config ui */
	d.games_gui = new QVector<CfgDlgGuiGame> (d.dlg_data->games.size());
	for (int i=0; i<d.dlg_data->games.size(); i++) {
		(*d.games_gui)[i].box = new QGroupBox (QString(STR_BOX_NAME).arg(d.dlg_data->games[i]->name), d.body);
		d.layout->addWidget ((*d.games_gui)[i].box,	row, 0, 1, 3);
		row++;

		(*d.games_gui)[i].layout = new QGridLayout((*d.games_gui)[i].box);

		(*d.games_gui)[i].oob_label = new QLabel ((*d.games_gui)[i].box);
		(*d.games_gui)[i].oob_label->setText (QString(STR_OOB_NAME));
		(*d.games_gui)[i].oob_label->setToolTip (QString(STR_OOB_TOOLTIP).arg(d.dlg_data->games[i]->name));

		(*d.games_gui)[i].oob_edit = new QLineEdit ((*d.games_gui)[i].box);
		(*d.games_gui)[i].oob_label->setBuddy ((*d.games_gui)[i].oob_edit);
		(*d.games_gui)[i].oob_edit->setToolTip (QString(STR_OOB_TOOLTIP).arg(d.dlg_data->games[i]->name));
		(*d.games_gui)[i].oob_edit->setWhatsThis (QString(STR_OOB_WHATSTHIS1).arg(d.dlg_data->games[i]->name));

		(*d.games_gui)[i].oob_browse = new QPushButton ((*d.games_gui)[i].box);
		(*d.games_gui)[i].oob_browse->setText ("...");
		(*d.games_gui)[i].oob_browse->setMaximumSize(QSize(d.bbw, d.bbh));
		(*d.games_gui)[i].oob_browse->setToolTip (QString(STR_OOB_TOOLTIP).arg(d.dlg_data->games[i]->name));
		(*d.games_gui)[i].oob_browse->setWhatsThis (QString(STR_OOB_WHATSTHIS2).arg(d.dlg_data->games[i]->name));
		(*d.games_gui)[i].oob_browse->setProperty ("index", i);

		(*d.games_gui)[i].layout->addWidget ((*d.games_gui)[i].oob_label,	0, 0, 1, 1);
		(*d.games_gui)[i].layout->addWidget ((*d.games_gui)[i].oob_edit,	0, 1, 1, 1);
		(*d.games_gui)[i].layout->addWidget ((*d.games_gui)[i].oob_browse,	0, 2, 1, 1);

		(*d.games_gui)[i].sve_label = new QLabel ((*d.games_gui)[i].box);
		(*d.games_gui)[i].sve_label->setText (QString(STR_SVE_NAME));
		(*d.games_gui)[i].sve_label->setToolTip (QString(STR_SVE_TOOLTIP).arg(d.dlg_data->games[i]->name));

		(*d.games_gui)[i].sve_edit = new QLineEdit ((*d.games_gui)[i].box);
		(*d.games_gui)[i].sve_label->setBuddy ((*d.games_gui)[i].sve_edit);
		(*d.games_gui)[i].sve_edit->setToolTip (QString(STR_SVE_TOOLTIP).arg(d.dlg_data->games[i]->name));
		(*d.games_gui)[i].sve_edit->setWhatsThis (QString(STR_SVE_WHATSTHIS1).arg(d.dlg_data->games[i]->name));

		(*d.games_gui)[i].sve_browse = new QPushButton ((*d.games_gui)[i].box);
		(*d.games_gui)[i].sve_browse->setText ("...");
		(*d.games_gui)[i].sve_browse->setMaximumSize(QSize(d.bbw, d.bbh));
		(*d.games_gui)[i].sve_browse->setToolTip (QString(STR_SVE_TOOLTIP).arg(d.dlg_data->games[i]->name));
		(*d.games_gui)[i].sve_browse->setWhatsThis (QString(STR_SVE_WHATSTHIS2).arg(d.dlg_data->games[i]->name));
		(*d.games_gui)[i].sve_browse->setProperty ("index", i);

		(*d.games_gui)[i].layout->addWidget ((*d.games_gui)[i].sve_label,	1, 0, 1, 1);
		(*d.games_gui)[i].layout->addWidget ((*d.games_gui)[i].sve_edit,	1, 1, 1, 1);
		(*d.games_gui)[i].layout->addWidget ((*d.games_gui)[i].sve_browse,	1, 2, 1, 1);
	}

	d.layout->setRowStretch (row, 1);
	row++;

	/* Fix dialog tab order */
	setTabOrder (d.locprf_edit, d.snp_edit);
	setTabOrder (d.snp_edit, d.snp_browse);
	setTabOrder (d.snp_browse, d.compress_edit);
	setTabOrder (d.compress_edit, d.autoload_edit);
	setTabOrder (d.autoload_edit, d.dgt_select);
	setTabOrder (d.dgt_select, (*d.games_gui)[0].oob_edit);
	for (int i=0; i<(*d.games_gui).size(); i++) {
		if (i>0) setTabOrder ((*d.games_gui)[i-1].sve_browse, (*d.games_gui)[i].oob_edit);
		setTabOrder ((*d.games_gui)[i].oob_edit, (*d.games_gui)[i].oob_browse);
		setTabOrder ((*d.games_gui)[i].oob_browse, (*d.games_gui)[i].sve_edit);
		setTabOrder ((*d.games_gui)[i].sve_edit, (*d.games_gui)[i].sve_browse);
	}
	setTabOrder ((*d.games_gui)[(*d.games_gui).size()-1].sve_browse, d.buttons);

	/* Finally connect signals and slots */
	connect (d.buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect (d.buttons, SIGNAL(rejected()), this, SLOT(reject()));
	connect (d.locprf_edit, SIGNAL(clicked(bool)), this, SLOT(locprf_edit_clicked(bool)));
	connect (d.snp_browse, SIGNAL(clicked(bool)), this, SLOT(snp_browse_clicked(bool)));
	for (int i=0; i<d.dlg_data->games.size(); i++) {
		connect ((*d.games_gui)[i].oob_browse, SIGNAL(clicked(bool)), this, SLOT(oob_browse_clicked(bool)));
		connect ((*d.games_gui)[i].sve_browse, SIGNAL(clicked(bool)), this, SLOT(sve_browse_clicked(bool)));
	}
}

CfgDlg::~CfgDlg (void)
{
	// QT deletes child widgets
	delete d.games_gui;
	delete d.dlgf;
	delete d.dlgfm;
}

void
CfgDlg::prepare(void)
{
	if (!d.dlg_data) return;

	d.locprf_edit->setCheckState (d.dlg_data->locprf ? Qt::Checked : Qt::Unchecked);
	d.snp_edit->setText (d.dlg_data->snp);
	d.compress_edit->setCheckState (d.dlg_data->compress ? Qt::Checked : Qt::Unchecked);
	d.autoload_edit->setCheckState (d.dlg_data->autoload ? Qt::Checked : Qt::Unchecked);
	for (int i=0; i<d.dlg_data->types.size(); i++) {
		if (d.dlg_data->types[i].type == d.dlg_data->def_game) {
			d.dgt_select->setCurrentIndex (i);
			break;
		}
	}
	for (int i=0; i<d.dlg_data->games.size(); i++) {
		(*d.games_gui)[i].oob_edit->setText (d.dlg_data->games[i]->oob);
		(*d.games_gui)[i].sve_edit->setText (d.dlg_data->games[i]->sve);
	}
}

void
CfgDlg::update (void)
{
	if (!d.dlg_data) return;

	d.dlg_data->locprf = (d.locprf_edit->checkState() == Qt::Checked) ? true : false;
	d.dlg_data->snp = d.snp_edit->text().replace('/', '\\');
	d.dlg_data->compress = (d.compress_edit->checkState() == Qt::Checked) ? true : false;
	d.dlg_data->autoload = (d.autoload_edit->checkState() == Qt::Checked) ? true : false;
	d.dlg_data->def_game = (SPWAW_GAME_TYPE)(d.dgt_select->itemData (d.dgt_select->currentIndex()).toUInt());
	for (int i=0; i<d.dlg_data->games.size(); i++) {
		d.dlg_data->games[i]->oob = (*d.games_gui)[i].oob_edit->text().replace('/', '\\');
		d.dlg_data->games[i]->sve = (*d.games_gui)[i].sve_edit->text().replace('/', '\\');
	}
}

int
CfgDlg::exec (void)
{
	int	rc;

	prepare();
	rc = QDialog::exec();
	update();

	return (rc);
}

void
CfgDlg::locprf_edit_clicked (bool checked)
{
	int	rc;

	/* Warn the user for a local->global change (configuration overwrite),
	 * except if this is the first time the configuration dialog is shown.
	 *
	 * Well, technically, this should be:
	 * except if this is the first time the configuration dialog is shown
	 * *AND* there where no other changes to the data...
	 */
	if (!d.dlg_data->isfirstrun && d.dlg_data->locprf && !checked) {
		rc = GUI_msgbox (MSGBOX_CONFIRM, "Overwrite global preferences?", STR_LOCPRF_OVERWRITE);
		if (rc !=  QDialog::Accepted) {
			d.locprf_edit->setCheckState (Qt::Checked);
		}
	}
}

void
CfgDlg::snp_browse_clicked (bool /*checked*/)
{
	QString	dir;

	dir = QFileDialog::getExistingDirectory (this,
		"Specify Warcab snapshot directory:",
		d.snp_edit->text(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);

	if (!dir.isNull()) { d.snp_edit->setText (dir); }
}

void
CfgDlg::oob_browse_clicked (bool /*checked*/)
{
	QObject *from = sender();
	if (!from) return;
	int	idx = from->property ("index").toInt();

	QString	dir;

	dir = QFileDialog::getExistingDirectory (this,
		QString(STR_OOB_BROWSE).arg(SPWAW_gametype2str(SPWAW_GAME_TYPE_SPWAW)),
		(*d.games_gui)[idx].oob_edit->text(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);

	if (!dir.isNull()) { (*d.games_gui)[idx].oob_edit->setText (dir); }
}

void
CfgDlg::sve_browse_clicked (bool /*checked*/)
{
	QObject *from = sender();
	if (!from) return;
	int	idx = from->property ("index").toInt();

	QString	start, dir;

	// If the OOB directory was already set, but the save directory not yet:
	// use the OOB directory as starting point for the save directoy browse.
	start = (*d.games_gui)[idx].sve_edit->text();
	if (start.isEmpty()) start = (*d.games_gui)[idx].oob_edit->text();

	dir = QFileDialog::getExistingDirectory (this,
		QString(STR_SVE_BROWSE).arg(SPWAW_gametype2str(SPWAW_GAME_TYPE_SPWAW)),
		start, QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);

	if (!dir.isNull()) { (*d.games_gui)[idx].sve_edit->setText (dir); }
}