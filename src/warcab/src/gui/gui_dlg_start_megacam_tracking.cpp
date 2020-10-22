/** \file
 * The SPWaW war cabinet - GUI - "start megacam tracking" dialog box.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_dlg_start_megacam_tracking.h"

#define	BOX_WIDTH	800
#define	BOX_HEIGHT	400
#define	BOX_MARGIN	 10

GuiDlgStartMegaCamTracking::GuiDlgStartMegaCamTracking (SPWAW_SAVELIST_TARGET &target, char *path, SPWAW_SAVELIST *ignore)
	: QDialog (0, Qt::Dialog)
{
	int		but_height = 0;
	SPWAW_ERROR	rc;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Set dialog modality */
	setModal (true);

	/* Set dialog size */
	setFixedSize (BOX_WIDTH, BOX_HEIGHT);

	/* Set dialog caption and icon */
	setWindowTitle ("Start Mega Campaign Tracking");
	setWindowIcon (*(RES_pixmap (RID_MAIN_ICON)));

	/* Set dialog font */
	GUINEW (d.dlgf, QFont (font()), ERR_GUI_DLG_START_MEGACAM_TRACKING_INIT_FAILED, "font");
	d.dlgf->setFamily ("Tahoma");
	setFont (*d.dlgf);

	/* Get dialog font metrics */
	GUINEW (d.dlgfm, QFontMetrics (*d.dlgf), ERR_GUI_DLG_START_MEGACAM_TRACKING_INIT_FAILED, "font metrics");

	/* Create button box */
	GUINEW (d.buttons, QDialogButtonBox (this), ERR_GUI_DLG_START_MEGACAM_TRACKING_INIT_FAILED, "button box");
	but_height = d.buttons->height();
	d.buttons->setGeometry(QRect(BOX_MARGIN, BOX_HEIGHT - BOX_MARGIN - but_height, BOX_WIDTH - 2*BOX_MARGIN, but_height));
	d.buttons->setOrientation(Qt::Horizontal);

	/* Create and add buttons */
	GUINEW (d.but_ok, QPushButton (d.buttons), ERR_GUI_DLG_START_MEGACAM_TRACKING_INIT_FAILED, "ok button");
	d.but_ok->setText ("&OK");
	d.buttons->addButton (d.but_ok, QDialogButtonBox::AcceptRole);
	d.but_ok->setDefault (true);

	GUINEW (d.but_cancel, QPushButton (d.buttons), ERR_GUI_DLG_START_MEGACAM_TRACKING_INIT_FAILED, "cancel button");
	d.but_cancel->setText ("&Cancel");
	d.buttons->addButton (d.but_cancel, QDialogButtonBox::RejectRole);

	/* Create body widget */
	GUINEW (d.body, QWidget (this), ERR_GUI_DLG_START_MEGACAM_TRACKING_INIT_FAILED, "body widget");
	d.body->setGeometry(QRect(BOX_MARGIN, BOX_MARGIN, BOX_WIDTH - 2*BOX_MARGIN, BOX_HEIGHT - 3*BOX_MARGIN - but_height));

	/* Create info label */
	GUINEW (d.info, QLabel (d.body), ERR_GUI_DLG_START_MEGACAM_TRACKING_INIT_FAILED, "info label");
	d.info->setText ("\nPlease select a Mega Campaign savegame to track:\n");

	/* Create tree view */
	GUINEW (d.view, QTreeView (d.body), ERR_GUI_DLG_START_MEGACAM_TRACKING_INIT_FAILED, "tree view");
	d.view->setRootIsDecorated (false);
	d.view->setSortingEnabled (true);
	d.view->setUniformRowHeights (true);
	//d.view->setAlternatingRowColors (true);
	d.view->setAlternatingRowColors (false);
	d.view->setDragEnabled (false);
	d.view->setItemsExpandable (false);
	d.view->setSelectionBehavior (QAbstractItemView::SelectRows);
	d.view->setSelectionMode (QAbstractItemView::ExtendedSelection);
	d.view->sortByColumn (2, Qt::AscendingOrder);
	d.view->header()->setResizeMode(QHeaderView::ResizeToContents);
	d.view->header()->setStretchLastSection(false);

	/* Create body layout */
	GUINEW (d.layout, QGridLayout (d.body), ERR_GUI_DLG_START_MEGACAM_TRACKING_INIT_FAILED, "body layout");
	d.layout->setContentsMargins(0, 0, 0, 0);
	d.layout->addWidget (d.info, 0, 0, 1, 1);
	d.layout->addWidget (d.view, 1, 0, 1, 1);

	/* Finally connect signals and slots */
	connect (d.buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect (d.buttons, SIGNAL(rejected()), this, SLOT(reject()));

	// And set the focus
	d.view->setFocus(Qt::ActiveWindowFocusReason);

	/* Create empty savegames list */
	rc = SPWAW_savelist_new (&target, &d.savelist);
	if (rc != SPWERR_OK)
		SET_GUICLS_ERROR (ERR_GUI_DLG_ADD_BATTLE_SAVEGAME_INIT_FAILED, "failed to create new savegame list");

	/* Create data model */
	GUINEW (d.savemodel, ModelSaveList (target, path, ignore), ERR_GUI_DLG_ADD_BATTLE_SAVEGAME_INIT_FAILED, "savelist data model");

	/* Connect data model with tree view */
	d.view->setModel (d.savemodel);

	/* Hide useless gametype column */
	d.view->hideColumn (1);

	/* Hook up the selectionChanged signal of the d.view->selectionModel() to refresh_ok_button_status */
	connect (d.view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
		this, SLOT(selection_changed(const QItemSelection&, const QItemSelection&)));

	refresh_ok_button_status();

	SET_GUICLS_NOERR;
}

GuiDlgStartMegaCamTracking::~GuiDlgStartMegaCamTracking (void)
{
	// QT deletes child widgets
	delete d.dlgf;
	delete d.dlgfm;

	delete d.savemodel;
	SPWAW_savelist_free (&d.savelist);
}

void
GuiDlgStartMegaCamTracking::get_data (SPWAW_SAVELIST *list)
{
	if (!list) return;

	SPWAW_savelist_copy (list, d.savelist);
	// Target list cleared by API in case of error
}

void
GuiDlgStartMegaCamTracking::refresh_ok_button_status (void)
{
	bool			activate = false;
	QItemSelectionModel	*selection;

	selection = d.view->selectionModel();
	activate = selection->hasSelection();

	if (activate) d.but_ok->setEnabled(true); else d.but_ok->setEnabled(false);
}

void
GuiDlgStartMegaCamTracking::selection_changed (const QItemSelection&/*selected*/, const QItemSelection&/*deselected*/)
{
	refresh_ok_button_status();
}

void
GuiDlgStartMegaCamTracking::accept (void)
{
	QDialog::DialogCode	rc;
	QItemSelectionModel	*selection;
	QModelIndex		index;
	QVariant		data;
	QByteArray		file;

	SPWAW_SAVELIST_NODE	*save;

	rc = QDialog::Rejected;

	SPWAW_savelist_clear (d.savelist);

	selection = d.view->selectionModel();
	if (selection->hasSelection ()) {
		int cnt = selection->selectedRows(0).size();

		for (int i=0; i<cnt; i++) {
			index = selection->selectedRows(0)[i];
			DEVASSERT (index.isValid());

			data = index.data (Qt::UserRole);
			DEVASSERT (data.isValid());

			save = (SPWAW_SAVELIST_NODE *)data.toInt();
			SPWAW_savelist_addcpy (d.savelist, save);
		}

		rc = QDialog::Accepted;
	}

	done (rc);
}
