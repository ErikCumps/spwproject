/** \file
 * The SPWaW war cabinet - GUI - main report widget.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_mainreport.h"
#include "gui_reports_dossier.h"
#include "gui_reports_battle.h"
#include "gui_reports_bturn.h"

GuiMainRpt::GuiMainRpt (QWidget *P)
	: QTabWidget (P)
{
	DBG_TRACE_CONSTRUCT;

	DEVASSERT (P != 0);

	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.dossier = NULL; d.battle = NULL; d.bturn = NULL;

	GUINEW (d.dossier, GuiRptDsr (NULL), ERR_GUI_REPORTS_INIT_FAILED, "dossier reports");
	GUIERR (d.dossier, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.battle, GuiRptBtl (NULL), ERR_GUI_REPORTS_INIT_FAILED, "battle reports");
	GUIERR (d.battle, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.bturn, GuiRptTrn (NULL), ERR_GUI_REPORTS_INIT_FAILED, "battle turn reports");
	GUIERR (d.bturn, ERR_GUI_REPORTS_INIT_FAILED);

	d.dossier_idx = addTab (d.dossier, QIcon (*RES_pixmap (RID_ICON_DOSSIER)), "Dossier");
	d.dossier->enable (false);

	d.battle_idx = addTab (d.battle, QIcon (*RES_pixmap (RID_ICON_BATTLE)), "Battle");
	d.battle->enable (false);

	d.bturn_idx = addTab (d.bturn, QIcon (*RES_pixmap (RID_ICON_TURN)), "Turn");
	d.bturn->enable (false);

	if (!connect (this, SIGNAL (currentChanged(int)), SLOT (selectedCurrentIndex(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <currentChanged> to <selectedCurrentIndex>");

	tabBar()->setTabTextColor (currentIndex(), *RES_color (RID_TAB_FG_SEL));

	SET_GUICLS_NOERR;
}

GuiMainRpt::~GuiMainRpt (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
}

void
GuiMainRpt::report (MDLD_TREE_ITEM *item)
{
	if (!item) return;

	DBG_log ("[%s] item=0x%8.8x\n", __FUNCTION__, item);

	switch (item->type) {
		case MDLD_TREE_DOSSIER:
			if (!item->data.d || !item->data.d->bcnt) {
				prepare_battle (NULL);
				prepare_bturn (NULL);
			} else {
				prepare_battle (MDLD_TREE_lower_to (item, MDLD_TREE_BATTLE));
				prepare_bturn (MDLD_TREE_lower_to (item, MDLD_TREE_BTURN));
			}
			select_dossier (item);
			setCurrentIndex (indexOf (d.dossier));
			break;
		case MDLD_TREE_STDALONE:
		case MDLD_TREE_BATTLE:
			prepare_bturn (MDLD_TREE_lower_to (item, MDLD_TREE_BTURN));
			select_dossier (MDLD_TREE_raise_to (item, MDLD_TREE_DOSSIER));
			select_battle (item);
			setCurrentIndex (indexOf (d.battle));
			break;
		case MDLD_TREE_BTURN:
			select_battle (MDLD_TREE_raise_to (item, MDLD_TREE_BATTLE));
			select_dossier (MDLD_TREE_raise_to (item, MDLD_TREE_DOSSIER));
			select_bturn (item);
			setCurrentIndex (indexOf (d.bturn));
			break;
		default:
			break;
	}
}

void
GuiMainRpt::update (void)
{
	DEVASSERT (d.dossier != 0);

	d.dossier->refresh(true);
	d.battle->refresh(true);
	d.bturn->refresh(true);
}

void
GuiMainRpt::clear (void)
{
	if (d.dossier) d.dossier->select (NULL);
	if (d.battle) d.battle->select (NULL);
	if (d.bturn) d.bturn->select (NULL);
}

void
GuiMainRpt::will_delete (MDLD_TREE_ITEM *item)
{
	if (d.dossier) d.dossier->will_delete (item);
	if (d.battle) d.battle->will_delete (item);
	if (d.bturn) d.bturn->will_delete (item);
}

void
GuiMainRpt::select_dossier (MDLD_TREE_ITEM *item)
{
	DEVASSERT (d.dossier != 0);

	d.dossier->select (item);
}

void
GuiMainRpt::select_battle (MDLD_TREE_ITEM *item)
{
	DEVASSERT (d.battle != 0);

	d.battle->select (item);
}

void
GuiMainRpt::select_bturn (MDLD_TREE_ITEM *item)
{
	DEVASSERT (d.bturn != 0);

	d.bturn->select (item);
}

MDLD_TREE_ITEM *
GuiMainRpt::prepare_dossier (MDLD_TREE_ITEM *item)
{
	DEVASSERT (d.dossier != 0);

	d.dossier->select (item);

	return (item);
}

MDLD_TREE_ITEM *
GuiMainRpt::prepare_battle (MDLD_TREE_ITEM *item)
{
	MDLD_TREE_ITEM	*cur;

	DEVASSERT (d.battle != 0);

	cur = d.battle->current();
	if (!item || !cur) {
		d.battle->select (item); cur = item;
	}
	return (cur);
}

MDLD_TREE_ITEM *
GuiMainRpt::prepare_bturn (MDLD_TREE_ITEM *item)
{
	MDLD_TREE_ITEM	*cur;

	DEVASSERT (d.bturn != 0);

	cur = d.bturn->current();
	if (!item || !cur || (cur->parent != item->parent)) {
		d.bturn->select (item); cur = item;
	}
	return (cur);
}

void
GuiMainRpt::selectedCurrentIndex (int index)
{
	int		i;
	MDLD_TREE_ITEM	*item = NULL;

	DBG_log ("[%s] index=0x%8.8x\n", __FUNCTION__, index);

	for (i=0; i<count(); i++)
		tabBar()->setTabTextColor (i, *((i!=index) ? RES_color (RID_TAB_FG_DEFAULT): RES_color (RID_TAB_FG_SEL)));

	if (index == d.dossier_idx) {
		d.dossier->refresh(false);
		item = d.dossier->current ();
	}
	if (index == d.battle_idx) {
		d.battle->refresh();
		item = d.battle->current ();
	}
	if (index == d.bturn_idx) {
		d.bturn->refresh();
		item = d.bturn->current ();
	}

	DBG_log ("[%s] item=0x%8.8x\n", __FUNCTION__, item);
	if (!item) return;

	GUI_WIN->get_dossier()->setCurrentItem (item);
}
