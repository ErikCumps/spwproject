/** \file
 * The SPWaW war cabinet - MDLD_TREE handling.
 *
 * Copyright (C) 2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "mdld_tree.h"

void
MDLD_TREE_update_seqnums (MDLD_TREE_ITEM *item)
{
	MDLD_TREE_ITEM	*parent;

	if (!item) return;

	parent = item->parent;
	while (parent) {
                parent->seqnum.update();
		parent = parent->parent;
	}
}

void
MDLD_TREE_link_as_child (MDLD_TREE_ITEM *item)
{
	MDLD_TREE_ITEM	*parent;

	if (!item) return;

	parent = item->parent;
	if (!parent) {
		item->prev = item->next = NULL;
	} else {
		parent->children.append (item);

		item->prev = parent->clast; item->next = NULL;
		if (item->prev) item->prev->next = item;

		parent->cfirst = parent->children[0];
		parent->clast  = parent->children[parent->children.size()-1];
	}
}

MDLD_TREE_ITEM *
MDLD_TREE_new_battle (SPWAW_BATTLE *data, MDLD_TREE_ITEM *tree)
{
	MDLD_TREE_ITEM	*p = tree;
	MDLD_TREE_ITEM	*b;

	if (!tree->campaign) {
		p = new MDLD_TREE_ITEM;
		p->parent   = tree;
		p->type     = MDLD_TREE_STDALONE;
		p->data.b   = data;
		p->campaign = tree->campaign;

		p->cfirst = p->clast = NULL;

		MDLD_TREE_link_as_child (p);
	}

	b = new MDLD_TREE_ITEM;
	b->parent   = p;
	b->type     = MDLD_TREE_BATTLE;
	b->data.b   = data;
	b->campaign = tree->campaign;

	b->cfirst = b->clast = NULL;

	MDLD_TREE_link_as_child (b);

	return (b);
}

void
MDLD_TREE_delete_battle (MDLD_TREE_ITEM *b)
{
	MDLD_TREE_ITEM	*p, *q;

	MDLD_TREE_update_seqnums (b);

	if (!b->campaign) {
		 b = b->cfirst;
		 delete (b->parent);
	}

	p = b->cfirst;
	while (p) {
		q = p; p = p->next; delete (q);
	}

	delete (b);
}

MDLD_TREE_ITEM *
MDLD_TREE_new_bturn (SPWAW_BTURN *data, MDLD_TREE_ITEM *tree)
{
	MDLD_TREE_ITEM	*t;

	t = new MDLD_TREE_ITEM;
	t->parent   = tree;
	t->type     = MDLD_TREE_BTURN;
	t->data.t   = data;
	t->campaign = tree->campaign;

	t->cfirst = t->clast = NULL;

	MDLD_TREE_link_as_child (t);

	return (t);
}

void
MDLD_TREE_delete_bturn (MDLD_TREE_ITEM *t)
{
	MDLD_TREE_update_seqnums (t);
	delete (t);
}
