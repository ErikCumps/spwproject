/** \file
 * The SPWaW war cabinet - GUI - unit list view.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_unitlist_view.h"
#include "gui_history.h"
#include "model/model_unitlist.h"
#include "model/model_unitlist_data.h"

#define	BASE_SIZE	40
#define	ICON_SIZE	16
#define	VSB_WIDTH	16
#define	SPACING		2

GuiUnitlistView::GuiUnitlistView (GuiHistory *history, QWidget *P)
	: QListView (P)
{
	QPalette	pal;

	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	if (!history) SET_GUICLS_ERROR (ERR_GUI_REPORTS_UNITLIST_INIT_FAILED, "no parent history widget provided");

	d.parent = history;

	setModel (d.parent->d.lmodel);

	GUINEW (d.font, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_REPORTS_UNITLIST_INIT_FAILED, "font");
	setFont (*d.font);

	setViewMode (QListView::ListMode);
	setFlow (QListView::TopToBottom);
	setWrapping (false);
	setModelColumn (MDLU_COLUMN_ID);
	setMovement (QListView::Static);
	setResizeMode (QListView::Fixed);

	setAlternatingRowColors (false);
	setDragEnabled (false);
	setIconSize (QSize(ICON_SIZE,ICON_SIZE));
	setSelectionBehavior (QAbstractItemView::SelectItems);
	setSelectionMode (QAbstractItemView::SingleSelection);
	//setTabKeyNavigation (true);
	setTabKeyNavigation (false);
	setTextElideMode (Qt::ElideRight);
	setHorizontalScrollMode (QAbstractItemView::ScrollPerPixel);
	setVerticalScrollMode (QAbstractItemView::ScrollPerItem);
	setSpacing (SPACING);

	/* Fix highlight palette for inactive items */
	pal = palette();
	pal.setBrush (QPalette::Inactive, QPalette::Highlight, pal.brush (QPalette::Active, QPalette::Highlight));
	pal.setBrush (QPalette::Inactive, QPalette::HighlightedText, pal.brush (QPalette::Active, QPalette::HighlightedText));
	setPalette (pal);

	setMinimumWidth (BASE_SIZE + 5);
	setMaximumWidth (BASE_SIZE + 5);

	setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
	setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);

	if (!connect (this, SIGNAL(activated(const QModelIndex&)), SLOT(select(const QModelIndex&))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_UNITLIST_INIT_FAILED, "failed to connect <activated> to <select>");

	if (!connect (this, SIGNAL(pressed(const QModelIndex&)), SLOT(select(const QModelIndex&))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_UNITLIST_INIT_FAILED, "failed to connect <pressed> to <select>");

	if (!connect (this, SIGNAL(entered(const QModelIndex&)), SLOT(select(const QModelIndex&))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_UNITLIST_INIT_FAILED, "failed to connect <entered> to <select>");

	if (!connect (this, SIGNAL(clicked(const QModelIndex&)), SLOT(select(const QModelIndex&))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_UNITLIST_INIT_FAILED, "failed to connect <clicked> to <select>");

	SET_GUICLS_NOERR;
}

GuiUnitlistView::~GuiUnitlistView (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
}

void
GuiUnitlistView::reload (void)
{
	if (!d.parent->d.pdata) return;

	refresh();
}

void
GuiUnitlistView::select (const QModelIndex &index)
{
	setCurrentIndex (index);
	scrollTo (index);
	emit selected (d.parent->d.lmodel->data(index, MDLU_UIDX_ROLE).toInt());
}

void
GuiUnitlistView::currentChanged (const QModelIndex &current, const QModelIndex &/*previous*/)
{
	select (current);
}

void
GuiUnitlistView::refresh (void)
{
	int		mmw, fmw, w;
	QFontMetrics	fm(*d.font);

	//DBG_TRACE_FENTER;
	mmw = d.parent->d.lmodel->max_width() + 1;
	fmw = fm.maxWidth() + 1;
	w = (mmw * fmw) + VSB_WIDTH + 6;
	if (d.parent->d.lmodel->dossier_mode()) w += ICON_SIZE;

	DBG_log ("GuiUnitlistView: mmw=%d, fmw=%d, VSB=%d, dossier=%d, w=%d\n", mmw, fmw, VSB_WIDTH, d.parent->d.lmodel->dossier_mode(), w);

	setMinimumWidth (w); setMaximumWidth (w);
	//DBG_TRACE_FLEAVE;
}
