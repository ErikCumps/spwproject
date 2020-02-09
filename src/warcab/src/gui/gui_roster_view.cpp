/** \file
 * The SPWaW war cabinet - GUI - unit roster view.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_roster_view.h"
#include "gui_roster.h"
#include "model/model_roster.h"
#include "model/model_roster_data.h"
#include "gui_reports_dossier.h"

#define	BASE_SIZE	40
#define	NUMBER_SIZE	(BASE_SIZE * 5 / 3)

// TODO: rename NUMBER_SIZE to something more appropriate

GuiRosterView::GuiRosterView (bool hdr, GuiRoster *roster, QWidget *P)
	: QTableView (P)
{
	QPalette	pal;

	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	if (!roster) SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "no parent roster widget provided");

	d.ishdr = hdr; d.parent = roster;

	setModel (d.parent->d.model);

	GUINEW (d.font, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "font");
	setFont (*d.font);

	setCornerButtonEnabled (false);
	setSortingEnabled (true);
	setAlternatingRowColors (false);
	setDragEnabled (false);
	setIconSize (QSize(8,8));
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

	verticalHeader()->setHidden (true);

	horizontalHeader()->setDefaultAlignment (Qt::AlignLeft|Qt::AlignVCenter);
	horizontalHeader()->setHighlightSections (false);
	horizontalHeader()->setSortIndicatorShown (true);
	horizontalHeader()->setResizeMode (QHeaderView::Fixed);

	if (d.ishdr) {
		setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
		horizontalHeader()->setStretchLastSection (true);

		for (int i=0; i<MDLR_COLUMN_CNT; i++) setColumnHidden (i, true);
		setColumnHidden (MDLR_COLUMN_UID, false);
		setColumnHidden (MDLR_COLUMN_UNIT, false);

		setColumnWidth (MDLR_COLUMN_UID,	BASE_SIZE);
		setColumnWidth (MDLR_COLUMN_UNIT,	BASE_SIZE * 3);

		setMinimumWidth (BASE_SIZE * 4 + 5);
		setMaximumWidth (BASE_SIZE * 4 + 5);
	} else {
		setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
		horizontalHeader()->setStretchLastSection (true);

		for (int i=0; i<MDLR_COLUMN_CNT; i++) setColumnHidden (i, false);
		setColumnHidden (MDLR_COLUMN_UID, true);
		setColumnHidden (MDLR_COLUMN_UNIT, true);

		setColumnWidth (MDLR_COLUMN_RNK,	BASE_SIZE * 2);
		setColumnWidth (MDLR_COLUMN_LDR,	BASE_SIZE * 3);
		setColumnWidth (MDLR_COLUMN_STATUS,	BASE_SIZE * 3);
		setColumnWidth (MDLR_COLUMN_KILL,	NUMBER_SIZE);
		setColumnWidth (MDLR_COLUMN_EXP,	NUMBER_SIZE);
		setColumnWidth (MDLR_COLUMN_MOR,	NUMBER_SIZE);
		setColumnWidth (MDLR_COLUMN_SUP,	NUMBER_SIZE);
		setColumnWidth (MDLR_COLUMN_RAL,	NUMBER_SIZE);
		setColumnWidth (MDLR_COLUMN_INF,	NUMBER_SIZE);
		setColumnWidth (MDLR_COLUMN_ARM,	NUMBER_SIZE);
		setColumnWidth (MDLR_COLUMN_ART,	NUMBER_SIZE);
		setColumnWidth (MDLR_COLUMN_MEN,	NUMBER_SIZE);
		setColumnWidth (MDLR_COLUMN_RDY,	BASE_SIZE * 3);
		setColumnWidth (MDLR_COLUMN_KIA,	NUMBER_SIZE);
		setColumnWidth (MDLR_COLUMN_DMG,	NUMBER_SIZE);
		setColumnWidth (MDLR_COLUMN_SEEN,	BASE_SIZE * 2);
		setColumnWidth (MDLR_COLUMN_ABAND,	BASE_SIZE * 2);
		setColumnWidth (MDLR_COLUMN_LOADED,	BASE_SIZE * 2);
		setColumnWidth (MDLR_COLUMN_TYPE,	BASE_SIZE * 4);
		setColumnWidth (MDLR_COLUMN_CLASS,	BASE_SIZE * 3);
	}
	setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOn);

	horizontalHeader()->setResizeMode (MDLR_COLUMN_LDR, QHeaderView::Interactive);
	horizontalHeader()->setResizeMode (MDLR_COLUMN_TYPE, QHeaderView::Interactive);
	horizontalHeader()->setResizeMode (MDLR_COLUMN_CLASS, QHeaderView::Interactive);
	horizontalHeader()->setResizeMode (MDLR_COLUMN_STATUS, QHeaderView::Interactive);

	//setContextMenuPolicy (Qt::DefaultContextMenu);

	if (!connect (horizontalHeader(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), SLOT(savesort(int,Qt::SortOrder))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "failed to connect header <sortIndicatorChanged> to sorted");

	d.sidx = -1; d.sord = Qt::AscendingOrder;

	SET_GUICLS_NOERR;
}

GuiRosterView::~GuiRosterView (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
}

void
GuiRosterView::wheelEvent (QWheelEvent *e)
{
	if (!e) return;

	if (d.ishdr) {
		emit wheeled (e);
	} else {
		QAbstractScrollArea::wheelEvent (e);
	}
}

#if	EXPERIMENTAL
void
GuiRosterView::contextMenuEvent (QContextMenuEvent *event)
{
	QMenu		*menu = NULL;

	if (!d.ishdr) return;

	menu = new QMenu (this);

	DBG_log ("[%s] event=0x%8.8x\n", __FUNCTION__, event);
	DBG_log ("[%s] global pos: x=%d, y=%d\n", __FUNCTION__, event->globalX(), event->globalY());
	DBG_log ("[%s] widget pos: x=%d, y=%d\n", __FUNCTION__, event->x(), event->y());
	DBG_log ("[%s] d.ishdr=%d\n", __FUNCTION__, d.ishdr);

	QModelIndex idx = currentIndex ();
	DBG_log ("[%s] index=0x%8.8x\n", __FUNCTION__, idx);

	menu->addSeparator ();
	menu->addAction (GUI_ACTIONS->p.dossier_edit);
	menu->addSeparator ();

	menu->exec(event->globalPos());

	delete menu;

	switch (d.parent->d.ptype) {
		case MDLD_TREE_DOSSIER:
			//TEST!
			//d.parent->d.pptr.d->setCurrentIndex (5);	//FIXME: use enum iso hardcoded index!
			d.parent->d.pptr.d->setCurrentIndex (d.parent->d.pptr.d->indexOf (d.parent->d.pptr.d->d.oob));
			// TODO: select current item in oob, expand and scroll into view
			//d.parent->d.pptr.d->d.oob->d.oob;
			break;
		case MDLD_TREE_BATTLE:
			//d.parent->d.pptr.b->d.force_core->d.oob;
			break;
		case MDLD_TREE_BTURN:
			//d.parent->d.pptr.t->d.force_core->d.oob;
			break;
		default:
			break;
	}
}
#endif	/* EXPERIMENTAL */

void
GuiRosterView::reload (bool sort)
{
	if (!d.parent->d.pcurr || !d.parent->d.pbase) return;

	if (d.sidx < 0) { d.sidx = 0; d.sord = Qt::AscendingOrder; }
	if (sort || d.parent->d.Vautosort) sortByColumn (d.sidx, d.sord);
	refresh();
}

void
GuiRosterView::resort (void)
{
	if (d.sidx != -1) sortByColumn (d.sidx, d.sord);
}

void
GuiRosterView::scroll (int d)
{
	this->verticalScrollBar()->setValue (d);
}

void
GuiRosterView::wheel (QWheelEvent *e)
{
	wheelEvent (e);
}

void
GuiRosterView::savesort (int idx, Qt::SortOrder order)
{
	d.sidx = idx;
	d.sord = order;
	emit sorted (this, idx, order);
}

void
GuiRosterView::sort (int idx, Qt::SortOrder order)
{
	if ((idx == d.sidx) && (order == d.sord)) return;

	sortByColumn (idx, order);
	refresh();
}

void
GuiRosterView::select (const QModelIndex &index)
{
	setCurrentIndex (index);
}

void
GuiRosterView::currentChanged (const QModelIndex &current, const QModelIndex &/*previous*/)
{
	emit selected (this, current);
}

void
GuiRosterView::refresh (void)
{
	QModelIndex	idx;

	DBG_TRACE_FENTER;

	if (!d.ishdr) goto leave;

	int p = verticalScrollBar()->value ();

	emit wantscroll (p);

leave:
	DBG_TRACE_FLEAVE;
}
