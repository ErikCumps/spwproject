/** \file
 * The SPWaW war cabinet - configuration dialog.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "../common.h"
#include "res/res.h"
#include "cfg_dlg.h"

#define	STR_OOB_TOOLTIP		"Configure the folder containing the SPWaW OOB files."
#define	STR_OOB_WHATSTHIS1	"This should be the folder containing the SPWaW OOB files."
#define	STR_OOB_WHATSTHIS2	"Press this button to browse to the folder containg the SPWaW OOB files."

#define	STR_SVE_TOOLTIP		"Configure the SPWaW SAVE folder."
#define	STR_SVE_WHATSTHIS1	"This should be the SPWaW SAVE folder."
#define	STR_SVE_WHATSTHIS2	"Press this button to browse to the SPWaW SAVE folder."

#define	STR_SNP_TOOLTIP		"Configure the Warcab saves folder."
#define	STR_SNP_WHATSTHIS1	"This should be the folder where warcab writes saved dossiers."
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

CfgDlg::CfgDlg (void)
	: QDialog (0, Qt::Dialog)
{
	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Set dialog modality */
	setModal (true);

	/* Set dialog size */
	resize(400, 300);

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
	d.buttons->setGeometry(QRect(30, 240, 341, 32));
	d.buttons->setOrientation(Qt::Horizontal);
	d.buttons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

	/* Create body widget */
	d.body = new QWidget (this);
	d.body->setGeometry(QRect(10, 10, 381, 221));

	/* Create body layout */
	d.layout = new QGridLayout (d.body);
	d.layout->setContentsMargins(0, 0, 0, 0);

	/* Create "oob" config ui */
	d.oob_label = new QLabel (d.body);
	d.oob_label->setText ("SPWaW OOB files:");
	d.oob_label->setToolTip (STR_OOB_TOOLTIP);

	d.oob_edit = new QLineEdit (d.body);
	d.oob_label->setBuddy (d.oob_edit);
	d.oob_edit->setToolTip (STR_OOB_TOOLTIP);
	d.oob_edit->setWhatsThis (STR_OOB_WHATSTHIS1);

	d.oob_browse = new QPushButton (d.body);
	d.oob_browse->setText ("...");
	d.oob_browse->setMaximumSize(QSize(d.bbw, d.bbh));
	d.oob_browse->setToolTip (STR_OOB_TOOLTIP);
	d.oob_browse->setWhatsThis (STR_OOB_WHATSTHIS2);

	d.layout->addWidget (d.oob_label,	0, 0, 1, 1);
	d.layout->addWidget (d.oob_edit,	0, 1, 1, 1);
	d.layout->addWidget (d.oob_browse,	0, 2, 1, 1);

	/* Create "save" config ui */
	d.sve_label = new QLabel (d.body);
	d.sve_label->setText ("SPWaW SAVE folder:");
	d.sve_label->setToolTip (STR_SVE_TOOLTIP);

	d.sve_edit = new QLineEdit (d.body);
	d.sve_label->setBuddy (d.sve_edit);
	d.sve_edit->setToolTip (STR_SVE_TOOLTIP);
	d.sve_edit->setWhatsThis (STR_SVE_WHATSTHIS1);

	d.sve_browse = new QPushButton (d.body);
	d.sve_browse->setText ("...");
	d.sve_browse->setMaximumSize(QSize(d.bbw, d.bbh));
	d.sve_browse->setToolTip (STR_SVE_TOOLTIP);
	d.sve_browse->setWhatsThis (STR_SVE_WHATSTHIS2);

	d.layout->addWidget (d.sve_label,	1, 0, 1, 1);
	d.layout->addWidget (d.sve_edit,	1, 1, 1, 1);
	d.layout->addWidget (d.sve_browse,	1, 2, 1, 1);

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

	d.layout->addWidget (d.snp_label,	2, 0, 1, 1);
	d.layout->addWidget (d.snp_edit,	2, 1, 1, 1);
	d.layout->addWidget (d.snp_browse,	2, 2, 1, 1);

	/* Create "compress" config ui */
	d.compress_label = new QLabel (d.body);
	d.compress_label->setText ("Dossier compression:");
	d.compress_label->setToolTip (STR_COMPRESS_TOOLTIP);

	d.compress_edit = new QCheckBox (d.body);
	d.compress_edit->setToolTip (STR_COMPRESS_TOOLTIP);
	d.compress_edit->setWhatsThis (STR_COMPRESS_WHATSTHIS);

	d.layout->addWidget (d.compress_label,	3, 0, 1, 1);
	d.layout->addWidget (d.compress_edit,	3, 1, 1, 1);

	/* Create "autoload" config ui */
	d.autoload_label = new QLabel (d.body);
	d.autoload_label->setText ("Dossier autoload:");
	d.autoload_label->setToolTip (STR_AUTOLOAD_TOOLTIP);

	d.autoload_edit = new QCheckBox (d.body);
	d.autoload_edit->setToolTip (STR_AUTOLOAD_TOOLTIP);
	d.autoload_edit->setWhatsThis (STR_AUTOLOAD_WHATSTHIS);

	d.layout->addWidget (d.autoload_label,	4, 0, 1, 1);
	d.layout->addWidget (d.autoload_edit,	4, 1, 1, 1);

	/* Fix dialog tab order */
	setTabOrder (d.oob_edit, d.oob_browse);	setTabOrder (d.oob_browse, d.sve_edit);
	setTabOrder (d.sve_edit, d.sve_browse);	setTabOrder (d.sve_browse, d.snp_edit);
	setTabOrder (d.snp_edit, d.snp_browse);	setTabOrder (d.snp_browse, d.compress_edit);
	setTabOrder (d.compress_edit, d.autoload_edit);
	setTabOrder (d.autoload_edit, d.buttons);

	/* Finally connect signals and slots */
	connect (d.buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect (d.buttons, SIGNAL(rejected()), this, SLOT(reject()));
	connect (d.oob_browse, SIGNAL(clicked(bool)), this, SLOT(oob_browse_clicked(bool)));
	connect (d.sve_browse, SIGNAL(clicked(bool)), this, SLOT(sve_browse_clicked(bool)));
	connect (d.snp_browse, SIGNAL(clicked(bool)), this, SLOT(snp_browse_clicked(bool)));
}

CfgDlg::~CfgDlg (void)
{
	// QT deletes child widgets
	delete d.dlgf;
	delete d.dlgfm;
}

void
CfgDlg::data_set(CfgDlgData *data)
{
	Qt::CheckState	state;

	if (!data) return;

	if (data->oob) d.oob_edit->setText (data->oob);
	if (data->sve) d.sve_edit->setText (data->sve);
	if (data->snp) d.snp_edit->setText (data->snp);

	state = data->compress ? Qt::Checked : Qt::Unchecked;
	d.compress_edit->setCheckState (state);

	state = data->autoload ? Qt::Checked : Qt::Unchecked;
	d.autoload_edit->setCheckState (state);
}

void
CfgDlg::data_get (CfgDlgData *data)
{
	if (!data) return;
	memset (data, 0, sizeof (*data));

	SL_SAFE_STRDUP (data->oob, qPrintable (d.oob_edit->text().replace('/', '\\')));
	SL_SAFE_STRDUP (data->sve, qPrintable (d.sve_edit->text().replace('/', '\\')));
	SL_SAFE_STRDUP (data->snp, qPrintable (d.snp_edit->text().replace('/', '\\')));
	data->compress = (d.compress_edit->checkState() == Qt::Checked) ? true : false;
	data->autoload = (d.autoload_edit->checkState() == Qt::Checked) ? true : false;
}

void
CfgDlg::data_free (CfgDlgData *data)
{
	if (!data) return;

	if (data->oob) { free (data->oob); }
	if (data->sve) { free (data->sve); }
	if (data->snp) { free (data->snp); }
	memset (data, 0, sizeof (*data));
}

void
CfgDlg::oob_browse_clicked (bool /*checked*/)
{
	QString	dir;

	dir = QFileDialog::getExistingDirectory (this,
		"Specify SPWaW OOB directory:",
		d.oob_edit->text(), QFileDialog::ShowDirsOnly);

	if (!dir.isNull()) { d.oob_edit->setText (dir); }
}

void
CfgDlg::sve_browse_clicked (bool /*checked*/)
{
	QString	start, dir;

	// If the OOB directory was already set, but the save directory not yet:
	// use the OOB directory as starting point for the save directoy browse.
	start = d.sve_edit->text();
	if (start.isEmpty()) start = d.oob_edit->text();

	dir = QFileDialog::getExistingDirectory (this,
		"Specify SPWaW save directory:",
		start, QFileDialog::ShowDirsOnly);

	if (!dir.isNull()) { d.sve_edit->setText (dir); }
}

void
CfgDlg::snp_browse_clicked (bool /*checked*/)
{
	QString	dir;

	dir = QFileDialog::getExistingDirectory (this,
		"Specify Warcab snapshot directory:",
		d.snp_edit->text(), QFileDialog::ShowDirsOnly);

	if (!dir.isNull()) { d.snp_edit->setText (dir); }
}
