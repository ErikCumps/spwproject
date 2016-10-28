/** \file
 * The SPWaW war cabinet - GUI - order of battle view.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_oob_view.h"
#include "gui_oob.h"
#include "model/model_oob.h"
#include "model/model_oob_data.h"
#include "model/model_sanity.h"

#define	BASE_SIZE	40
#define	NUMBER_SIZE	(BASE_SIZE * 3 / 2)

// TODO: rename NUMBER_SIZE to something more appropriate

static void report_MDLO (MDLO_DATA *p);

GuiOobView::GuiOobView	(bool hdr, GuiOob *oob, QWidget *P)
	: QTreeView (P)
{
	QPalette	pal;

	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	if (!oob) SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "no parent OOB widget provided");

	d.ishdr = hdr; d.parent = oob;

	setModel (d.parent->d.model);

	GUINEW (d.font, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_REPORTS_OOB_INIT_FAILED, "font");
	setFont (*d.font);

	setRootIsDecorated (true);
	setSortingEnabled (true);
	setUniformRowHeights (true);
	setAlternatingRowColors (false);
	setDragEnabled (false);
	//setIconSize (QSize(28,16));
	setSelectionBehavior (QAbstractItemView::SelectRows);
	setSelectionMode (QAbstractItemView::SingleSelection);
	//setTabKeyNavigation (true);
	setTabKeyNavigation (false);
	setTextElideMode (Qt::ElideRight);
	setHorizontalScrollMode (QAbstractItemView::ScrollPerPixel);
	setVerticalScrollMode (QAbstractItemView::ScrollPerItem);

	/* Fix highlight palette for inactive items */
	pal = palette();
	pal.setBrush (QPalette::Inactive, QPalette::Highlight, pal.brush (QPalette::Active, QPalette::Highlight));
	pal.setBrush (QPalette::Inactive, QPalette::HighlightedText, pal.brush (QPalette::Active, QPalette::HighlightedText));
	setPalette (pal);

	header()->setDefaultAlignment (Qt::AlignLeft|Qt::AlignVCenter);
	header()->setHighlightSections (false);
	header()->setSortIndicatorShown (true);
	header()->setResizeMode (QHeaderView::Fixed);

	if (d.ishdr) {
		setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
		header()->setStretchLastSection (true);

		for (int i=0; i<d.parent->d.model->columnCount(); i++) setColumnHidden (i, true);
		setColumnHidden (MDLO_COLUMN_FID, false);
		setColumnHidden (MDLO_COLUMN_TYPE, false);

		setColumnWidth (MDLO_COLUMN_FID,	BASE_SIZE * 2);
		setColumnWidth (MDLO_COLUMN_TYPE,	BASE_SIZE * 3);

		setMinimumWidth (BASE_SIZE * 5 + 5);
		setMaximumWidth (BASE_SIZE * 5 + 5);
	} else {
		setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
		header()->setStretchLastSection (true);

		for (int i=0; i<d.parent->d.model->columnCount(); i++) setColumnHidden (i, false);
		setColumnHidden (MDLO_COLUMN_FID, true);
		setColumnHidden (MDLO_COLUMN_TYPE, true);

		setColumnWidth (MDLO_COLUMN_LDR,	BASE_SIZE * 3);
		setColumnWidth (MDLO_COLUMN_HCMD,	BASE_SIZE);
		setColumnWidth (MDLO_COLUMN_STATUS,	BASE_SIZE * 4);
		setColumnWidth (MDLO_COLUMN_KILLS,	BASE_SIZE * 7 / 4);
		setColumnWidth (MDLO_COLUMN_RDY,	BASE_SIZE * 3);
		setColumnWidth (MDLO_COLUMN_EXP,	BASE_SIZE);
		setColumnWidth (MDLO_COLUMN_MOR,	BASE_SIZE);
		setColumnWidth (MDLO_COLUMN_SUP,	BASE_SIZE);
		setColumnWidth (MDLO_COLUMN_RAL,	BASE_SIZE);
		setColumnWidth (MDLO_COLUMN_INF,	BASE_SIZE);
		setColumnWidth (MDLO_COLUMN_ARM,	BASE_SIZE);
		setColumnWidth (MDLO_COLUMN_ART,	BASE_SIZE);
	}
	setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOn);

	setColumnHidden (MDLO_COLUMN_COUNT, true);

	if (!connect (header(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), SLOT(savesort(int,Qt::SortOrder))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect header <sortIndicatorChanged> to <savesort>");

	if (!connect (this, SIGNAL (collapsed(const QModelIndex &)), SLOT (collapsed(const QModelIndex &))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <collapsed> to <collapsed>");

	if (!connect (this, SIGNAL (expanded(const QModelIndex &)), SLOT (expanded(const QModelIndex &))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <expanded> to <expanded>");

	d.sidx = -1; d.sord = Qt::AscendingOrder;

	SET_GUICLS_NOERR;
}

GuiOobView::~GuiOobView (void)
{
	DBG_TRACE_DESTRUCT;

	delete d.font;
}

void
GuiOobView::reparented (void)
{
	if (d.ishdr) return;

	switch (d.parent->d.ptype) {
		case MDLD_TREE_DOSSIER:
			setColumnHidden (MDLO_COLUMN_RDY, true);
			setColumnHidden (MDLO_COLUMN_SUP, true);

			setColumnWidth (MDLO_COLUMN_EXP,	BASE_SIZE * 3);
			setColumnWidth (MDLO_COLUMN_MOR,	BASE_SIZE * 3);
			setColumnWidth (MDLO_COLUMN_RAL,	BASE_SIZE * 3);
			setColumnWidth (MDLO_COLUMN_INF,	BASE_SIZE * 3);
			setColumnWidth (MDLO_COLUMN_ARM,	BASE_SIZE * 3);
			setColumnWidth (MDLO_COLUMN_ART,	BASE_SIZE * 3);

			break;
		case MDLD_TREE_BATTLE:
			setColumnHidden (MDLO_COLUMN_RDY, false);

			setColumnWidth (MDLO_COLUMN_RDY,	BASE_SIZE * 3);

			if (d.parent->d.Vprevcmp) {
				setColumnHidden (MDLO_COLUMN_SUP, true);

				setColumnWidth (MDLO_COLUMN_EXP,	BASE_SIZE * 3);
				setColumnWidth (MDLO_COLUMN_MOR,	BASE_SIZE * 3);
				setColumnWidth (MDLO_COLUMN_RAL,	BASE_SIZE * 3);
				setColumnWidth (MDLO_COLUMN_INF,	BASE_SIZE * 3);
				setColumnWidth (MDLO_COLUMN_ARM,	BASE_SIZE * 3);
				setColumnWidth (MDLO_COLUMN_ART,	BASE_SIZE * 3);
			} else {
				setColumnHidden (MDLO_COLUMN_SUP, false);

				setColumnWidth (MDLO_COLUMN_EXP,	NUMBER_SIZE);
				setColumnWidth (MDLO_COLUMN_MOR,	NUMBER_SIZE);
				setColumnWidth (MDLO_COLUMN_SUP,	NUMBER_SIZE);
				setColumnWidth (MDLO_COLUMN_RAL,	NUMBER_SIZE);
				setColumnWidth (MDLO_COLUMN_INF,	NUMBER_SIZE);
				setColumnWidth (MDLO_COLUMN_ARM,	NUMBER_SIZE);
				setColumnWidth (MDLO_COLUMN_ART,	NUMBER_SIZE);
			}

			break;
		case MDLD_TREE_BTURN:
			setColumnHidden (MDLO_COLUMN_RDY, false);
			setColumnHidden (MDLO_COLUMN_SUP, false);

			setColumnWidth (MDLO_COLUMN_RDY,	BASE_SIZE * 3);
			setColumnWidth (MDLO_COLUMN_EXP,	NUMBER_SIZE);
			setColumnWidth (MDLO_COLUMN_MOR,	NUMBER_SIZE);
			setColumnWidth (MDLO_COLUMN_SUP,	NUMBER_SIZE);
			setColumnWidth (MDLO_COLUMN_RAL,	NUMBER_SIZE);
			setColumnWidth (MDLO_COLUMN_INF,	NUMBER_SIZE);
			setColumnWidth (MDLO_COLUMN_ARM,	NUMBER_SIZE);
			setColumnWidth (MDLO_COLUMN_ART,	NUMBER_SIZE);

			break;
		default:
			break;
	}
	//header()->setStretchLastSection (true);
}

void
GuiOobView::reload (bool sort)
{
	if (!d.parent->d.pcurr || !d.parent->d.pbase) return;

	if (d.sidx < 0) { d.sidx = 0; d.sord = Qt::AscendingOrder; }
	if (sort || d.parent->d.Vautosort) sortByColumn (d.sidx, d.sord);
	refresh();
}

void
GuiOobView::resort (void)
{
	if (d.sidx != -1)	sortByColumn (d.sidx, d.sord);
}

void
GuiOobView::wheelEvent (QWheelEvent *e)
{
	if (!e) return;

	if (d.ishdr) {
		emit wheeled (e);
	} else {
		QAbstractScrollArea::wheelEvent (e);
	}
}

void
GuiOobView::mouseDoubleClickEvent (QMouseEvent * /*event*/)
{
	// stub function to ignore mouse double click events
}

#if	EXPERIMENTAL
void
GuiOobView::contextMenuEvent (QContextMenuEvent *event)
{
	QMenu	*menu = NULL;
	QAction	*action_expandAll = NULL;
	QAction	*action_collapseAll = NULL;
	QAction	*action;

	if (!d.ishdr) return;

	menu = new QMenu (this);

	DBG_log ("[%s] event=0x%8.8x\n", __FUNCTION__, event);
	DBG_log ("[%s] global pos: x=%d, y=%d\n", __FUNCTION__, event->globalX(), event->globalY());
	DBG_log ("[%s] widget pos: x=%d, y=%d\n", __FUNCTION__, event->x(), event->y());
	DBG_log ("[%s] d.ishdr=%d\n", __FUNCTION__, d.ishdr);

	QModelIndex idx = currentIndex ();
	DBG_log ("[%s] index=0x%8.8x\n", __FUNCTION__, idx);

	menu->addSeparator ();
	action_expandAll = menu->addAction ("Expand all...");
	action_collapseAll = menu->addAction ("Collapse all...");
	//menu->addSeparator ();

	action = menu->exec(event->globalPos());

	delete menu;

	//FIXME: both parts of the view must expandAll or collapseAll
	if (action == action_expandAll) {
		expandAll();
	} else if (action == action_collapseAll) {
		collapseAll();
	} else {
		// nothing
	}
}
#endif	/* EXPERIMENTAL */

void
GuiOobView::scroll (int d)
{
	this->verticalScrollBar()->setValue (d);
}

void
GuiOobView::wheel (QWheelEvent *e)
{
	wheelEvent (e);
}

void
GuiOobView::savesort (int idx, Qt::SortOrder order)
{
	d.sidx = idx;
	d.sord = order;
	emit sorted (this, idx, order);
}

void
GuiOobView::sort (int idx, Qt::SortOrder order)
{
	if ((idx == d.sidx) && (order == d.sord)) return;

	sortByColumn (idx, order);
	refresh();
}

void
GuiOobView::collapsed (const QModelIndex &index)
{
	emit was_collapsed (this, index);
}

void
GuiOobView::expanded (const QModelIndex &index)
{
	emit was_expanded (this, index);
}

void
GuiOobView::select (const QModelIndex &index)
{
	setCurrentIndex (index);
}

void
GuiOobView::currentChanged (const QModelIndex &current, const QModelIndex &/*previous*/)
{
	emit selected (this, current);
}

void
GuiOobView::refresh (void)
{
	int		i;
	QModelIndex	idx;

	DBG_TRACE_FENTER;

	if (!d.ishdr) goto leave;

	int p = verticalScrollBar()->value ();

	for (i=0; i<d.parent->d.model->rowCount (); i++) {
		idx = d.parent->d.model->index (i, 0, QModelIndex());
		setExpanded (idx, d.parent->d.model->is_expanded (idx));
	}

	emit wantscroll (p);

leave:
	DBG_TRACE_FLEAVE;
}
