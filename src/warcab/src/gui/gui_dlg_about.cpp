/** \file
 * The SPWaW war cabinet - GUI - "about" dialog box.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_dlg_about.h"

#define	BOX_WIDTH	400
#define	BOX_HEIGHT	300
#define	BOX_MARGIN	 10

#define	BKG_WIDTH	BOX_WIDTH
#define	BKG_HEIGHT	BOX_HEIGHT

#define	ABOUT_MSG								\
	"Warcab is a tool to record and track battles in an SPWaW campaign.\n"	\
	"It is inspired by Campaign Watcher.\n"					\
	"\n"									\
	"Warcab tracks information for core units (including reassignments)\n"	\
	"during a campaign. It also shows and tracks information for core,\n"	\
	"auxiliary and opponent units during battles and battle turns.\n"	\
	"\n"									\
	"Its current main missing features are data export and printing.\n"	\
	"\n"

GuiDlgAbout::GuiDlgAbout (void)
	: QDialog (0, Qt::Dialog)
{
	char	buffer[256];
	int	but_height = 0;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Set dialog modality */
	setModal (true);

	/* Set dialog size */
	setFixedSize (BOX_WIDTH, BOX_HEIGHT);

	/* Set dialog caption */
	memset (buffer, 0, sizeof (buffer));
	snprintf (buffer, sizeof (buffer) - 1, "About %s...", SL_APP_name());
	setWindowTitle (buffer);

	/* Set dialog icon */
	setWindowIcon (*(RES_pixmap (RID_MAIN_ICON)));

	/* Set dialog font */
	GUINEW (d.dlgf, QFont (font()), ERR_GUI_DLG_ABOUT_INIT_FAILED, "font");
	d.dlgf->setFamily ("Tahoma");
	setFont (*d.dlgf);

	/* Get dialog font metrics */
	GUINEW (d.dlgfm, QFontMetrics (*d.dlgf), ERR_GUI_DLG_ABOUT_INIT_FAILED, "font metrics");

	/* Create body widget */
	GUINEW (d.body, QWidget (this), ERR_GUI_DLG_ABOUT_INIT_FAILED, "body widget");
	d.body->setGeometry(QRect(0, 0, BKG_WIDTH, BKG_HEIGHT));

	/* Create body brush */
	GUINEW (d.body_brush, QBrush (*RES_pixmap (RID_ABOUT_BKG)), ERR_GUI_DLG_ABOUT_INIT_FAILED, "background brush");

	/* Create body palette */
	GUINEW (d.body_palette, QPalette (), ERR_GUI_DLG_ABOUT_INIT_FAILED, "background palette");
	*(d.body_palette) = d.body->palette();
	d.body_palette->setBrush (QPalette::Background, *(d.body_brush));

	/* Update body widget */
	d.body->setPalette (*(d.body_palette));
	d.body->setAutoFillBackground (true);

	/* Create button box */
	GUINEW (d.buttons, QDialogButtonBox (this), ERR_GUI_DLG_ABOUT_INIT_FAILED, "button box");
	but_height = d.buttons->height();
	d.buttons->setGeometry(QRect(BOX_MARGIN, BOX_HEIGHT - BOX_MARGIN - but_height, BOX_WIDTH - 2*BOX_MARGIN, but_height));
	d.buttons->setOrientation(Qt::Horizontal);
	d.buttons->setStandardButtons(QDialogButtonBox::Ok);

	/* Create application icon label */
	GUINEW (d.app_icon, QLabel (d.body), ERR_GUI_DLG_ABOUT_INIT_FAILED, "application icon label");
	d.app_icon->setPixmap (*RES_pixmap (RID_MAIN_ICON));

	/* Create application info label */
	GUINEW (d.app_info, QLabel (d.body), ERR_GUI_DLG_ABOUT_INIT_FAILED, "application info label");
	d.app_info->setText (SL_APP_info_ex());

	/* Create message label */
	GUINEW (d.message, QLabel (d.body), ERR_GUI_DLG_ABOUT_INIT_FAILED, "message label");
	d.message->setText (ABOUT_MSG);

	/* Create body layout */
	GUINEW (d.layout, QGridLayout (d.body), ERR_GUI_DLG_ABOUT_INIT_FAILED, "body layout");
	d.layout->setContentsMargins(BOX_MARGIN, BOX_MARGIN, BOX_MARGIN, BOX_MARGIN + but_height);
	d.layout->addWidget (d.app_icon,	0, 0, 1, 1);
	d.layout->addWidget (d.app_info,	0, 1, 1, 1);
	d.layout->addWidget (d.message,		1, 0, 1, 2);

	/* Finally connect signals and slots */
	connect (d.buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect (d.buttons, SIGNAL(rejected()), this, SLOT(reject()));

	SET_GUICLS_NOERR;
}

GuiDlgAbout::~GuiDlgAbout (void)
{
	// QT deletes child widgets
	delete d.dlgf;
	delete d.dlgfm;
	delete d.body_palette;
	delete d.body_brush;
}
