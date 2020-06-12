/** \file
 * The SPWaW war cabinet - GUI - unit history view.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_history_view.h"
#include "gui_history.h"
#include "model/model_history.h"
#include "model/model_history_data.h"

#define	BASE_SIZE	40
#define	NUMBER_SIZE	(BASE_SIZE * 5 / 3)
#define	ICON_SIZE	16

// TODO: rename NUMBER_SIZE to something more appropriate

typedef bool GHV_VISIBILITY[GHV_MODE_LIMIT];

static GHV_VISIBILITY	ghvmode[MDLH_COLUMN_CNT] = {
	{ true,  true  },	/* MDLH_COLUMN_DATE	*/
	{ true,  false },	/* MDLH_COLUMN_UID	*/
	{ true,  false },	/* MDLH_COLUMN_UNIT	*/
	{ true,  false },	/* MDLH_COLUMN_RNK	*/
	{ true,  false },	/* MDLH_COLUMN_LDR	*/
	{ false, true  },	/* MDLH_COLUMN_STATUS	*/
	{ true,  true  },	/* MDLH_COLUMN_KILL	*/
	{ true,  false },	/* MDLH_COLUMN_EXP	*/
	{ true,  false },	/* MDLH_COLUMN_MOR	*/
	{ false, true  },	/* MDLH_COLUMN_SUP	*/
	{ true,  false },	/* MDLH_COLUMN_RAL	*/
	{ true,  false },	/* MDLH_COLUMN_INF	*/
	{ true,  false },	/* MDLH_COLUMN_ARM	*/
	{ true,  false },	/* MDLH_COLUMN_ART	*/
	{ true,  false },	/* MDLH_COLUMN_MEN	*/
	{ false, true  },	/* MDLH_COLUMN_RDY	*/
	{ true,  true  },	/* MDLH_COLUMN_KIA	*/
	{ true,  true  },	/* MDLH_COLUMN_DMG	*/
	{ false, true  },	/* MDLH_COLUMN_SEEN	*/
	{ false, true  },	/* MDLH_COLUMN_ABAND	*/
	{ false, true  },	/* MDLH_COLUMN_LOADED	*/
	{ true,  false },	/* MDLH_COLUMN_TYPE	*/
	{ true,  false },	/* MDLH_COLUMN_CLASS	*/
	{ true,  false },	/* MDLH_COLUMN_COST	*/
	{ false, true  },	/* MDLH_COLUMN_SPEED	*/
};

typedef bool IMD_VISIBILITY[INTEL_MODE_CNT];

static IMD_VISIBILITY imdmode[MDLH_COLUMN_CNT] = {
	{ true,  true,  true  },	/* MDLH_COLUMN_DATE	*/
	{ true,  true,  false },	/* MDLH_COLUMN_UID	*/
	{ true,  true,  false },	/* MDLH_COLUMN_UNIT	*/
	{ true,  true,  false },	/* MDLH_COLUMN_RNK	*/
	{ true,  true,  false },	/* MDLH_COLUMN_LDR	*/
	{ true,  true,  false },	/* MDLH_COLUMN_STATUS	*/
	{ true,  false, false },	/* MDLH_COLUMN_KILL	*/
	{ true,  false, false },	/* MDLH_COLUMN_EXP	*/
	{ true,  false, false },	/* MDLH_COLUMN_MOR	*/
	{ true,  false, false },	/* MDLH_COLUMN_SUP	*/
	{ true,  false, false },	/* MDLH_COLUMN_RAL	*/
	{ true,  false, false },	/* MDLH_COLUMN_INF	*/
	{ true,  false, false },	/* MDLH_COLUMN_ARM	*/
	{ true,  false, false },	/* MDLH_COLUMN_ART	*/
	{ true,  false, false },	/* MDLH_COLUMN_MEN	*/
	{ true,  false, false },	/* MDLH_COLUMN_RDY	*/
	{ true,  false, false },	/* MDLH_COLUMN_KIA	*/
	{ true,  false, false },	/* MDLH_COLUMN_DMG	*/
	{ true,  true,  false },	/* MDLH_COLUMN_SEEN	*/
	{ true,  true,  false },	/* MDLH_COLUMN_ABAND	*/
	{ true,  false, false },	/* MDLH_COLUMN_LOADED	*/
	{ true,  true,  false },	/* MDLH_COLUMN_TYPE	*/
	{ true,  true,  false },	/* MDLH_COLUMN_CLASS	*/
	{ true,  true,  false },	/* MDLH_COLUMN_COST	*/
	{ true,  false, false },	/* MDLH_COLUMN_SPEED	*/
};

MDLH_COLUMN HDR_LIMIT = MDLH_COLUMN_DATE;

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
	setIconSize (QSize(ICON_SIZE,ICON_SIZE));
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
	} else {
		setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
		horizontalHeader()->setStretchLastSection (true);
	}
	setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOn);

	build_rlayout();
	build_mlayout();

	d.ghvm = GHV_MODE_DOSSIER;
	apply_mode();

	d.mflag = false;
	apply_layout();

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
GuiHistoryView::reload (GHV_MODE ghvm, bool mflag, bool pflag, INTEL_MODE mode)
{
	bool	apply = false;

	if (d.ghvm != ghvm) {
		d.ghvm = ghvm;
		apply = true;
	}
	if (d.mflag != mflag) {
		d.mflag = mflag;
		apply = true;
	}
	if (d.pflag != pflag) {
		d.pflag = pflag;
		apply = true;
	}
	if (d.intel_mode != mode) {
		d.intel_mode = mode;
		apply = true;
	}

	if (apply) apply_mode();

	refresh();
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
GuiHistoryView::intel_mode_set (INTEL_MODE mode)
{
	d.intel_mode = mode;
	if (!d.pflag && d.intel_mode != INTEL_MODE_FULL) {
		sortByColumn (0, Qt::AscendingOrder);
		setSortingEnabled (false);
	} else {
		setSortingEnabled (true);
	}
	apply_mode();
}

void
GuiHistoryView::apply_ghvmode (void)
{
	if (d.ishdr) {
		for (int i=0; i<=HDR_LIMIT; i++) {
			setColumnHidden (i, !ghvmode[i][d.ghvm]);
		}
		for (int i=(HDR_LIMIT+1); i<MDLH_COLUMN_CNT; i++) {
			setColumnHidden (i, true);
		}
	} else {
		for (int i=0; i<=HDR_LIMIT; i++) {
			setColumnHidden (i, true);
		}
		for (int i=(HDR_LIMIT+1); i<MDLH_COLUMN_CNT; i++) {
			setColumnHidden (i, !ghvmode[i][d.ghvm]);
		}
	}
}

void
GuiHistoryView::apply_imdmode (void)
{
	if (d.pflag) return;

	if (d.ishdr) {
		for (int i=0; i<=HDR_LIMIT; i++) {
			if (!imdmode[i][d.intel_mode]) setColumnHidden (i, true);
		}
	} else {
		for (int i=(HDR_LIMIT+1); i<MDLH_COLUMN_CNT; i++) {
			if (!imdmode[i][d.intel_mode]) setColumnHidden (i, true);
		}
	}
}

void
GuiHistoryView::apply_mode (void)
{
	apply_ghvmode();
	apply_imdmode();
	apply_layout();
}

void
GuiHistoryView::build_rlayout (void)
{
	d.rlayout[MDLH_COLUMN_DATE]	= (BASE_SIZE * 3);
	d.rlayout[MDLH_COLUMN_UID]	= BASE_SIZE;
	d.rlayout[MDLH_COLUMN_UNIT]	= (BASE_SIZE * 3);
	d.rlayout[MDLH_COLUMN_RNK]	= (BASE_SIZE * 2);
	d.rlayout[MDLH_COLUMN_LDR]	= (BASE_SIZE * 3);
	d.rlayout[MDLH_COLUMN_STATUS]	= (BASE_SIZE * 3);
	d.rlayout[MDLH_COLUMN_KILL]	= NUMBER_SIZE;
	d.rlayout[MDLH_COLUMN_EXP]	= NUMBER_SIZE;
	d.rlayout[MDLH_COLUMN_MOR]	= NUMBER_SIZE;
	d.rlayout[MDLH_COLUMN_SUP]	= NUMBER_SIZE;
	d.rlayout[MDLH_COLUMN_RAL]	= NUMBER_SIZE;
	d.rlayout[MDLH_COLUMN_INF]	= NUMBER_SIZE;
	d.rlayout[MDLH_COLUMN_ARM]	= NUMBER_SIZE;
	d.rlayout[MDLH_COLUMN_ART]	= NUMBER_SIZE;
	d.rlayout[MDLH_COLUMN_MEN]	= NUMBER_SIZE;
	d.rlayout[MDLH_COLUMN_RDY]	= (BASE_SIZE * 3);
	d.rlayout[MDLH_COLUMN_KIA]	= NUMBER_SIZE;
	d.rlayout[MDLH_COLUMN_DMG]	= NUMBER_SIZE;
	d.rlayout[MDLH_COLUMN_SEEN]	= (BASE_SIZE * 2);
	d.rlayout[MDLH_COLUMN_ABAND]	= (BASE_SIZE * 2);
	d.rlayout[MDLH_COLUMN_LOADED]	= (BASE_SIZE * 2);
	d.rlayout[MDLH_COLUMN_TYPE]	= (BASE_SIZE * 4);
	d.rlayout[MDLH_COLUMN_CLASS]	= (BASE_SIZE * 3);
	d.rlayout[MDLH_COLUMN_COST]	= NUMBER_SIZE;
	d.rlayout[MDLH_COLUMN_SPEED]	= NUMBER_SIZE;
}

void
GuiHistoryView::build_mlayout (void)
{
	d.mlayout[MDLH_COLUMN_DATE]	= (BASE_SIZE * 3);
	d.mlayout[MDLH_COLUMN_UID]	= ((BASE_SIZE * 1) + ICON_SIZE);
	d.mlayout[MDLH_COLUMN_UNIT]	= ((BASE_SIZE * 3) + ICON_SIZE);
	d.mlayout[MDLH_COLUMN_RNK]	= ((BASE_SIZE * 2) + ICON_SIZE);
	d.mlayout[MDLH_COLUMN_LDR]	= ((BASE_SIZE * 3) + ICON_SIZE);
	d.mlayout[MDLH_COLUMN_STATUS]	= ((BASE_SIZE * 3) + ICON_SIZE);
	d.mlayout[MDLH_COLUMN_KILL]	= NUMBER_SIZE;
	d.mlayout[MDLH_COLUMN_EXP]	= NUMBER_SIZE;
	d.mlayout[MDLH_COLUMN_MOR]	= NUMBER_SIZE;
	d.mlayout[MDLH_COLUMN_SUP]	= NUMBER_SIZE;
	d.mlayout[MDLH_COLUMN_RAL]	= NUMBER_SIZE;
	d.mlayout[MDLH_COLUMN_INF]	= NUMBER_SIZE;
	d.mlayout[MDLH_COLUMN_ARM]	= NUMBER_SIZE;
	d.mlayout[MDLH_COLUMN_ART]	= NUMBER_SIZE;
	d.mlayout[MDLH_COLUMN_MEN]	= NUMBER_SIZE;
	d.mlayout[MDLH_COLUMN_RDY]	= (BASE_SIZE * 3);
	d.mlayout[MDLH_COLUMN_KIA]	= NUMBER_SIZE;
	d.mlayout[MDLH_COLUMN_DMG]	= NUMBER_SIZE;
	d.mlayout[MDLH_COLUMN_SEEN]	= (BASE_SIZE * 2);
	d.mlayout[MDLH_COLUMN_ABAND]	= (BASE_SIZE * 2);
	d.mlayout[MDLH_COLUMN_LOADED]	= (BASE_SIZE * 2);
	d.mlayout[MDLH_COLUMN_TYPE]	= (BASE_SIZE * 4);
	d.mlayout[MDLH_COLUMN_CLASS]	= (BASE_SIZE * 3);
	d.mlayout[MDLH_COLUMN_COST]	= NUMBER_SIZE;
	d.mlayout[MDLH_COLUMN_SPEED]	= NUMBER_SIZE;
}

void
GuiHistoryView::apply_layout (void)
{
	int	*layout = d.mflag ? d.mlayout : d.rlayout;

	if (d.ishdr) {
		int w = 0;
		for (int i=0; i<=HDR_LIMIT; i++) {
			setColumnWidth (i, layout[i]);
			if (!isColumnHidden(i)) w += layout[i];
		}
		w += 5;

		setMinimumWidth (w); setMaximumWidth (w);
	} else {
		for (int i=(HDR_LIMIT+1); i<MDLH_COLUMN_CNT; i++) {
			setColumnWidth (i, layout[i]);
		}
	}

}
