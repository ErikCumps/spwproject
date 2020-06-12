/** \file
 * The SPWaW war cabinet - GUI - order of battle view.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_oob_view.h"
#include "gui_oob.h"
#include "model/model_oob.h"
#include "model/model_sanity.h"

#define	BASE_SIZE	40
#define	NUMBER_SIZE	(BASE_SIZE * 3 / 2)

// TODO: rename NUMBER_SIZE to something more appropriate

typedef bool GOV_VISIBILITY[GOV_MODE_LIMIT];

static GOV_VISIBILITY	govmode[MDLO_COLUMN_CNT] = {
	{ true,  true,  true  },	/* MDLO_COLUMN_FID	*/
	{ true,  true,  true  },	/* MDLO_COLUMN_TYPE	*/
	{ true,  true,  true  },	/* MDLO_COLUMN_LDR	*/
	{ true,  true,  true  },	/* MDLO_COLUMN_HCMD	*/
	{ true,  true,  true  },	/* MDLO_COLUMN_STATUS	*/
	{ true,  true,  true  },	/* MDLO_COLUMN_KILLS	*/
	{ true,  false, false },	/* MDLO_COLUMN_RDY	*/
	{ false, false, false },	/* MDLO_COLUMN_COUNT	*/
	{ true,  true,  true  },	/* MDLO_COLUMN_EXP	*/
	{ true,  true,  true  },	/* MDLO_COLUMN_MOR	*/
	{ true,  true,  true  },	/* MDLO_COLUMN_SUP	*/
	{ true,  true,  true  },	/* MDLO_COLUMN_RAL	*/
	{ true,  true,  true  },	/* MDLO_COLUMN_INF	*/
	{ true,  true,  true  },	/* MDLO_COLUMN_ARM	*/
	{ true,  true,  true  },	/* MDLO_COLUMN_ART	*/
};

typedef bool IMD_VISIBILITY[INTEL_MODE_CNT];

static IMD_VISIBILITY imdmode[MDLO_COLUMN_CNT] = {
	{ true,  true,  true  },	/* MDLO_COLUMN_FID	*/
	{ true,  true,  true  },	/* MDLO_COLUMN_TYPE	*/
	{ true,  true,  true  },	/* MDLO_COLUMN_LDR	*/
	{ true,  true,  true  },	/* MDLO_COLUMN_HCMD	*/
	{ true,  false, false },	/* MDLO_COLUMN_STATUS	*/
	{ true,  false, false },	/* MDLO_COLUMN_KILLS	*/
	{ true,  false, false },	/* MDLO_COLUMN_RDY	*/
	{ false, false, false },	/* MDLO_COLUMN_COUNT	*/
	{ true,  false, false },	/* MDLO_COLUMN_EXP	*/
	{ true,  false, false },	/* MDLO_COLUMN_MOR	*/
	{ true,  false, false },	/* MDLO_COLUMN_SUP	*/
	{ true,  false, false },	/* MDLO_COLUMN_RAL	*/
	{ true,  false, false },	/* MDLO_COLUMN_INF	*/
	{ true,  false, false },	/* MDLO_COLUMN_ARM	*/
	{ true,  false, false },	/* MDLO_COLUMN_ART	*/
};

MDLO_COLUMN HDR_LIMIT = MDLO_COLUMN_TYPE;

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
	} else {
		setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
		header()->setStretchLastSection (true);
	}
	setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOn);

	build_rlayout();
	build_dlayout();

	d.govm = GOV_MODE_DOSSIER;
	apply_mode();
	apply_layout();

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

	apply_layout();
}

void
GuiOobView::reload (GOV_MODE govm, bool sort, bool pflag, INTEL_MODE mode)
{
	bool	apply = false;

	if (d.govm != govm) {
		d.govm = govm;
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
GuiOobView::intel_mode_set (INTEL_MODE mode)
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

void
GuiOobView::apply_govmode (void)
{
	govmode[MDLO_COLUMN_SUP][GOV_MODE_BATTLE] = d.parent->d.Vprevcmp;

	if (d.ishdr) {
		for (int i=0; i<=HDR_LIMIT; i++) {
			setColumnHidden (i, !govmode[i][d.govm]);
		}
		for (int i=(HDR_LIMIT+1); i<MDLO_COLUMN_CNT; i++) {
			setColumnHidden (i, true);
		}
	} else {
		for (int i=0; i<=HDR_LIMIT; i++) {
			setColumnHidden (i, true);
		}
		for (int i=(HDR_LIMIT+1); i<MDLO_COLUMN_CNT; i++) {
			setColumnHidden (i, !govmode[i][d.govm]);
		}
	}
}

void
GuiOobView::apply_imdmode (void)
{
	if (d.pflag) return;

	if (d.ishdr) {
		for (int i=0; i<=HDR_LIMIT; i++) {
			if (!imdmode[i][d.intel_mode]) setColumnHidden (i, true);
		}
	} else {
		for (int i=(HDR_LIMIT+1); i<MDLO_COLUMN_CNT; i++) {
			if (!imdmode[i][d.intel_mode]) setColumnHidden (i, true);
		}
	}
}

void
GuiOobView::apply_mode (void)
{
	apply_govmode();
	apply_imdmode();
	apply_layout();
}

void
GuiOobView::build_rlayout (void)
{
	d.rlayout[MDLO_COLUMN_FID]	= (BASE_SIZE * 1);
	d.rlayout[MDLO_COLUMN_TYPE]	= (BASE_SIZE * 3);
	d.rlayout[MDLO_COLUMN_LDR]	= (BASE_SIZE * 3);
	d.rlayout[MDLO_COLUMN_HCMD]	= (BASE_SIZE * 4 / 3);
	d.rlayout[MDLO_COLUMN_STATUS]	= (BASE_SIZE * 9 / 2);
	d.rlayout[MDLO_COLUMN_KILLS]	= (BASE_SIZE * 7 / 4);
	d.rlayout[MDLO_COLUMN_RDY]	= (BASE_SIZE * 3);
	d.rlayout[MDLO_COLUMN_COUNT]	= (BASE_SIZE * 3);
	d.rlayout[MDLO_COLUMN_EXP]	= NUMBER_SIZE;
	d.rlayout[MDLO_COLUMN_MOR]	= NUMBER_SIZE;
	d.rlayout[MDLO_COLUMN_SUP]	= NUMBER_SIZE;
	d.rlayout[MDLO_COLUMN_RAL]	= NUMBER_SIZE;
	d.rlayout[MDLO_COLUMN_INF]	= NUMBER_SIZE;
	d.rlayout[MDLO_COLUMN_ARM]	= NUMBER_SIZE;
	d.rlayout[MDLO_COLUMN_ART]	= NUMBER_SIZE;
}

void
GuiOobView::build_dlayout (void)
{
	d.dlayout[MDLO_COLUMN_FID]	= (BASE_SIZE * 1);
	d.dlayout[MDLO_COLUMN_TYPE]	= (BASE_SIZE * 3);
	d.dlayout[MDLO_COLUMN_LDR]	= (BASE_SIZE * 3);
	d.dlayout[MDLO_COLUMN_HCMD]	= (BASE_SIZE * 4 / 3);
	d.dlayout[MDLO_COLUMN_STATUS]	= (BASE_SIZE * 9 / 2);
	d.dlayout[MDLO_COLUMN_KILLS]	= (BASE_SIZE * 7 / 4);
	d.dlayout[MDLO_COLUMN_RDY]	= (BASE_SIZE * 3);
	d.dlayout[MDLO_COLUMN_COUNT]	= (BASE_SIZE * 3);
	d.dlayout[MDLO_COLUMN_EXP]	= (BASE_SIZE * 3);
	d.dlayout[MDLO_COLUMN_MOR]	= (BASE_SIZE * 3);
	d.dlayout[MDLO_COLUMN_SUP]	= (BASE_SIZE * 3);
	d.dlayout[MDLO_COLUMN_RAL]	= (BASE_SIZE * 3);
	d.dlayout[MDLO_COLUMN_INF]	= (BASE_SIZE * 3);
	d.dlayout[MDLO_COLUMN_ARM]	= (BASE_SIZE * 3);
	d.dlayout[MDLO_COLUMN_ART]	= (BASE_SIZE * 3);
}

void
GuiOobView::apply_layout (void)
{
	int	*layout;

	if (d.parent->d.ptype == MDLD_TREE_DOSSIER) {
		layout = d.dlayout;
	} else if ((d.parent->d.ptype == MDLD_TREE_BATTLE) && d.parent->d.Vprevcmp) {
		layout = d.dlayout;
	} else {
		layout = d.rlayout;
	}

	if (d.ishdr) {
		int w = 0;
		for (int i=0; i<=HDR_LIMIT; i++) {
			setColumnWidth (i, layout[i]);
			if (!isColumnHidden(i)) w += layout[i];
		}
		w += 5;

		setMinimumWidth (w); setMaximumWidth (w);
	} else {
		for (int i=(HDR_LIMIT+1); i<MDLO_COLUMN_CNT; i++) {
			setColumnWidth (i, layout[i]);
		}
	}
}