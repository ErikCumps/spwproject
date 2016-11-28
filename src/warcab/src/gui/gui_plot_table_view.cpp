/** \file
 * The SPWaW war cabinet - GUI - plot table view.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_plot_table_view.h"

#define	BASE_SIZE	40

GuiPlotTableView::GuiPlotTableView (bool hdr, ModelPlotTable *model, QWidget *P)
	: QTableView (P)
{
	QPalette	pal;

	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	if (!model) SET_GUICLS_ERROR (ERR_GUI_VIEW_INIT_FAILED, "no model provided");

	d.hdr = hdr; d.model = model;
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

GuiPlotTableView::~GuiPlotTableView (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
}

void
GuiPlotTableView::reload (void)
{
	refresh();
}

void
GuiPlotTableView::wheelEvent (QWheelEvent *e)
{
	if (!e) return;

	if (d.hdr) {
		emit wheeled (e);
	} else {
		QAbstractScrollArea::wheelEvent (e);
	}
}

void
GuiPlotTableView::scroll (int d)
{
	verticalScrollBar()->setValue (d);
}

void
GuiPlotTableView::wheel (QWheelEvent *e)
{
	wheelEvent (e);
}

void
GuiPlotTableView::select (const QModelIndex &index)
{
	setCurrentIndex (index);
}

void
GuiPlotTableView::currentChanged (const QModelIndex &current, const QModelIndex &/*previous*/)
{
	emit selected (this, current);
}

void
GuiPlotTableView::setup (void)
{
	MDLPT_COLDEF	*p = NULL;

	if (d.hdr) {
		int	w = 0;

		setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);

		for (int i=1; i<d.model->columnCount(); i++) setColumnHidden (i, true);
		setColumnHidden (0, false);

		if (p = d.model->col_info (0)) w = p->width;
		setColumnWidth (0, w);

		setMinimumWidth (w + 5);
		setMaximumWidth (w + 5);
	} else {
		setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);

		for (int i=1; i<d.model->columnCount(); i++) {
			setColumnHidden (i, false);
			if (p = d.model->col_info (i)) setColumnWidth (i, p->width);
		}
		setColumnHidden (0, true);
	}
}

void
GuiPlotTableView::refresh (void)
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
GuiPlotTableView::commit (void)
{
	setup();
	refresh();
}

void
GuiPlotTableView::commit_def (void)
{
	setup();
}

void
GuiPlotTableView::commit_dat (void)
{
	refresh();
}
