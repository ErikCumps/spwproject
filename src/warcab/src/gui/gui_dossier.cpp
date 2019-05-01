/** \file
 * The SPWaW war cabinet - GUI - dossier treeview.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_dossier.h"

#ifdef	_DEBUG
#include "model/model_sanity.h"
#endif	/* _DEBUG */

static void report_GMD (MDLD_TREE_ITEM *p);

GuiDossier::GuiDossier (QWidget *P)
	: QTreeView (P)
{
	DBG_TRACE_CONSTRUCT;

	DEVASSERT (P != 0);

	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Create model */
	GUINEW (d.model, ModelDossier (NULL), ERR_GUI_DOSSIER_INIT_FAILED, "data model");

#ifdef	_DEBUG
	ModelSanity *s = NULL;
	GUINEW (s, ModelSanity (d.model, this), ERR_GUI_DOSSIER_INIT_FAILED, "data model sanity checker");
#endif	/* _DEBUG */

	/* Setup view */
	setModel (d.model);
	setRootIsDecorated (true);
	setSortingEnabled (false);
	setUniformRowHeights (true);
	setAlternatingRowColors (false);
	setDragEnabled (false);
	setSelectionBehavior (QAbstractItemView::SelectRows);
	setSelectionMode (QAbstractItemView::SingleSelection);
	sortByColumn (1, Qt::AscendingOrder);
	resizeColumnToContents (0);
	setIconSize (QSize(28,16));
	setContextMenuPolicy (Qt::DefaultContextMenu);
	//header()->setVisible (false);

	if (!connect (WARCAB, SIGNAL (was_loaded(MDLD_TREE_ITEM *)), SLOT (was_loaded(MDLD_TREE_ITEM *))))
		SET_GUICLS_ERROR (ERR_GUI_MAINWINDOW_INIT_FAILED, "failed to connect <WARCAB:was_loaded> to <has_loaded>");
	if (!connect (WARCAB, SIGNAL (will_close(void)), SLOT (will_close(void))))
		SET_GUICLS_ERROR (ERR_GUI_MAINWINDOW_INIT_FAILED, "failed to connect <WARCAB:will_close> to <will_close>");
	if (!connect (WARCAB, SIGNAL (was_closed(void)), SLOT (was_closed(void))))
		SET_GUICLS_ERROR (ERR_GUI_MAINWINDOW_INIT_FAILED, "failed to connect <WARCAB:was_closed> to <was_closed>");
	if (!connect (WARCAB, SIGNAL (was_added(MDLD_TREE_ITEM *)), SLOT (was_added(MDLD_TREE_ITEM *))))
		SET_GUICLS_ERROR (ERR_GUI_MAINWINDOW_INIT_FAILED, "failed to connect <WARCAB:was_added> to <was_added>");
	if (!connect (WARCAB, SIGNAL (will_delete(MDLD_TREE_ITEM *)), SLOT (will_delete(MDLD_TREE_ITEM *))))
		SET_GUICLS_ERROR (ERR_GUI_MAINWINDOW_INIT_FAILED, "failed to connect <WARCAB:will_delete> to <will_delete>");
	if (!connect (WARCAB, SIGNAL (was_deleted(MDLD_TREE_ITEM *)), SLOT (was_deleted(MDLD_TREE_ITEM *))))
		SET_GUICLS_ERROR (ERR_GUI_MAINWINDOW_INIT_FAILED, "failed to connect <WARCAB:was_deleted> to <was_deleted>");
	if (!connect (WARCAB, SIGNAL (was_edited(void)), SLOT (was_edited(void))))
		SET_GUICLS_ERROR (ERR_GUI_MAINWINDOW_INIT_FAILED, "failed to connect <WARCAB:was_edited> to <was_edited>");

	SET_GUICLS_NOERR;
}

GuiDossier::~GuiDossier (void)
{
	DBG_TRACE_DESTRUCT;

	if (d.model) delete d.model;
}

void
GuiDossier::contextMenuEvent (QContextMenuEvent *event)
{
	QMenu		*menu;

	menu = new QMenu (this);

	DBG_log ("[%s] event=0x%8.8x\n", __FUNCTION__, event);
	DBG_log ("[%s] global pos: x=%d, y=%d\n", __FUNCTION__, event->globalX(), event->globalY());
	DBG_log ("[%s] widget pos: x=%d, y=%d\n", __FUNCTION__, event->x(), event->y());

	d.actionitem = selected (); if (d.actionitem == NULL) return;
	report_GMD (d.actionitem);

	switch (d.actionitem->type) {
		case MDLD_TREE_DOSSIER:
			menu->addAction (GUI_ACTIONS->p.delete_dossier);
			break;
		case MDLD_TREE_STDALONE:
		case MDLD_TREE_BATTLE:
			menu->addAction (GUI_ACTIONS->p.delete_battle);
			break;
		case MDLD_TREE_BTURN:
			menu->addAction (GUI_ACTIONS->p.delete_turn);
			break;
		default:
			DBG_log ("[%s] INVALID DATA TYPE for selection!\n", __FUNCTION__);
			break;
	}
	menu->addSeparator ();

	if (d.actionitem->dossier_type != SPWAW_STDALONE_DOSSIER) {
		menu->addAction (GUI_ACTIONS->p.file_add_campaign_savegame);
#if	ALLOW_SNAPSHOTS_LOAD
		menu->addAction (GUI_ACTIONS->p.file_add_campaign_snapshot);
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	} else {
		menu->addAction (GUI_ACTIONS->p.add_battle_savegame);
#if	ALLOW_SNAPSHOTS_LOAD
		menu->addAction (GUI_ACTIONS->p.add_battle_snapshot);
#endif	/* ALLOW_SNAPSHOTS_LOAD */
		menu->addSeparator ();

		menu->addAction (GUI_ACTIONS->p.file_add_battle_savegame);
#if	ALLOW_SNAPSHOTS_LOAD
		menu->addAction (GUI_ACTIONS->p.file_add_battle_snapshot);
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	}
	menu->addSeparator ();

	menu->addAction (GUI_ACTIONS->p.dossier_edit);

	menu->exec(event->globalPos());

	d.actionitem = NULL;

	delete menu;
}

void
GuiDossier::setCurrentItem (MDLD_TREE_ITEM *item)
{
	setCurrentIndex (d.model->index(item));
}

MDLD_TREE_ITEM *
GuiDossier::get_actionitem (void)
{
	return (d.actionitem);
}

void
GuiDossier::refresh (void)
{
	//MDLD_TREE_ITEM	*item;

	DBG_TRACE_FENTER;

	expandAll();

	// TODO: this code expands the currently selected item,
	// but it does not expand all previously expanded items!
	//item = selected ();
	//if (item) {
	//	while (item->type != MDLD_TREE_DOSSIER) {
	//		expand (d.model->index(item));
	//		item = item->parent;
	//	}
	//	expand (d.model->index(item));
	//}

	DBG_TRACE_FLEAVE;
}

MDLD_TREE_ITEM *
GuiDossier::selected (void)
{
	return (d.model->item(this->currentIndex()));
}

static void
report_GMD (MDLD_TREE_ITEM *p)
{
	char	*s;

	DBG_log ("[MDLD_TREE_ITEM] 0x%8.8x\n", p);
	if (!p) return;

	DBG_log ("[MDLD_TREE_ITEM] type=%d, parent=0x%8.8x, children=%d\n", p->type, p->parent, p->children.size());
	DBG_log ("[MDLD_TREE_ITEM] seqnum=%d, campaign=%s\n", p->seqnum.value(), (p->dossier_type == SPWAW_CAMPAIGN_DOSSIER) ? "true" : "false");

	switch (p->type) {
		case MDLD_TREE_DOSSIER:
			DBG_log ("[MDLD_TREE_ITEM] DOSSIER 0x%8.8x:\n", p->data.d);
			if (p->data.d) {
				DBG_log ("[MDLD_TREE_ITEM_DOSSIER] name         = %s\n", p->data.d->name);
				//DBG_log ("[MDLD_TREE_ITEM_DOSSIER] comment      = %s\n", p->data.d->comment);
				DBG_log ("[MDLD_TREE_ITEM_DOSSIER] oobdir       = %s\n", p->data.d->oobdir);
				DBG_log ("[MDLD_TREE_ITEM_DOSSIER] OOB          = %u\n", p->data.d->props.OOB);
				DBG_log	("[MDLD_TREE_ITEM_DOSSIER] fcnt         = %u\n", p->data.d->props.fcnt);
				DBG_log ("[MDLD_TREE_ITEM_DOSSIER] ucnt         = %u\n", p->data.d->props.ucnt);
				SPWAW_date2str (&(p->data.d->props.start), &s);
				DBG_log ("[MDLD_TREE_ITEM_DOSSIER] start        = %s\n", s); free(s);
				SPWAW_date2str (&(p->data.d->props.end), &s);
				DBG_log ("[MDLD_TREE_ITEM_DOSSIER] end          = %s\n", s); free(s);
				DBG_log ("[MDLD_TREE_ITEM_DOSSIER] max battles  = %u\n", p->data.d->props.maxbtlcnt);
				DBG_log ("[MDLD_TREE_ITEM_DOSSIER] battle count = %u\n", p->data.d->bcnt);

			}
			break;
		case MDLD_TREE_STDALONE:
			DBG_log ("[MDLD_TREE_ITEM] STANDALONE BATTLE 0x%8.8x:\n", p->data.b);
			break;
		case MDLD_TREE_BATTLE:
			DBG_log ("[MDLD_TREE_ITEM] BATTLE 0x%8.8x:\n", p->data.b);
			if (p->data.b) {
				SPWAW_date2str (&(p->data.b->date), &s);
				DBG_log ("[MDLD_TREE_ITEM_BATTLE] date     = %s\n", s); free (s);
				DBG_log ("[MDLD_TREE_ITEM_BATTLE] location = %s\n", p->data.b->location);
				DBG_log ("[MDLD_TREE_ITEM_BATTLE] OOB_p1   = %d\n", p->data.b->OOB_p1);
				DBG_log ("[MDLD_TREE_ITEM_BATTLE] OOB_p2   = %d\n", p->data.b->OOB_p2);
				DBG_log ("[MDLD_TREE_ITEM_BATTLE] miss_p1  = %s\n", p->data.b->miss_p1);
				DBG_log ("[MDLD_TREE_ITEM_BATTLE] miss_p2  = %s\n", p->data.b->miss_p2);
				DBG_log ("[MDLD_TREE_ITEM_BATTLE] turns    = %d\n", p->data.b->tcnt);
			}
			break;
		case MDLD_TREE_BTURN:
			DBG_log ("[MDLD_TREE_ITEM] BTURN 0x%8.8x:\n", p->data.t);
			if (p->data.t) {
				SPWAW_date2str (&(p->data.t->date), &s);
				DBG_log ("[MDLD_TREE_ITEM_BTURN] date = %s\n", s); free (s);
				DBG_log ("[MDLD_TREE_ITEM_BTURN] turn = %d\n", p->data.t->turn);
			}
			break;
		default:
			DBG_log ("[MDLD_TREE_ITEM] UNKNOWN DATA TYPE.\n");
			break;
	}
	if (p->parent) report_GMD (p->parent);
}

void
GuiDossier::was_loaded (MDLD_TREE_ITEM *tree)
{
	DBG_log ("[%s] tree=0x%8.8x\n", __FUNCTION__, tree);

	if (!tree) return;
	report_GMD (tree);

	d.model->load (tree);
	refresh();
	setCurrentIndex (d.model->root_index());

	GUI_ACTIONS->enable_dossier_actions (true, tree->dossier_type);
}

void
GuiDossier::will_close (void)
{
	DBG_log ("[%s]\n", __FUNCTION__);

	GUI_WIN->get_report()->clear ();
	GUI_ACTIONS->enable_dossier_actions (false, SPWAW_EMPTY_DOSSIER);
}

void
GuiDossier::was_closed (void)
{
	DBG_log ("[%s]\n", __FUNCTION__);

	d.model->clear();
	refresh();
}

void
GuiDossier::was_added (MDLD_TREE_ITEM *item)
{
	DBG_log ("[%s] item=0x%8.8x\n", __FUNCTION__, item);

	if (!item) return;
	report_GMD (item);

	d.model->refresh();
	refresh();
	setCurrentItem (item);

	GUI_ACTIONS->enable_dossier_actions (true, item->dossier_type);
}

void
GuiDossier::will_delete (MDLD_TREE_ITEM *item)
{
	DBG_log ("[%s] item=0x%8.8x\n", __FUNCTION__, item);

	if (!item) {
		GUI_WIN->get_report()->clear ();
	} else {
		report_GMD (item);
		GUI_WIN->get_report()->will_delete (item);
	}
}

void
GuiDossier::was_deleted (MDLD_TREE_ITEM *next)
{
	DBG_log ("[%s]\n", __FUNCTION__);

	d.model->refresh();
	refresh();

	setCurrentItem (next);
}

void
GuiDossier::was_edited (void)
{
	if (d.model) {
		DBG_log ("[%s] requesting report update\n", __FUNCTION__);
		GUI_WIN->get_report()->update();
	}
}


void
GuiDossier::delete_item (void)
{
	if (d.actionitem == NULL) return;
	//report_GMD (d.actionitem);

	switch (d.actionitem->type) {
		case MDLD_TREE_STDALONE:
		case MDLD_TREE_BATTLE:
		case MDLD_TREE_BTURN:
			WARCAB->del (d.actionitem);
			break;
		default:
			DBG_log ("[%s] INVALID DATA TYPE for selection!\n", __FUNCTION__);
			break;
	}
}

void
GuiDossier::select_parent_item (void)
{
	MDLD_TREE_ITEM	*p;

	p = selected();
	if (p == NULL) return;
	if (p->type == MDLD_TREE_DOSSIER) return;

	setCurrentItem (p->parent);
}

void
GuiDossier::select_first_item (void)
{
	MDLD_TREE_ITEM	*p, *q;

	p = selected();
	if (p == NULL) return;
	if (p->type == MDLD_TREE_DOSSIER) return;

	if (p->type == MDLD_TREE_BATTLE && !(p->dossier_type == SPWAW_CAMPAIGN_DOSSIER)) {
		q = p->parent->prev ? p->parent->parent->cfirst->cfirst : NULL;
	} else {
		q = p->prev ? p->parent->cfirst : NULL;
	}

	if (!q) return;

	setCurrentItem (q);

}

void
GuiDossier::select_prev_item (void)
{
	MDLD_TREE_ITEM	*p, *q;

	p = selected();
	if (p == NULL) return;
	if (p->type == MDLD_TREE_DOSSIER) return;

	if (p->type == MDLD_TREE_BATTLE && !(p->dossier_type == SPWAW_CAMPAIGN_DOSSIER)) {
		q = p->parent->prev ? p->parent->prev->cfirst : NULL;
	} else {
		q = p->prev;
	}
	if (!q) return;

	setCurrentItem (q);
}

void
GuiDossier::select_next_item (void)
{
	MDLD_TREE_ITEM	*p, *q;

	p = selected();
	if (p == NULL) return;
	if (p->type == MDLD_TREE_DOSSIER) return;

	if (p->type == MDLD_TREE_BATTLE && !(p->dossier_type == SPWAW_CAMPAIGN_DOSSIER)) {
		q = p->parent->next ? p->parent->next->cfirst : NULL;
	} else {
		q = p->next;
	}
	if (!q) return;

	setCurrentItem (q);
}

void
GuiDossier::select_last_item (void)
{
	MDLD_TREE_ITEM	*p, *q;

	p = selected();
	if (p == NULL) return;
	if (p->type == MDLD_TREE_DOSSIER) return;

	if (p->type == MDLD_TREE_BATTLE && !(p->dossier_type == SPWAW_CAMPAIGN_DOSSIER)) {
		q = p->parent->next ? p->parent->parent->clast->cfirst : NULL;
	} else {
		q = p->next ? p->parent->clast : NULL;
	}

	if (!q) return;

	setCurrentItem (q);
}

void
GuiDossier::select_child_item (void)
{
	MDLD_TREE_ITEM	*p;

	p = selected();
	if (p == NULL) return;
	if (p->type == MDLD_TREE_BTURN) return;
	if (p->cfirst == NULL) return;

	setCurrentItem (p->cfirst);
}

void
GuiDossier::currentChanged (const QModelIndex &current, const QModelIndex &previous)
{
	MDLD_TREE_ITEM	*old, *cur;

	scrollTo (current);

	old = d.model->item((QModelIndex)previous);
	cur = d.model->item((QModelIndex)current);

	DBG_log ("[%s] cur=0x%8.8x, old=0x%8.8x\n", __FUNCTION__, cur, old);

	if (cur != old) {
		DBG_log ("[%s] requesting report\n", __FUNCTION__);
		GUI_WIN->get_report()->report (cur);
	} else {
		DBG_log ("[%s] NO report requested\n", __FUNCTION__);
	}
}

void
GuiDossier::set_cmpcurr (MDLD_TREE_ITEM *item)
{
	d.model->set_cmpcurr (item);
	refresh();
}

void
GuiDossier::set_cmpbase (MDLD_TREE_ITEM *item)
{
	d.model->set_cmpbase (item);
	refresh();
}

