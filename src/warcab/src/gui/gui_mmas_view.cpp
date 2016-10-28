/** \file
 * The SPWaW war cabinet - GUI - min-max-average-spread view.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_mmas_view.h"
#include "gui_mmas.h"
#include "model/model_mmas.h"
#include "model/model_mmas_data.h"

#define	BASE_SIZE	40

GuiMMASView::GuiMMASView (bool hdr, GuiMMAS *mmas, QWidget *P)
	: QTableView (P)
{
	QPalette	pal;

	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	if (!mmas) SET_GUICLS_ERROR (ERR_GUI_VIEW_INIT_FAILED, "no parent mmas widget provided");

	d.ishdr = hdr; d.parent = mmas;

	setModel (d.parent->d.model);

	GUINEW (d.font, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_VIEW_INIT_FAILED, "font");
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
	horizontalHeader()->setDefaultSectionSize (BASE_SIZE * 5 / 4);
	horizontalHeader()->setHighlightSections (false);
	horizontalHeader()->setSortIndicatorShown (false);
	horizontalHeader()->setResizeMode (QHeaderView::Fixed);

	setColumnWidth (MDLMMAS_COLUMN_DATE, BASE_SIZE * 3);
	setColumnWidth (MDLMMAS_COLUMN_MAX,  BASE_SIZE * 4);
	setColumnWidth (MDLMMAS_COLUMN_MIN,  BASE_SIZE * 4);
	setColumnWidth (MDLMMAS_COLUMN_AVG,  BASE_SIZE * 4);
	setColumnWidth (MDLMMAS_COLUMN_SPR,  BASE_SIZE * 4);
	setColumnWidth (MDLMMAS_COLUMN_COUNT,  BASE_SIZE * 4);

	if (d.ishdr) {
		setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
		//horizontalHeader()->setStretchLastSection (false);
		horizontalHeader()->setStretchLastSection (true);

		for (int i=0; i<MDLMMAS_COLUMN_CNT; i++) setColumnHidden (i, true);
		setColumnHidden (MDLMMAS_COLUMN_DATE, false);

		setMinimumWidth (BASE_SIZE * 3 + 5);
		setMaximumWidth (BASE_SIZE * 3 + 5);
	} else {
		setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
		horizontalHeader()->setStretchLastSection (true);

		for (int i=0; i<MDLMMAS_COLUMN_CNT; i++) setColumnHidden (i, false);
		setColumnHidden (MDLMMAS_COLUMN_DATE, true);
	}
	setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOn);
	setColumnHidden (MDLMMAS_COLUMN_MAXID, true);
	setColumnHidden (MDLMMAS_COLUMN_MINID, true);

	//setContextMenuPolicy (Qt::DefaultContextMenu);

	SET_GUICLS_NOERR;
}

GuiMMASView::~GuiMMASView (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
}

void
GuiMMASView::reload (void)
{
	if (!d.parent->d.pdata) return;

	refresh();
}

void
GuiMMASView::wheelEvent (QWheelEvent *e)
{
	if (!e) return;

	if (d.ishdr) {
		emit wheeled (e);
	} else {
		QAbstractScrollArea::wheelEvent (e);
	}
}

void
GuiMMASView::scroll (int d)
{
	this->verticalScrollBar()->setValue (d);
}

void
GuiMMASView::wheel (QWheelEvent *e)
{
	wheelEvent (e);
}

void
GuiMMASView::select (const QModelIndex &index)
{
	setCurrentIndex (index);
}

void
GuiMMASView::currentChanged (const QModelIndex &current, const QModelIndex &/*previous*/)
{
	emit selected (this, current);
}

void
GuiMMASView::refresh (void)
{
	QModelIndex	idx;

	DBG_TRACE_FENTER;

	if (!d.ishdr) goto leave;

	int p = verticalScrollBar()->value ();

	emit wantscroll (p);

leave:
	DBG_TRACE_FLEAVE;
}
