/** \file
 * The SPWaW war cabinet - configuration dialog.
 *
 * Copyright (C) 2005-2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "../common.h"
#include "res/res.h"
#include "cfg_dlg.h"

#define	BOX_WIDTH	600
#define	BOX_HEIGHT	630
#define	BOX_MARGIN	 10

#define	STR_LOCPRF_NAME			"Store preferences locally:"
#define	STR_LOCPRF_TOOLTIP							\
	"Select this to store the preferences locally in a \"warcab.ini\"\n"	\
	"file, allowing installations to have their own preferences.\n"		\
	"\n"									\
	"Disable this to store the preferences globally in the registry,\n"	\
	"so that they can be shared between different installations."
#define	STR_LOCPRF_WHATSTHIS		STR_LOCPRF_TOOLTIP
#define	STR_LOCPRF_OVERWRITE							\
	"Unchecking the \"Store preferences locally\" checkbox will\n"		\
	"cause any existing global preferences to be overwritten\n"		\
	"with the local preferences!\n"						\
	"\n"									\
	"If this is not desired, please select <Cancel> to revert\n"		\
	"to storing the preferences locally...\n"

#define	STR_SNP_NAME			"Dossier saves folder:"
#define	STR_SNP_TOOLTIP								\
	"Specify the folder where dossiers and stratmap images should be saved."
#define	STR_SNP_WHATSTHIS1		STR_SNP_TOOLTIP
#define	STR_SNP_WHATSTHIS2		STR_SNP_TOOLTIP
#define	STR_SNP_BROWSE			"Specify the dossier saves folder:"

#define	STR_COMPRESS_NAME		"Dossier compression:"
#define	STR_COMPRESS_TOOLTIP							\
	"Select this to enable compression when saving dossier files.\n"	\
	"\n"									\
	"Disable this to allow easier file inspection of *.warcab files."
#define	STR_COMPRESS_WHATSTHIS		STR_COMPRESS_TOOLTIP

#define	STR_AUTOLOAD_NAME		"Dossier autoload:"
#define	STR_AUTOLOAD_TOOLTIP							\
	"Select this to load the last opened dossier at startup.\n"		\
	"\n"									\
	"Disable this to start without reloading the last dossier."
#define	STR_AUTOLOAD_WHATSTHIS		STR_AUTOLOAD_TOOLTIP

#define	STR_FULL_HISTORY_NAME		"Full campaign history:"
#define	STR_FULL_HISTORY_TOOLTIP						\
	"Select this to enable full campaign history, which also shows\n"	\
	"data for decommissioned units on the following Dossier tabs:\n"	\
	"Overview, Status, Progress, Kills, Losses, Roster and History.\n"	\
	"\n"									\
	"Disable this to hide this data for decommisioned units."
#define	STR_FULL_HISTORY_WHATSTHIS	STR_FULL_HISTORY_TOOLTIP

#define	STR_IMODE_NAME			"Default intel mode:"
#define	STR_IMODE_TOOLTIP		"Choose the default intel mode."
#define	STR_IMODE_WHATSTHIS		STR_IMODE_TOOLTIP

#define	STR_HCFTYPE_NAME		"Default strategic map colorfield:"
#define	STR_HCFTYPE_TOOLTIP		"Choose the default strategic map height colorfield type."
#define	STR_HCFTYPE_WHATSTHIS		STR_HCFTYPE_TOOLTIP

#define	STR_GERMAN_CROSS_NAME		"German Cross flag:"
#define	STR_GERMAN_CROSS_TOOLTIP						\
	"Select this if you do not want War Cabinet to display the\n"		\
	"historic German Flag from WWII for personal reasons or to\n"		\
	"satisfy the requirements of German Law.\n"				\
	"\n"									\
	"Disable this to display the historic German Flag from WWII."
#define	STR_GERMAN_CROSS_WHATSTHIS	STR_GERMAN_CROSS_TOOLTIP

#define	STR_DG_NAME			"Default game config:"
#define	STR_DG_TOOLTIP			"Choose the default game config for new dossiers."
#define	STR_DG_WHATSTHIS		STR_DG_TOOLTIP

#define	STR_GAMECFG_NAME		"Game config #%1"

#define	STR_GAMENAME_NAME		"Name:"
#define	STR_GAMENAME_TOOLTIP		"Specify a (preferaby unique) name."
#define	STR_GAMENAME_WHATSTHIS		STR_GAMENAME_TOOLTIP

#define	STR_GAMETYPE_NAME		"Type:"
#define	STR_GAMETYPE_TOOLTIP		"Specify the game type."
#define	STR_GAMETYPE_WHATSTHIS		STR_GAMETYPE_TOOLTIP

#define	STR_GAMEPATH_NAME		"Installation:"
#define	STR_GAMEPATH_TOOLTIP							\
	"Specify the installation folder of a supported game.\n"		\
	"\n"									\
	"Some examples:\n"							\
	"    \"E:\\games\\war\\SPWaW\\\"\n"					\
	"    \"E:\\games\\war\\winSPWW2\\\""
#define	STR_GAMEPATH_WHATSTHIS1		STR_GAMEPATH_TOOLTIP
#define	STR_GAMEPATH_WHATSTHIS2		STR_GAMEPATH_TOOLTIP
#define	STR_GAMEPATH_BROWSE		"Specify the installation folder of a supported game:"

#define	STR_GAME_HLT_MISSING_NAME						\
	"\n\n"									\
	"This field is highlighted in red, because it is\n"			\
	"still empty, but some other fields for this\n"				\
	"game config are already filled in."

#define	STR_GAME_HLT_MISSING_TYPE						\
	"\n\n"									\
	"This combo box is highlighted in red, because\n"			\
	"no choice is made, but some fields for this\n"				\
	"game config are already filled in."

#define	STR_GAME_HLT_MISSING_PATH						\
	"\n\n"									\
	"This field is highlighted in red, because it is still\n"		\
	"empty, but some other fields for this game config\n"			\
	"are already filled in."

#define	STR_GAME_HLT_NOTFOUND							\
	"\n\n"									\
	"This field is highlighted in red, because this folder\n"		\
	"can not be found."

#define	STR_GAME_HLT_INVALID_PATH						\
	"\n\n"									\
	"This field is highlighted in red, because this folder\n"		\
	"does not contain a game of the specified game type."

#define	STR_GAME_HLT_INVALID_TYPE						\
	"\n\n"									\
	"This combo box is highlighted in red, because the\n"			\
	"specified installation folder does not contain a game\n"		\
	"of this game type."

CfgDlgDataGame::CfgDlgDataGame (void)
{
	active = false;
	type = SPWAW_GAME_TYPE_UNKNOWN;
}

CfgDlgGuiGame::CfgDlgGuiGame (void)
{
	memset (&d, 0, sizeof (d));
	d.status = EMPTY;
}

static inline void
set_color (QWidget *w, QColor c, QPalette::ColorRole r)
{
	if (!w) return;

	QPalette p = w->palette();
	p.setColor(r, c);
	w->setPalette(p);
}

void
CfgDlgGuiGame::unhighlight (void)
{
	QColor	color;

	color = d.name_def_status;
	set_color (d.name_edit, color, QPalette::Base);
	d.name_edit->setToolTip (QString(STR_GAMENAME_TOOLTIP));
	d.name_edit->setWhatsThis (QString(STR_GAMENAME_WHATSTHIS));

	color = d.type_def_status;
	set_color (d.type_select, color, QPalette::Text);
	d.type_select->setToolTip (QString(STR_GAMETYPE_TOOLTIP));
	d.type_select->setWhatsThis (QString(STR_GAMETYPE_WHATSTHIS));

	color = d.path_def_status;
	set_color (d.path_edit, color, QPalette::Base);
	d.path_edit->setToolTip (QString(STR_GAMEPATH_TOOLTIP));
	d.path_edit->setWhatsThis (QString(STR_GAMEPATH_WHATSTHIS1));
}

void
CfgDlgGuiGame::highlight (bool needs_name, bool needs_type, bool needs_path, bool path_not_found, bool invalid_path)
{
	QColor	color;

	color = needs_name ? d.name_hlt_status : d.name_def_status;
	set_color (d.name_edit, color, QPalette::Base);
	if (needs_name) {
		d.name_edit->setToolTip (QString(STR_GAMENAME_TOOLTIP STR_GAME_HLT_MISSING_NAME));
		d.name_edit->setWhatsThis (QString(STR_GAMENAME_WHATSTHIS STR_GAME_HLT_MISSING_NAME));
	} else {
		d.name_edit->setToolTip (QString(STR_GAMENAME_TOOLTIP));
		d.name_edit->setWhatsThis (QString(STR_GAMENAME_WHATSTHIS));
	}

	color = (needs_type || invalid_path) ? d.type_hlt_status : d.type_def_status;
	set_color (d.type_select, color, QPalette::Text);
	if (needs_type) {
		d.type_select->setToolTip (QString(STR_GAMETYPE_TOOLTIP STR_GAME_HLT_MISSING_TYPE));
		d.type_select->setWhatsThis (QString(STR_GAMETYPE_WHATSTHIS STR_GAME_HLT_MISSING_TYPE));
	} else if (invalid_path) {
		d.type_select->setToolTip (QString(STR_GAMETYPE_TOOLTIP STR_GAME_HLT_INVALID_TYPE));
		d.type_select->setWhatsThis (QString(STR_GAMETYPE_WHATSTHIS STR_GAME_HLT_INVALID_TYPE));
	} else {
		d.type_select->setToolTip (QString(STR_GAMETYPE_TOOLTIP));
		d.type_select->setWhatsThis (QString(STR_GAMETYPE_WHATSTHIS));
	}

	color = (needs_path || path_not_found || invalid_path) ? d.path_hlt_status : d.path_def_status;
	set_color (d.path_edit, color, QPalette::Base);
	if (needs_path) {
		d.path_edit->setToolTip (QString(STR_GAMEPATH_TOOLTIP STR_GAME_HLT_MISSING_PATH));
		d.path_edit->setWhatsThis (QString(STR_GAMEPATH_WHATSTHIS1 STR_GAME_HLT_MISSING_PATH));
	} else if (path_not_found) {
		d.path_edit->setToolTip (QString(STR_GAMEPATH_TOOLTIP STR_GAME_HLT_NOTFOUND));
		d.path_edit->setWhatsThis (QString(STR_GAMEPATH_WHATSTHIS1 STR_GAME_HLT_NOTFOUND));
	} else if (invalid_path) {
		d.path_edit->setToolTip (QString(STR_GAMEPATH_TOOLTIP STR_GAME_HLT_INVALID_PATH));
		d.path_edit->setWhatsThis (QString(STR_GAMEPATH_WHATSTHIS1 STR_GAME_HLT_INVALID_PATH));
	} else {
		d.path_edit->setToolTip (QString(STR_GAMEPATH_TOOLTIP));
		d.path_edit->setWhatsThis (QString(STR_GAMEPATH_WHATSTHIS1));
	}
}

void
CfgDlgGuiGame::update_status (void)
{
	QString		name;
	SPWAW_GAME_TYPE	type;
	QString		path;
	bool		complete, path_exists, valid;

	name = d.name_edit->text().replace('/', '\\');
	type =(SPWAW_GAME_TYPE)d.type_select->currentIndex();
	path = d.path_edit->text().replace('/', '\\');

	if (name.isEmpty() && (type == SPWAW_GAME_TYPE_UNKNOWN) && path.isEmpty()) {
		d.status = EMPTY;
		unhighlight ();
		return;
	}

	complete = (!name.isEmpty() && (type != SPWAW_GAME_TYPE_UNKNOWN) && !path.isEmpty());
	valid = CFG_valid_gamepath ((char *)qPrintable(path), type, path_exists);

	if (!complete) {
		d.status = PARTIAL;
		highlight (name.isEmpty(), type == SPWAW_GAME_TYPE_UNKNOWN, path.isEmpty(), !path_exists, !valid);
	} else if (!valid) {
		d.status = COMPLETE;
		highlight (false, false, false, !path_exists, true);
	} else {
		d.status = CORRECT;
		highlight (false, false, false, false, false);
	}
}

CfgDlgData::CfgDlgData (void)
{
	isfirstrun	= false;
	locprf		= DEFAULT_LOCALCFG;
	snp.clear();
	compress	= DEFAULT_COMPRESSION;
	autoload	= DEFAULT_AUTOLOAD;
	fhistory	= DEFAULT_FULL_HISTORY;
	imode		= DEFAULT_INTEL_MODE;
	hcftype		= DEFAULT_HCFTYPE;
	gecross		= DEFAULT_GERMAN_CROSS;
	def_game	= DEFAULT_GAME;
}

CfgDlg::CfgDlg (CfgDlgData *data)
	: QDialog (0, Qt::Dialog)
{
	int	but_height = 0;
	int	row = 0;
	QWidget	*tcw = NULL;

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
	create_locprf (row, tcw);

	/* Add spacer */
	d.layout->setRowStretch (row, 1);
	row++;

	/* Create and add separator1 */
	d.separator1 = new QFrame (this);
	d.separator1->setFrameStyle (QFrame::HLine);
	d.layout->addWidget (d.separator1,	row, 0, 1, 3);
	row++;

	/* Create "snap" config ui */
	create_snp (row, tcw);

	/* Create "compress" config ui */
	create_compress (row, tcw);

	/* Create "autoload" config ui */
	create_autoload (row, tcw);

	/* Create "fhistory" config ui */
	create_fhistory (row, tcw);

	/* Create "imode" config ui */
	create_imode (row, tcw);

	/* Create "hcftype" config ui */
	create_hcftype (row, tcw);

	/* Create "gecross" config ui */
	create_gecross (row, tcw);

	/* Add spacer */
	d.layout->setRowStretch (row, 1);
	row++;

	/* Create and add separator2 */
	d.separator2 = new QFrame (this);
	d.separator2->setFrameStyle (QFrame::HLine);
	d.layout->addWidget (d.separator2,	row, 0, 1, 3);
	row++;

	/* Create "default game" config ui */
	create_defg (row, tcw);

	/* Create "game" config ui */
	for (int i=0; i<GAMECFG_CNT; i++) {
		create_gui_game (row, i, tcw);
	}

	/* Add spacer */
	d.layout->setRowStretch (row, 2);
	row++;

	/* Finish dialog tab order */
	setTabOrder (tcw, d.buttons);

	/* Finish connecting signals and slots */
	connect (d.buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect (d.buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

CfgDlg::~CfgDlg (void)
{
	// QT deletes child widgets
	delete d.dlgf;
	delete d.dlgfm;
}

void
CfgDlg::create_locprf (int &row, QWidget* &tcw)
{
	d.locprf_label = new QLabel (d.body);
	d.locprf_label->setText (STR_LOCPRF_NAME);
	d.locprf_label->setToolTip (STR_LOCPRF_TOOLTIP);

	d.locprf_edit = new QCheckBox (d.body);
	d.locprf_edit->setToolTip (STR_LOCPRF_TOOLTIP);
	d.locprf_edit->setWhatsThis (STR_LOCPRF_WHATSTHIS);

	d.layout->addWidget (d.locprf_label,	row, 0, 1, 1);
	d.layout->addWidget (d.locprf_edit,	row, 1, 1, 1);
	row++;

	if (tcw) setTabOrder (tcw, d.locprf_edit);
	tcw = d.locprf_edit;

	connect (d.locprf_edit, SIGNAL(clicked(bool)), this, SLOT(locprf_edit_clicked(bool)));
}

void
CfgDlg::create_snp (int &row, QWidget* &tcw)
{
	d.snp_label = new QLabel (d.body);
	d.snp_label->setText (STR_SNP_NAME);
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

	if (tcw) setTabOrder (tcw, d.snp_edit);
	setTabOrder (d.snp_edit, d.snp_browse);
	tcw = d.snp_browse;

	connect (d.snp_browse, SIGNAL(clicked(bool)), this, SLOT(snp_browse_clicked(bool)));
}

void
CfgDlg::create_compress (int &row, QWidget* &tcw)
{
	d.compress_label = new QLabel (d.body);
	d.compress_label->setText (STR_COMPRESS_NAME);
	d.compress_label->setToolTip (STR_COMPRESS_TOOLTIP);

	d.compress_edit = new QCheckBox (d.body);
	d.compress_edit->setToolTip (STR_COMPRESS_TOOLTIP);
	d.compress_edit->setWhatsThis (STR_COMPRESS_WHATSTHIS);

	d.layout->addWidget (d.compress_label,	row, 0, 1, 1);
	d.layout->addWidget (d.compress_edit,	row, 1, 1, 1);
	row++;

	if (tcw) setTabOrder (tcw, d.compress_edit);
	tcw = d.compress_edit;
}

void
CfgDlg::create_autoload (int &row, QWidget* &tcw)
{
	d.autoload_label = new QLabel (d.body);
	d.autoload_label->setText (STR_AUTOLOAD_NAME);
	d.autoload_label->setToolTip (STR_AUTOLOAD_TOOLTIP);

	d.autoload_edit = new QCheckBox (d.body);
	d.autoload_edit->setToolTip (STR_AUTOLOAD_TOOLTIP);
	d.autoload_edit->setWhatsThis (STR_AUTOLOAD_WHATSTHIS);

	d.layout->addWidget (d.autoload_label,	row, 0, 1, 1);
	d.layout->addWidget (d.autoload_edit,	row, 1, 1, 1);
	row++;

	if (tcw) setTabOrder (tcw, d.autoload_edit);
	tcw = d.autoload_edit;
}

void
CfgDlg::create_fhistory (int &row, QWidget* &tcw)
{
	d.fhistory_label = new QLabel (d.body);
	d.fhistory_label->setText (STR_FULL_HISTORY_NAME);
	d.fhistory_label->setToolTip (STR_FULL_HISTORY_TOOLTIP);

	d.fhistory_edit = new QCheckBox (d.body);
	d.fhistory_edit->setToolTip (STR_FULL_HISTORY_TOOLTIP);
	d.fhistory_edit->setWhatsThis (STR_FULL_HISTORY_WHATSTHIS);

	d.layout->addWidget (d.fhistory_label,	row, 0, 1, 1);
	d.layout->addWidget (d.fhistory_edit,	row, 1, 1, 1);
	row++;

	if (tcw) setTabOrder (tcw, d.fhistory_edit);
	tcw = d.fhistory_edit;
}

void
CfgDlg::create_imode (int &row, QWidget* &tcw)
{
	d.imode_label = new QLabel (d.body);
	d.imode_label->setText (STR_IMODE_NAME);
	d.imode_label->setToolTip (STR_IMODE_TOOLTIP);

	d.imode_edit = new QComboBox (d.body);
	for (int i=0; i<INTEL_MODE_CNT; i++) {
		d.imode_edit->addItem (intelmode2str((INTEL_MODE)i));
	}
	d.imode_edit->setEditable (false);
	d.imode_edit->setToolTip (STR_IMODE_TOOLTIP);
	d.imode_edit->setWhatsThis (STR_IMODE_WHATSTHIS);

	d.layout->addWidget (d.imode_label,	row, 0, 1, 1);
	d.layout->addWidget (d.imode_edit,	row, 1, 1, 1);
	row++;

	if (tcw) setTabOrder (tcw, d.imode_edit);
	tcw = d.imode_edit;
}

void
CfgDlg::create_hcftype (int &row, QWidget* &tcw)
{
	d.hcftype_label = new QLabel (d.body);
	d.hcftype_label->setText (STR_HCFTYPE_NAME);
	d.hcftype_label->setToolTip (STR_HCFTYPE_TOOLTIP);

	d.hcftype_edit = new QComboBox (d.body);
	for (int i=0; i<SMAP_HPMC_TYPE_CNT; i++) {
		d.hcftype_edit->addItem (SMAP_hpmctype2str((SMAP_HPMC_TYPE)i));
	}
	d.hcftype_edit->setEditable (false);
	d.hcftype_edit->setToolTip (STR_HCFTYPE_TOOLTIP);
	d.hcftype_edit->setWhatsThis (STR_HCFTYPE_WHATSTHIS);

	d.layout->addWidget (d.hcftype_label,	row, 0, 1, 1);
	d.layout->addWidget (d.hcftype_edit,	row, 1, 1, 1);
	row++;

	if (tcw) setTabOrder (tcw, d.hcftype_edit);
	tcw = d.hcftype_edit;
}

void
CfgDlg::create_gecross (int &row, QWidget* &tcw)
{
	d.gecross_label = new QLabel (d.body);
	d.gecross_label->setText (STR_GERMAN_CROSS_NAME);
	d.gecross_label->setToolTip (STR_GERMAN_CROSS_TOOLTIP);

	d.gecross_edit = new QCheckBox (d.body);
	d.gecross_edit->setToolTip (STR_GERMAN_CROSS_TOOLTIP);
	d.gecross_edit->setWhatsThis (STR_GERMAN_CROSS_WHATSTHIS);

	d.layout->addWidget (d.gecross_label,	row, 0, 1, 1);
	d.layout->addWidget (d.gecross_edit,	row, 1, 1, 1);
	row++;

	if (tcw) setTabOrder (tcw, d.gecross_edit);
	tcw = d.gecross_edit;
}

void
CfgDlg::create_defg (int &row, QWidget* &tcw)
{
	d.defg_label = new QLabel (d.body);
	d.defg_label->setText (STR_DG_NAME);
	d.defg_label->setToolTip (STR_DG_TOOLTIP);

	d.defg_select = new QComboBox (d.body);
	d.defg_select->setEditable (false);
	d.defg_select->setToolTip (STR_DG_TOOLTIP);
	d.defg_select->setWhatsThis (STR_DG_WHATSTHIS);

	d.layout->addWidget (d.defg_label,	row, 0, 1, 1);
	d.layout->addWidget (d.defg_select,	row, 1, 1, 1);
	row++;

	if (tcw) setTabOrder (tcw, d.defg_select);
	tcw = d.defg_select;
}


void
CfgDlg::create_gui_game (int &row, int idx, QWidget* &tcw)
{
	CfgDlgGuiGame *gg = &(o.gui_games[idx]);

	gg->d.box = new QGroupBox (d.body);
	gg->d.box->setTitle (QString(STR_GAMECFG_NAME).arg(idx+1));
	d.layout->addWidget (gg->d.box, row, 0, 1, 3);
	row++;

	gg->d.name_label = new QLabel (gg->d.box);
	gg->d.name_label->setText (QString(STR_GAMENAME_NAME));
	gg->d.name_label->setToolTip (QString(STR_GAMENAME_TOOLTIP));

	gg->d.name_edit = new QLineEdit (gg->d.box);
	gg->d.name_label->setBuddy (gg->d.name_edit);
	gg->d.name_edit->setToolTip (QString(STR_GAMENAME_TOOLTIP));
	gg->d.name_edit->setWhatsThis (QString(STR_GAMENAME_WHATSTHIS));
	gg->d.name_edit->setProperty ("index", idx);
	gg->d.name_edit->setAutoFillBackground (true);

	gg->d.name_def_status = gg->d.name_edit->palette().color(QPalette::Base);
	gg->d.name_hlt_status = *RES_color(RID_RGB_RED);

	gg->d.type_label = new QLabel (gg->d.box);
	gg->d.type_label->setText (STR_GAMETYPE_NAME);
	gg->d.type_label->setToolTip (STR_GAMETYPE_TOOLTIP);

	gg->d.type_select = new QComboBox (gg->d.box);
	gg->d.type_select->setEditable (false);
	gg->d.type_select->setToolTip (STR_GAMETYPE_TOOLTIP);
	gg->d.type_select->setWhatsThis (STR_GAMETYPE_WHATSTHIS);
	for (int t=0; t<SPWAW_GAME_TYPE_CNT; t++) {
		if (t == SPWAW_GAME_TYPE_UNKNOWN) {
			gg->d.type_select->addItem ("-- please choose --");
		} else {
			gg->d.type_select->addItem (QString(SPWAW_gametype2str((SPWAW_GAME_TYPE)t)));
		}
	}
	gg->d.type_select->setProperty ("index", idx);

	gg->d.type_def_status = gg->d.type_select->palette().color(QPalette::Text);
	gg->d.type_hlt_status = *RES_color(RID_RGB_DRKRED);


	gg->d.path_label = new QLabel (gg->d.box);
	gg->d.path_label->setText (QString(STR_GAMEPATH_NAME));
	gg->d.path_label->setToolTip (QString(STR_GAMEPATH_TOOLTIP));

	gg->d.path_edit = new QLineEdit (gg->d.box);
	gg->d.path_label->setBuddy (gg->d.path_edit);
	gg->d.path_edit->setToolTip (QString(STR_GAMEPATH_TOOLTIP));
	gg->d.path_edit->setWhatsThis (QString(STR_GAMEPATH_WHATSTHIS1));
	gg->d.path_edit->setProperty ("index", idx);
	gg->d.path_edit->setAutoFillBackground (true);

	gg->d.path_def_status = gg->d.path_edit->palette().color(QPalette::Base);
	gg->d.path_hlt_status = *RES_color(RID_RGB_RED);

	gg->d.path_browse = new QPushButton (gg->d.box);
	gg->d.path_browse->setText ("...");
	gg->d.path_browse->setMaximumSize(QSize(d.bbw, d.bbh));
	gg->d.path_browse->setToolTip (QString(STR_GAMEPATH_TOOLTIP));
	gg->d.path_browse->setWhatsThis (QString(STR_GAMEPATH_WHATSTHIS2));
	gg->d.path_browse->setProperty ("index", idx);

	gg->d.layout = new QGridLayout(gg->d.box);

	gg->d.layout->addWidget (gg->d.name_label,	0, 0, 1, 1);
	gg->d.layout->addWidget (gg->d.name_edit,	0, 1, 1, 1);
	gg->d.layout->addWidget (gg->d.type_label,	0, 2, 1, 1);
	gg->d.layout->addWidget (gg->d.type_select,	0, 3, 1, 3);

	gg->d.layout->addWidget (gg->d.path_label,	1, 0, 1, 1);
	gg->d.layout->addWidget (gg->d.path_edit,	1, 1, 1, 4);
	gg->d.layout->addWidget (gg->d.path_browse,	1, 5, 1, 1);

	if (tcw) setTabOrder (tcw, gg->d.name_edit);
	setTabOrder (gg->d.name_edit, gg->d.type_select);
	setTabOrder (gg->d.type_select, gg->d.path_edit);
	setTabOrder (gg->d.path_edit, gg->d.path_browse);
	tcw = gg->d.path_browse;

	connect (gg->d.name_edit, SIGNAL(textEdited(const QString &)), this, SLOT(game_name_edited(const QString &)));
	connect (gg->d.type_select, SIGNAL(currentIndexChanged (int)), this, SLOT(game_type_selected(int)));
	connect (gg->d.path_edit, SIGNAL(textEdited(const QString &)), this, SLOT(game_path_edited(const QString &)));
	connect (gg->d.path_browse, SIGNAL(clicked(bool)), this, SLOT(game_path_browse_clicked(bool)));
}

int
CfgDlg::current_defg (void)
{
	return (d.defg_select->itemData (d.defg_select->currentIndex()).toInt());
}

void
CfgDlg::select_defg (int idx)
{
	for (int i=0; i<GAMECFG_CNT; i++) {
		if (d.defg_select->itemData(i).toInt() == idx) {
			d.defg_select->setCurrentIndex (i);
			break;
		}
	}
}

void
CfgDlg::update_defg_gui (void)
{
	int	idx;
	QString	name;

	idx = current_defg();

	d.defg_select->clear();
	d.defg_select->addItem ("none", -1);
	for (int i=0; i<GAMECFG_CNT; i++) {
		name = o.gui_games[i].d.name_edit->text();
		if (name.isEmpty()) continue;
		d.defg_select->addItem (name, i);
	}
	select_defg (idx);
}

void
CfgDlg::load_data_game (CfgDlgDataGame &data, CfgDlgGuiGame &game)
{
	game.d.name_edit->setText (data.name);
	game.d.type_select->setCurrentIndex (data.type);
	game.d.path_edit->setText (data.path);

	game.update_status();
}

void
CfgDlg::load_data (void)
{
	if (!d.dlg_data) return;

	if (d.dlg_data->isfirstrun) {
		// Dialog should not be cancellable on first run
		d.buttons->setStandardButtons(QDialogButtonBox::Ok);
	}

	d.locprf_edit->setCheckState (d.dlg_data->locprf ? Qt::Checked : Qt::Unchecked);
	d.snp_edit->setText (d.dlg_data->snp);
	d.compress_edit->setCheckState (d.dlg_data->compress ? Qt::Checked : Qt::Unchecked);
	d.autoload_edit->setCheckState (d.dlg_data->autoload ? Qt::Checked : Qt::Unchecked);
	d.fhistory_edit->setCheckState (d.dlg_data->fhistory ? Qt::Checked : Qt::Unchecked);
	d.imode_edit->setCurrentIndex (d.dlg_data->imode);
	d.hcftype_edit->setCurrentIndex (d.dlg_data->hcftype);
	d.gecross_edit->setCheckState (d.dlg_data->gecross ? Qt::Checked : Qt::Unchecked);

	for (int i=0; i<GAMECFG_CNT; i++) {
		load_data_game (d.dlg_data->games[i], o.gui_games[i]);
	}
	update_defg_gui ();
	select_defg (d.dlg_data->def_game);
}

void
CfgDlg::save_data_game (CfgDlgGuiGame &game, CfgDlgDataGame &data)
{
	data.active = (game.d.status == CfgDlgGuiGame::CORRECT);
	data.name = game.d.name_edit->text().replace('/', '\\');
	data.type = (SPWAW_GAME_TYPE)game.d.type_select->currentIndex();
	data.path = game.d.path_edit->text().replace('/', '\\');
}

void
CfgDlg::save_data (void)
{
	if (!d.dlg_data) return;

	d.dlg_data->locprf = (d.locprf_edit->checkState() == Qt::Checked) ? true : false;
	d.dlg_data->snp = d.snp_edit->text().replace('/', '\\');
	d.dlg_data->compress = (d.compress_edit->checkState() == Qt::Checked) ? true : false;
	d.dlg_data->autoload = (d.autoload_edit->checkState() == Qt::Checked) ? true : false;
	d.dlg_data->fhistory = (d.fhistory_edit->checkState() == Qt::Checked) ? true : false;
	d.dlg_data->imode = d.imode_edit->currentIndex();
	d.dlg_data->hcftype = d.hcftype_edit->currentIndex();
	d.dlg_data->gecross = (d.gecross_edit->checkState() == Qt::Checked) ? true : false;

	d.dlg_data->def_game = current_defg();
	for (int i=0; i<GAMECFG_CNT; i++) {
		save_data_game (o.gui_games[i], d.dlg_data->games[i]);
	}
}

int
CfgDlg::exec (void)
{
	int	rc;

	load_data();
	rc = QDialog::exec();
	if (rc == QDialog::Accepted) save_data();

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
		STR_SNP_BROWSE,
		d.snp_edit->text(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);

	if (!dir.isNull()) { d.snp_edit->setText (dir); }
}

CfgDlgGuiGame *
CfgDlg::signal_sender_gui_game (void)
{
	QObject *from = sender();
	if (!from) return (NULL);

	int ggidx = from->property ("index").toInt();
	if ((ggidx < 0) || (ggidx > GAMECFG_CNT)) return (NULL);

	return (&(o.gui_games[ggidx]));
}

void
CfgDlg::game_name_edited (const QString &/*text*/)
{
	CfgDlgGuiGame *gg = signal_sender_gui_game();
	if (!gg) return;

	update_defg_gui();
	gg->update_status();
}

void
CfgDlg::game_type_selected (int /*index*/)
{
	CfgDlgGuiGame *gg = signal_sender_gui_game();
	if (!gg) return;

	gg->update_status();
}

void
CfgDlg::game_path_edited (const QString &/*text*/)
{
	CfgDlgGuiGame *gg = signal_sender_gui_game();
	if (!gg) return;

	gg->update_status();
}

void
CfgDlg::game_path_browse_clicked (bool /*checked*/)
{
	CfgDlgGuiGame *gg = signal_sender_gui_game();
	if (!gg) return;

	QString	dir = QFileDialog::getExistingDirectory (this,
			QString(STR_GAMEPATH_BROWSE),
			gg->d.path_edit->text(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);

	if (!dir.isNull()) { gg->d.path_edit->setText (dir); }

	gg->update_status();
}
