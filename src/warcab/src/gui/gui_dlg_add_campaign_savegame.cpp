/** \file
 * The SPWaW war cabinet - GUI - "add campaign savegame" dialog box.
 *
 * Copyright (C) 2005-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_dlg_add_campaign_savegame.h"

#define	BOX_WIDTH	600
#define	BOX_HEIGHT	400
#define	BOX_MARGIN	 10

void
GuiDlgAddCampaignSavegame::constructor_core (QString &type, QString &items)
{
	int	but_height = 0;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Set dialog modality */
	setModal (true);

	/* Set dialog size */
	setFixedSize (BOX_WIDTH, BOX_HEIGHT);

	/* Set dialog caption and icon */
	setWindowTitle ("Add " + type + "To Campaign");
	setWindowIcon (*(RES_pixmap (RID_MAIN_ICON)));

	/* Set dialog font */
	GUINEW (d.dlgf, QFont (font()), ERR_GUI_DLG_ADD_CAMPAIGN_SAVEGAME_INIT_FAILED, "font");
	d.dlgf->setFamily ("Tahoma");
	setFont (*d.dlgf);

	/* Get dialog font metrics */
	GUINEW (d.dlgfm, QFontMetrics (*d.dlgf), ERR_GUI_DLG_ADD_CAMPAIGN_SAVEGAME_INIT_FAILED, "font metrics");

	/* Create button box */
	GUINEW (d.buttons, QDialogButtonBox (this), ERR_GUI_DLG_ADD_CAMPAIGN_SAVEGAME_INIT_FAILED, "button box");
	but_height = d.buttons->height();
	d.buttons->setGeometry(QRect(BOX_MARGIN, BOX_HEIGHT - BOX_MARGIN - but_height, BOX_WIDTH - 2*BOX_MARGIN, but_height));
	d.buttons->setOrientation(Qt::Horizontal);
	
	/* Create and add buttons */
	GUINEW (d.but_ok, QPushButton (d.buttons), ERR_GUI_DLG_ADD_CAMPAIGN_SAVEGAME_INIT_FAILED, "ok button");
	d.but_ok->setText ("&OK");
	d.buttons->addButton (d.but_ok, QDialogButtonBox::AcceptRole);
	d.but_ok->setDefault (true);

	GUINEW (d.but_cancel, QPushButton (d.buttons), ERR_GUI_DLG_ADD_CAMPAIGN_SAVEGAME_INIT_FAILED, "cancel button");
	d.but_cancel->setText ("&Cancel");
	d.buttons->addButton (d.but_cancel, QDialogButtonBox::AcceptRole);

	/* Create body widget */
	GUINEW (d.body, QWidget (this), ERR_GUI_DLG_ADD_CAMPAIGN_SAVEGAME_INIT_FAILED, "body widget");
	d.body->setGeometry(QRect(BOX_MARGIN, BOX_MARGIN, BOX_WIDTH - 2*BOX_MARGIN, BOX_HEIGHT - 3*BOX_MARGIN - but_height));

	/* Create info label */
	GUINEW (d.info, QLabel (d.body), ERR_GUI_DLG_ADD_CAMPAIGN_SAVEGAME_INIT_FAILED, "info label");
	d.info->setText ("\nPlease select one or more " + items + " to add to the campaign:\n");

	/* Create tree view */
	GUINEW (d.view, QTreeView (d.body), ERR_GUI_DLG_ADD_CAMPAIGN_SAVEGAME_INIT_FAILED, "tree view");
	d.view->setRootIsDecorated (false);
	d.view->setSortingEnabled (true);
	d.view->setUniformRowHeights (true);
	//d.view->setAlternatingRowColors (true);
	d.view->setAlternatingRowColors (false);
	d.view->setDragEnabled (false);
	d.view->setItemsExpandable (false);
	d.view->setSelectionBehavior (QAbstractItemView::SelectRows);
	d.view->setSelectionMode (QAbstractItemView::ExtendedSelection);
	d.view->sortByColumn (1, Qt::AscendingOrder);
	d.view->header()->setResizeMode(QHeaderView::ResizeToContents);
	d.view->header()->setStretchLastSection(false);

	/* Create body layout */
	GUINEW (d.layout, QGridLayout (d.body), ERR_GUI_DLG_ADD_CAMPAIGN_SAVEGAME_INIT_FAILED, "body layout");
	d.layout->setContentsMargins(0, 0, 0, 0);
	d.layout->addWidget (d.info, 0, 0, 1, 1);
	d.layout->addWidget (d.view, 1, 0, 1, 1);

	/* Finally connect signals and slots */
	connect (d.buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect (d.buttons, SIGNAL(rejected()), this, SLOT(reject()));

	connect (d.view, SIGNAL(clicked(const QModelIndex&)), this, SLOT(tree_clicked(const QModelIndex&)));

	SET_GUICLS_NOERR;
}

GuiDlgAddCampaignSavegame::GuiDlgAddCampaignSavegame (char *path, SPWAW_SAVELIST *ignore)
	: QDialog (0, Qt::Dialog)
{
	SPWAW_ERROR	rc;

	constructor_core (QString("Savegame"), QString("savegames"));
	GUIROE (this);

	d.snapmode = false;

	/* Create empty savegames list */
	rc = SPWAW_savelist_new (&d.savelist);
	if (rc != SPWERR_OK)
		SET_GUICLS_ERROR (ERR_GUI_DLG_ADD_BATTLE_SAVEGAME_INIT_FAILED, "failed to create new savegame list");

	/* Create data model */
	GUINEW (d.savemodel, ModelSaveList (path, ignore), ERR_GUI_DLG_ADD_BATTLE_SAVEGAME_INIT_FAILED, "savelist data model");

	/* Connect data model with tree view */
	d.view->setModel (d.savemodel);

	refresh_activation_status();

	SET_GUICLS_NOERR;
}

#if	ALLOW_SNAPSHOTS_LOAD
GuiDlgAddCampaignSavegame::GuiDlgAddCampaignSavegame (char *path, SPWAW_SNAPLIST *ignore)
	: QDialog (0, Qt::Dialog)
{
	SPWAW_ERROR	rc;

	constructor_core (QString("Snapshot"), QString("snapshots"));
	GUIROE (this);

	d.snapmode = true;

	/* Create empty snapshot list */
	rc = SPWAW_snaplist_new (&d.snaplist);
	if (rc != SPWERR_OK)
		SET_GUICLS_ERROR (ERR_GUI_DLG_ADD_BATTLE_SAVEGAME_INIT_FAILED, "failed to create new snapshot list");

	/* Create data model */
	GUINEW (d.snapmodel, ModelSnapList (path, ignore), ERR_GUI_DLG_ADD_BATTLE_SAVEGAME_INIT_FAILED, "snaplist data model");

	/* Connect data model with tree view */
	d.view->setModel (d.snapmodel);

	refresh_activation_status();

	SET_GUICLS_NOERR;
}
#endif	/* ALLOW_SNAPSHOTS_LOAD */

GuiDlgAddCampaignSavegame::~GuiDlgAddCampaignSavegame (void)
{
	// QT deletes child widgets
	delete d.dlgf;
	delete d.dlgfm;

	if (!d.snapmode) {
		delete d.savemodel;
		SPWAW_savelist_free (&d.savelist);
#if	ALLOW_SNAPSHOTS_LOAD
	} else {
		delete d.snapmodel;
		SPWAW_snaplist_free (&d.snaplist);
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	}
}

void
GuiDlgAddCampaignSavegame::get_data (SPWAW_SAVELIST *list)
{
	if (!list) return;

	SPWAW_savelist_copy (list, d.savelist);
	// Target list cleared by API in case of error
}

#if	ALLOW_SNAPSHOTS_LOAD
void
GuiDlgAddCampaignSavegame::get_data (SPWAW_SNAPLIST *list)
{
	if (!list) return;

	SPWAW_snaplist_copy (list, d.snaplist);
	// Target list cleared by API in case of error
}
#endif	/* ALLOW_SNAPSHOTS_LOAD */

void
GuiDlgAddCampaignSavegame::refresh_activation_status (void)
{
	bool			activate = false;
	QItemSelectionModel	*selection;

	selection = d.view->selectionModel();
	activate = selection->hasSelection();

	if (activate) d.but_ok->setEnabled(true); else d.but_ok->setEnabled(false);
}

void
GuiDlgAddCampaignSavegame::tree_clicked (const QModelIndex& /*index*/)
{
	refresh_activation_status();
}

void
GuiDlgAddCampaignSavegame::accept (void)
{
	QDialog::DialogCode	rc;
	QItemSelectionModel	*selection;
	QModelIndex		index;
	QVariant		data;
	QByteArray		file;

	SPWAW_SAVELIST_NODE	*save;
#if	ALLOW_SNAPSHOTS_LOAD
	SPWAW_SNAPLIST_NODE	*snap;
#endif	/* ALLOW_SNAPSHOTS_LOAD */


	rc = QDialog::Rejected;

	if (!d.snapmode) {
		SPWAW_savelist_clear (d.savelist);
#if	ALLOW_SNAPSHOTS_LOAD
	} else {
		SPWAW_snaplist_clear (d.snaplist);
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	}

	selection = d.view->selectionModel();
	if (selection->hasSelection ()) {
		int cnt = selection->selectedRows(0).size();

		for (int i=0; i<cnt; i++) {
			index = selection->selectedRows(0)[i];
			DEVASSERT (index.isValid());

			data = index.data (Qt::UserRole);
			DEVASSERT (data.isValid());

			if (!d.snapmode) {
				save = (SPWAW_SAVELIST_NODE *)data.toInt();
				SPWAW_savelist_addcpy (d.savelist, save);
#if	ALLOW_SNAPSHOTS_LOAD
			} else {
				snap = (SPWAW_SNAPLIST_NODE *)data.toInt();
				SPWAW_snaplist_addcpy (d.snaplist, snap);
#endif	/* ALLOW_SNAPSHOTS_LOAD */
			}
		}

		rc = QDialog::Accepted;
	}

	done (rc);
}
