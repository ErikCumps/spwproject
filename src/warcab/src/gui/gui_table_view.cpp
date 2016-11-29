/** \file
 * The SPWaW war cabinet - GUI - table view.
 *
 * Copyright (C) 2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_table_view.h"

#define	BASE_SIZE	40

GuiTableView::GuiTableView (bool hdr, ModelTable *model, int hdrcols, QWidget *P)
	: QTableView (P)
{
	QPalette	pal;

	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	if (!model) SET_GUICLS_ERROR (ERR_GUI_VIEW_INIT_FAILED, "no model provided");

	d.hdr = hdr; d.model = model; d.hdrcols = hdrcols;
	setModel (d.model);

	GUINEW (d.font, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_VIEW_INIT_FAILED, "font");
	setFont (*d.font);

	setCornerButtonEnabled (false);
	setSortingEnabled (false);
	setAlternatingRowColors (false);
	setDragEnabled (false);
	setIconSize (QSize(8,8));
	setSelectionBehavior (QAbstractItemView::SelectRows);
	setSelectionMode (QAbstractItemView::SingleSelection);
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
	//horizontalHeader()->setDefaultSectionSize (BASE_SIZE * 5 / 4);
	horizontalHeader()->setHighlightSections (false);
	horizontalHeader()->setSortIndicatorShown (false);
	horizontalHeader()->setResizeMode (QHeaderView::Fixed);
	horizontalHeader()->setStretchLastSection (true);

	setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOn);

	SET_GUICLS_NOERR;
}

GuiTableView::~GuiTableView (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
}

void
GuiTableView::reload (void)
{
	refresh();
}

void
GuiTableView::wheelEvent (QWheelEvent *e)
{
	if (!e) return;

	if (d.hdr) {
		emit wheeled (e);
	} else {
		QAbstractScrollArea::wheelEvent (e);
	}
}

void
GuiTableView::scroll (int d)
{
	verticalScrollBar()->setValue (d);
}

void
GuiTableView::wheel (QWheelEvent *e)
{
	wheelEvent (e);
}

void
GuiTableView::select (const QModelIndex &index)
{
	setCurrentIndex (index);
}

void
GuiTableView::currentChanged (const QModelIndex &current, const QModelIndex &/*previous*/)
{
	emit selected (this, current);
}

void
GuiTableView::setup (void)
{
	MDLT_COLDEF	*p = NULL;

	if (d.hdr) {
		int	w = 0, totalw = 0;

		setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);

		for (int i=d.hdrcols; i<d.model->columnCount(); i++) setColumnHidden (i, true);

		for (int i=0; i<d.hdrcols; i++) {
			setColumnHidden (i, false);
			if (p = d.model->col_info (i)){
				w = p->width;
				setColumnWidth (i, w);
				totalw += w;
			}
		}

		setMinimumWidth (totalw + 5);
		setMaximumWidth (totalw + 5);
	} else {
		setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);

		for (int i=0; i<d.hdrcols; i++) setColumnHidden (i, true);

		for (int i=d.hdrcols; i<d.model->columnCount(); i++) {
			setColumnHidden (i, false);
			if (p = d.model->col_info (i)) setColumnWidth (i, p->width);
		}
	}
}

void
GuiTableView::refresh (void)
{
	QModelIndex	idx;

	DBG_TRACE_FENTER;

	if (!d.hdr) goto leave;

	int p = verticalScrollBar()->value ();

	emit wantscroll (p);

leave:
	DBG_TRACE_FLEAVE;
}

void
GuiTableView::commit (void)
{
	setup();
	refresh();
}

void
GuiTableView::commit_def (void)
{
	setup();
}

void
GuiTableView::commit_dat (void)
{
	refresh();
}