/** \file
 * The SPWaW war cabinet - GUI - unit history view.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_history_view.h"
#include "gui_history.h"
#include "model/model_history.h"
#include "model/model_history_data.h"

#define	BASE_SIZE	40
#define	NUMBER_SIZE	(BASE_SIZE * 6 / 4)

// TODO: rename NUMBER_SIZE to something more appropriate

GuiHistoryView::GuiHistoryView (bool hdr, GuiHistory *history, QWidget *P)
	: QTableView (P)
{
	QPalette	pal;

	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	if (!history) SET_GUICLS_ERROR (ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "no parent history widget provided");

	d.ishdr = hdr; d.parent = history;

	setModel (d.parent->d.hmodel);

	GUINEW (d.font, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "font");
	setFont (*d.font);

	setCornerButtonEnabled (false);
	setSortingEnabled (false);
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
	horizontalHeader()->setSortIndicatorShown (false);
	horizontalHeader()->setResizeMode (QHeaderView::Fixed);

	if (d.ishdr) {
		setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
		horizontalHeader()->setStretchLastSection (true);

		for (int i=0; i<MDLH_COLUMN_CNT; i++) setColumnHidden (i, true);
		setColumnHidden (MDLH_COLUMN_DATE, false);

		setColumnWidth (MDLH_COLUMN_DATE,	BASE_SIZE * 3);

		setMinimumWidth (BASE_SIZE * 3 + 5);
		setMaximumWidth (BASE_SIZE * 3 + 5);
	} else {
		setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
		horizontalHeader()->setStretchLastSection (true);

		for (int i=0; i<MDLH_COLUMN_CNT; i++) setColumnHidden (i, false);
		setColumnHidden (MDLH_COLUMN_DATE, true);
		setColumnHidden (MDLH_COLUMN_LDR,  true);

		setColumnWidth (MDLH_COLUMN_CFLAG,	BASE_SIZE / 2);
		setColumnWidth (MDLH_COLUMN_UID,	BASE_SIZE);
		setColumnWidth (MDLH_COLUMN_UNIT,	BASE_SIZE * 3);
		setColumnWidth (MDLH_COLUMN_RNK,	NUMBER_SIZE);
		setColumnWidth (MDLH_COLUMN_LDR,	BASE_SIZE * 3);
		setColumnWidth (MDLH_COLUMN_KILL,	NUMBER_SIZE);
		setColumnWidth (MDLH_COLUMN_EXP,	NUMBER_SIZE);
		setColumnWidth (MDLH_COLUMN_MOR,	NUMBER_SIZE);
		setColumnWidth (MDLH_COLUMN_RAL,	NUMBER_SIZE);
		setColumnWidth (MDLH_COLUMN_INF,	NUMBER_SIZE);
		setColumnWidth (MDLH_COLUMN_ARM,	NUMBER_SIZE);
		setColumnWidth (MDLH_COLUMN_ART,	NUMBER_SIZE);
		setColumnWidth (MDLH_COLUMN_TYPE,	BASE_SIZE * 4);
		setColumnWidth (MDLH_COLUMN_CLASS,	BASE_SIZE * 3);
		setColumnWidth (MDLH_COLUMN_RDY,	BASE_SIZE * 3);
		setColumnWidth (MDLH_COLUMN_SUP,	NUMBER_SIZE);
		setColumnWidth (MDLH_COLUMN_STATUS,	BASE_SIZE * 3);
		setColumnWidth (MDLH_COLUMN_SEEN,	BASE_SIZE * 2);
		setColumnWidth (MDLH_COLUMN_ABAND,	BASE_SIZE * 3 / 2);
		setColumnWidth (MDLH_COLUMN_LOADED,	BASE_SIZE * 2);
		setColumnWidth (MDLH_COLUMN_DMG,	NUMBER_SIZE);
		setColumnWidth (MDLH_COLUMN_COST,	NUMBER_SIZE);
		setColumnWidth (MDLH_COLUMN_SPEED,	NUMBER_SIZE);
	}
	setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOn);

	horizontalHeader()->setResizeMode (MDLH_COLUMN_LDR, QHeaderView::Interactive);
	horizontalHeader()->setResizeMode (MDLH_COLUMN_TYPE, QHeaderView::Interactive);
	horizontalHeader()->setResizeMode (MDLH_COLUMN_CLASS, QHeaderView::Interactive);
	horizontalHeader()->setResizeMode (MDLH_COLUMN_STATUS, QHeaderView::Interactive);

	//setContextMenuPolicy (Qt::DefaultContextMenu);

	SET_GUICLS_NOERR;
}

GuiHistoryView::~GuiHistoryView (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
}

void
GuiHistoryView::reload (void)
{
	if (!d.parent->d.pdata) return;

	refresh();
}

void
GuiHistoryView::wheelEvent (QWheelEvent *e)
{
	if (!e) return;

	if (d.ishdr) {
		emit wheeled (e);
	} else {
		QAbstractScrollArea::wheelEvent (e);
	}
}

void
GuiHistoryView::scroll (int d)
{
	this->verticalScrollBar()->setValue (d);
}

void
GuiHistoryView::wheel (QWheelEvent *e)
{
	wheelEvent (e);
}

void
GuiHistoryView::select (const QModelIndex &index)
{
	setCurrentIndex (index);
}

void
GuiHistoryView::currentChanged (const QModelIndex &current, const QModelIndex &/*previous*/)
{
	emit selected (this, current);
}

void
GuiHistoryView::battleview (bool set)
{
	if (d.ishdr) return;

	setColumnHidden (MDLH_COLUMN_CFLAG,	set);
	setColumnHidden (MDLH_COLUMN_UID,	set);
	setColumnHidden (MDLH_COLUMN_RNK,	set);
	setColumnHidden (MDLH_COLUMN_LDR,	set);
	setColumnHidden (MDLH_COLUMN_EXP,	set);
	setColumnHidden (MDLH_COLUMN_MOR,	set);
	setColumnHidden (MDLH_COLUMN_RAL,	set);
	setColumnHidden (MDLH_COLUMN_INF,	set);
	setColumnHidden (MDLH_COLUMN_ARM,	set);
	setColumnHidden (MDLH_COLUMN_ART,	set);
	setColumnHidden (MDLH_COLUMN_TYPE,	set);
	setColumnHidden (MDLH_COLUMN_CLASS,	set);
	setColumnHidden (MDLH_COLUMN_COST,	set);
	setColumnHidden (MDLH_COLUMN_SPEED,	set);

	setColumnHidden (MDLH_COLUMN_RDY,	!set);
	setColumnHidden (MDLH_COLUMN_SUP,	!set);
	setColumnHidden (MDLH_COLUMN_STATUS,	!set);
	setColumnHidden (MDLH_COLUMN_SEEN,	!set);
	setColumnHidden (MDLH_COLUMN_ABAND,	!set);
	setColumnHidden (MDLH_COLUMN_LOADED,	!set);
	setColumnHidden (MDLH_COLUMN_DMG,	!set);
}

void
GuiHistoryView::refresh (void)
{
	QModelIndex	idx;

	DBG_TRACE_FENTER;

	if (!d.ishdr) goto leave;

	int p = verticalScrollBar()->value ();

	emit wantscroll (p);

leave:
	DBG_TRACE_FLEAVE;
}
