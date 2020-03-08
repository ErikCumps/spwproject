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
#define	ICON_SIZE	16

// TODO: rename NUMBER_SIZE to something more appropriate

typedef bool GRV_VISIBILITY[GRV_MODE_LIMIT];

GRV_VISIBILITY	grvmode[MDLR_COLUMN_CNT] = {
	{ true,  true, false },	/* MDLR_COLUMN_CDSTATUS	*/
	{ true,  true, true  },	/* MDLR_COLUMN_UID	*/
	{ true,  true, true  },	/* MDLR_COLUMN_UNIT	*/
	{ true,  true, true  },	/* MDLR_COLUMN_RNK	*/
	{ true,  true, true  },	/* MDLR_COLUMN_LDR	*/
	{ false, true, true  },	/* MDLR_COLUMN_STATUS	*/
	{ true,  true, true  },	/* MDLR_COLUMN_KILL	*/
	{ true,  true, false },	/* MDLR_COLUMN_EXP	*/
	{ true,  true, false },	/* MDLR_COLUMN_MOR	*/
	{ true,  true, true  },	/* MDLR_COLUMN_SUP	*/
	{ true,  true, false },	/* MDLR_COLUMN_RAL	*/
	{ true,  true, false },	/* MDLR_COLUMN_INF	*/
	{ true,  true, false },	/* MDLR_COLUMN_ARM	*/
	{ true,  true, false },	/* MDLR_COLUMN_ART	*/
	{ true,  true, true  },	/* MDLR_COLUMN_MEN	*/
	{ false, true, true  },	/* MDLR_COLUMN_RDY	*/
	{ false, true, true  },	/* MDLR_COLUMN_KIA	*/
	{ false, true, true  },	/* MDLR_COLUMN_DMG	*/
	{ false, true, true  },	/* MDLR_COLUMN_SEEN	*/
	{ false, true, true  },	/* MDLR_COLUMN_ABAND	*/
	{ false, true, true  },	/* MDLR_COLUMN_LOADED	*/
	{ true,  true, false },	/* MDLR_COLUMN_TYPE	*/
	{ true,  true, false },	/* MDLR_COLUMN_CLASS	*/
	{ true,  true, false },	/* MDLR_COLUMN_COST	*/
	{ true,  true, false },	/* MDLR_COLUMN_SPEED	*/
};

MDLR_COLUMN HDR_LIMIT = MDLR_COLUMN_UNIT;

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
	horizontalHeader()->setSortIndicatorShown (true);
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

	d.grvm = GRV_MODE_DOSSIER;
	apply_grvmode();

	d.mflag = false;
	apply_layout (true);

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
GuiRosterView::reload (GRV_MODE grvm, bool sort, bool mflag)
{
	bool	apply = false;

	if (d.grvm != grvm) {
		d.grvm = grvm;
		apply = true;
	}
	if (d.mflag != mflag) {
		d.mflag = mflag;
		apply = true;
	}
	if (apply) {
		apply_grvmode ();
		apply_layout (false);
	}
	if (d.sidx < 0) { d.sidx = MDLR_COLUMN_UID; d.sord = Qt::AscendingOrder; }
	if (sort || d.parent->d.Vautosort) sortByColumn (d.sidx, d.sord);
	refresh();
}

void
GuiRosterView::resort (void)
{
	if (d.sidx != -1) sortByColumn (d.sidx, d.sord);
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
GuiRosterView::apply_grvmode (void)
{
	grvmode[MDLR_COLUMN_CDSTATUS][GRV_MODE_BATTLE] = d.mflag;

	if (d.ishdr) {
		for (int i=0; i<=HDR_LIMIT; i++) {
			setColumnHidden (i, !grvmode[i][d.grvm]);
		}
		for (int i=(HDR_LIMIT+1); i<MDLR_COLUMN_CNT; i++) {
			setColumnHidden (i, true);
		}
	} else {
		for (int i=0; i<=HDR_LIMIT; i++) {
			setColumnHidden (i, true);
		}
		for (int i=(HDR_LIMIT+1); i<MDLR_COLUMN_CNT; i++) {
			setColumnHidden (i, !grvmode[i][d.grvm]);
		}
	}
}

void
GuiRosterView::build_rlayout (void)
{
	d.rlayout[MDLR_COLUMN_CDSTATUS]	= ICON_SIZE + 7;
	d.rlayout[MDLR_COLUMN_UID]	= BASE_SIZE;
	d.rlayout[MDLR_COLUMN_UNIT]	= (BASE_SIZE * 3);
	d.rlayout[MDLR_COLUMN_RNK]	= (BASE_SIZE * 2);
	d.rlayout[MDLR_COLUMN_LDR]	= (BASE_SIZE * 3);
	d.rlayout[MDLR_COLUMN_STATUS]	= (BASE_SIZE * 3);
	d.rlayout[MDLR_COLUMN_KILL]	= NUMBER_SIZE;
	d.rlayout[MDLR_COLUMN_EXP]	= NUMBER_SIZE;
	d.rlayout[MDLR_COLUMN_MOR]	= NUMBER_SIZE;
	d.rlayout[MDLR_COLUMN_SUP]	= NUMBER_SIZE;
	d.rlayout[MDLR_COLUMN_RAL]	= NUMBER_SIZE;
	d.rlayout[MDLR_COLUMN_INF]	= NUMBER_SIZE;
	d.rlayout[MDLR_COLUMN_ARM]	= NUMBER_SIZE;
	d.rlayout[MDLR_COLUMN_ART]	= NUMBER_SIZE;
	d.rlayout[MDLR_COLUMN_MEN]	= NUMBER_SIZE;
	d.rlayout[MDLR_COLUMN_RDY]	= (BASE_SIZE * 3);
	d.rlayout[MDLR_COLUMN_KIA]	= NUMBER_SIZE;
	d.rlayout[MDLR_COLUMN_DMG]	= NUMBER_SIZE;
	d.rlayout[MDLR_COLUMN_SEEN]	= (BASE_SIZE * 2);
	d.rlayout[MDLR_COLUMN_ABAND]	= (BASE_SIZE * 2);
	d.rlayout[MDLR_COLUMN_LOADED]	= (BASE_SIZE * 2);
	d.rlayout[MDLR_COLUMN_TYPE]	= (BASE_SIZE * 4);
	d.rlayout[MDLR_COLUMN_CLASS]	= (BASE_SIZE * 3);
	d.rlayout[MDLR_COLUMN_COST]	= NUMBER_SIZE;
	d.rlayout[MDLR_COLUMN_SPEED]	= NUMBER_SIZE;
}

void
GuiRosterView::build_mlayout (void)
{
	d.mlayout[MDLR_COLUMN_CDSTATUS]	= ICON_SIZE + 7;
	d.mlayout[MDLR_COLUMN_UID]	= ((BASE_SIZE * 1) + ICON_SIZE);
	d.mlayout[MDLR_COLUMN_UNIT]	= ((BASE_SIZE * 3) + ICON_SIZE);
	d.mlayout[MDLR_COLUMN_RNK]	= ((BASE_SIZE * 2) + ICON_SIZE);
	d.mlayout[MDLR_COLUMN_LDR]	= ((BASE_SIZE * 3) + ICON_SIZE);
	d.mlayout[MDLR_COLUMN_STATUS]	= ((BASE_SIZE * 3) + ICON_SIZE);
	d.mlayout[MDLR_COLUMN_KILL]	= NUMBER_SIZE;
	d.mlayout[MDLR_COLUMN_EXP]	= NUMBER_SIZE;
	d.mlayout[MDLR_COLUMN_MOR]	= NUMBER_SIZE;
	d.mlayout[MDLR_COLUMN_SUP]	= NUMBER_SIZE;
	d.mlayout[MDLR_COLUMN_RAL]	= NUMBER_SIZE;
	d.mlayout[MDLR_COLUMN_INF]	= NUMBER_SIZE;
	d.mlayout[MDLR_COLUMN_ARM]	= NUMBER_SIZE;
	d.mlayout[MDLR_COLUMN_ART]	= NUMBER_SIZE;
	d.mlayout[MDLR_COLUMN_MEN]	= NUMBER_SIZE;
	d.mlayout[MDLR_COLUMN_RDY]	= (BASE_SIZE * 3);
	d.mlayout[MDLR_COLUMN_KIA]	= NUMBER_SIZE;
	d.mlayout[MDLR_COLUMN_DMG]	= NUMBER_SIZE;
	d.mlayout[MDLR_COLUMN_SEEN]	= (BASE_SIZE * 2);
	d.mlayout[MDLR_COLUMN_ABAND]	= (BASE_SIZE * 2);
	d.mlayout[MDLR_COLUMN_LOADED]	= (BASE_SIZE * 2);
	d.mlayout[MDLR_COLUMN_TYPE]	= (BASE_SIZE * 4);
	d.mlayout[MDLR_COLUMN_CLASS]	= (BASE_SIZE * 3);
	d.mlayout[MDLR_COLUMN_COST]	= NUMBER_SIZE;
	d.mlayout[MDLR_COLUMN_SPEED]	= NUMBER_SIZE;
}

void
GuiRosterView::apply_layout (bool reset)
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
		for (int i=(HDR_LIMIT+1); i<MDLR_COLUMN_CNT; i++) {
			if (reset || (columnWidth (i) < layout[i])) setColumnWidth (i, layout[i]);
		}
	}
}
