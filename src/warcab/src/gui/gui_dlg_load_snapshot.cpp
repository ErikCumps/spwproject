/** \file
 * The SPWaW war cabinet - GUI - "snapshot load" dialog box.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_dlg_load_snapshot.h"

#define	BOX_WIDTH	600
#define	BOX_HEIGHT	300
#define	BOX_MARGIN	 10

GuiDlgLoadSnapshot::GuiDlgLoadSnapshot (char *path, SPWAW_SNAPLIST *ignore)
	: QDialog (0, Qt::Dialog)
{
	SPWAW_ERROR	rc;
	int		but_height = 0;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	rc = SPWAW_snaplist_new (&d.data);
	if (rc != SPWERR_OK)
		SET_GUICLS_ERROR (ERR_GUI_DLG_LOAD_SNAPSHOT_INIT_FAILED, "failed to create new snapshot list");

	/* Set dialog modality */
	setModal (true);

	/* Set dialog size */
	resize (BOX_WIDTH, BOX_HEIGHT);

	/* Set dialog caption and icon */
	setWindowTitle ("Load Snapshot");
	setWindowIcon (*RES_PIXMAP_NONE);

	/* Set dialog font */
	GUINEW (d.dlgf, QFont (font()), ERR_GUI_DLG_LOAD_SNAPSHOT_INIT_FAILED, "font");
	d.dlgf->setFamily ("Tahoma");
	setFont (*d.dlgf);

	/* Get dialog font metrics */
	GUINEW (d.dlgfm, QFontMetrics (*d.dlgf), ERR_GUI_DLG_LOAD_SNAPSHOT_INIT_FAILED, "font metrics");

	/* Create button box */
	GUINEW (d.buttons, QDialogButtonBox (this), ERR_GUI_DLG_LOAD_SNAPSHOT_INIT_FAILED, "button box");
	but_height = d.buttons->height();
	d.buttons->setGeometry(QRect(BOX_MARGIN, BOX_HEIGHT - BOX_MARGIN - but_height, BOX_WIDTH - 2*BOX_MARGIN, but_height));
	d.buttons->setOrientation(Qt::Horizontal);
	d.buttons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

	/* Create body widget */
	GUINEW (d.body, QWidget (this), ERR_GUI_DLG_LOAD_SNAPSHOT_INIT_FAILED, "body widget");
	d.body->setGeometry(QRect(10, 10, 581, 221));

	/* Create data model */
	GUINEW (d.model, ModelSnapList (path, ignore), ERR_GUI_DLG_LOAD_SNAPSHOT_INIT_FAILED, "snaplist data model");

	/* Create tree view */
	GUINEW (d.view, QTreeView (d.body), ERR_GUI_DLG_LOAD_SNAPSHOT_INIT_FAILED, "tree view");
	d.view->setModel (d.model);
	d.view->setRootIsDecorated (false);
	d.view->setSortingEnabled (true);
	d.view->setUniformRowHeights (true);
	//d.view->setAlternatingRowColors (true);
	d.view->setAlternatingRowColors (false);
	d.view->setDragEnabled (false);
	d.view->setSelectionBehavior (QAbstractItemView::SelectRows);
	d.view->setSelectionMode (QAbstractItemView::ExtendedSelection);
	d.view->sortByColumn (3, Qt::AscendingOrder);
	d.view->resizeColumnToContents (0);
	d.view->resizeColumnToContents (1);
	d.view->resizeColumnToContents (2);
	d.view->resizeColumnToContents (3);
	//d.view->resizeColumnToContents (4);

	/* Create body layout */
	GUINEW (d.layout, QGridLayout (d.body), ERR_GUI_DLG_LOAD_SNAPSHOT_INIT_FAILED, "body layout");
	d.layout->setContentsMargins(0, 0, 0, 0);
	d.layout->addWidget (d.view, 0, 0, 1, 1);

	/* Finally connect signals and slots */
	connect (d.buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect (d.buttons, SIGNAL(rejected()), this, SLOT(reject()));

	connect (d.view, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));

	SET_GUICLS_NOERR;
}

GuiDlgLoadSnapshot::~GuiDlgLoadSnapshot (void)
{
	// QT deletes child widgets
	delete d.dlgf;
	delete d.dlgfm;
	delete d.model;

	SPWAW_snaplist_free (&d.data);
}

void
GuiDlgLoadSnapshot::data_get (SPWAW_SNAPLIST *data)
{
	if (!data) return;

	SPWAW_snaplist_copy (data, d.data);
	// Target list cleared by API in case of error
}

void
GuiDlgLoadSnapshot::clicked (const QModelIndex& /*index*/)
{
	accept();
}

void
GuiDlgLoadSnapshot::accept (void)
{
	QDialog::DialogCode	rc;
	QItemSelectionModel	*selection;
	int			cnt, i;
	QModelIndex		index;
	QVariant		data;
	QByteArray		file;
	SPWAW_SNAPLIST_NODE	*snap;

	rc = QDialog::Rejected;

	SPWAW_snaplist_clear (d.data);

	selection = d.view->selectionModel();
	if (selection->hasSelection ()) {
		cnt = selection->selectedRows(0).size();

		for (i=0; i<cnt; i++) {
			index = selection->selectedRows(0)[i];
			DEVASSERT (index.isValid());

			data = index.data (Qt::UserRole);
			DEVASSERT (data.isValid());

			snap = (SPWAW_SNAPLIST_NODE *)data.toInt();
			SPWAW_snaplist_addcpy (d.data, snap);
		}

		rc = QDialog::Accepted;
	}

	done (rc);
}
