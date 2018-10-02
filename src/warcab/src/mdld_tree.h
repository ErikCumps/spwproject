/** \file
 * The SPWaW war cabinet - MDLD_TREE handling.
 *
 * Copyright (C) 2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MDLD_TREE_H
#define	MDLD_TREE_H	1

#include "stdafx.h"

#include "util/util_seqnum.h"

typedef enum e_MDLD_TREE_TYPE {
	MDLD_TREE_NONE = 0,
	MDLD_TREE_DOSSIER,
	MDLD_TREE_STDALONE,
	MDLD_TREE_BATTLE,
	MDLD_TREE_BTURN
} MDLD_TREE_TYPE;

typedef struct s_MDLD_TREE_ITEM MDLD_TREE_ITEM;

struct s_MDLD_TREE_ITEM {
	MDLD_TREE_TYPE			type;
	union u_data {
		SPWAW_DOSSIER		*d;
		SPWAW_BATTLE		*b;
		SPWAW_BTURN		*t;
	}				data;
	MDLD_TREE_ITEM		*parent;
	MDLD_TREE_ITEM		*prev;
	MDLD_TREE_ITEM		*next;
	QList<MDLD_TREE_ITEM *>	children;
	MDLD_TREE_ITEM		*cfirst;
	MDLD_TREE_ITEM		*clast;
	UtilSeqnum		seqnum;
	SPWAW_DOSSIER_TYPE	dossier_type;	/*!< Dossier type	*/
};

static inline MDLD_TREE_ITEM *
MDLD_TREE_raise_to (MDLD_TREE_ITEM *item, MDLD_TREE_TYPE target)
{
	MDLD_TREE_ITEM *p = item;

	while (p && (p->type != target)) p = p->parent;

	return (p);
}

static inline MDLD_TREE_ITEM *
MDLD_TREE_lower_to (MDLD_TREE_ITEM *item, MDLD_TREE_TYPE target)
{
	MDLD_TREE_ITEM *p = item;

	while (p && (p->type != target)) p = p->cfirst;

	return (p);
}

static inline void
MDLD_TREE_extract_children (MDLD_TREE_ITEM *item, QList<MDLD_TREE_ITEM *> &list)
{
	list = item->children;

	item->children.clear();	item->cfirst = item->clast = NULL;
}

extern void		MDLD_TREE_update_seqnums	(MDLD_TREE_ITEM *item);
extern void		MDLD_TREE_link_as_child		(MDLD_TREE_ITEM *item);
extern MDLD_TREE_ITEM *	MDLD_TREE_new_battle		(SPWAW_BATTLE *data, MDLD_TREE_ITEM *tree);
extern void		MDLD_TREE_delete_battle		(MDLD_TREE_ITEM *b);
extern MDLD_TREE_ITEM *	MDLD_TREE_new_bturn		(SPWAW_BTURN *data, MDLD_TREE_ITEM *tree);
extern void		MDLD_TREE_delete_bturn		(MDLD_TREE_ITEM *t);

#endif	/* MDLD_TREE_H */
