/** \file
 * The SPWaW war cabinet - data model handling - dossier.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "model_dossier.h"
#include "gui/gui_dossier.h"

ModelDossier::ModelDossier (QObject *parent)
	: QAbstractItemModel (parent)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	header << "Campaign dossier";
	d.col_cnt = 1;
}

ModelDossier::~ModelDossier (void)
{
	DBG_TRACE_DESTRUCT;

	d.tree = NULL;
}

QBrush
ModelDossier::item_brush (int role, MDLD_TREE_ITEM *item) const
{
	QBrush	b = QBrush();

	if (!item) return (b);

	switch (role) {
		case Qt::ForegroundRole:
			if (item == d.cmpbase) {
				b = QBrush (*RES_color(RID_GMD_FG_CMPBASE));
			} else if (item == d.cmpcurr) {
				b = QBrush (*RES_color(RID_GMD_FG_CMPCURR));
			} else if (item == d.selected) {
				b = QBrush (*RES_color(RID_GMD_FG_SEL));
			} else {
				b = QBrush (*RES_color(RID_GMD_FG_DEFAULT));
			}
			break;
		case Qt::BackgroundRole:
			if (item == d.cmpbase) {
				b = QBrush (*RES_color(RID_GMD_BG_CMPBASE));
			} else if (item == d.cmpcurr) {
				b = QBrush (*RES_color(RID_GMD_BG_CMPCURR));
			} else if (item == d.selected) {
				b = QBrush (*RES_color(RID_GMD_BG_SEL));
			} else {
				b = QBrush (*RES_color(RID_GMD_BG_DEFAULT));
			}
			break;
		default:
			break;
	}
	return (b);
}

QVariant
ModelDossier::data_dossier (int role, MDLD_TREE_ITEM *p) const
{
	QVariant	v = QVariant();
	char		buf[256];

	if (!p || (p->type != MDLD_TREE_DOSSIER) || (!p->data.d)) return (v);
	memset (buf, 0, sizeof (buf));

	switch (role) {
		case Qt::DisplayRole:
			snprintf (buf, sizeof (buf) - 1, "%s (%s forces)",
				p->data.d->name, SPWAW_oob_people (p->data.d->OOB));
			v = QVariant (buf);
			break;
		case Qt::DecorationRole:
			v = QVariant (QIcon (*RES_flag (p->data.d->OOB)));
			break;
		case Qt::ForegroundRole:
		case Qt::BackgroundRole:
			v = item_brush (role, p);
			break;
		default:
			break;
	}
	return (v);
}

QVariant
ModelDossier::data_battle (int role, MDLD_TREE_ITEM *p) const
{
	QVariant	v = QVariant();
	char		buf[256];

	if (!p || (p->type != MDLD_TREE_BATTLE) || (!p->data.b)) return (v);
	memset (buf, 0, sizeof (buf));

	switch (role) {
		case Qt::DisplayRole:
			snprintf (buf, sizeof (buf) - 1, "%s: %s against %s %s",
				p->data.b->location, p->data.b->miss_p1, SPWAW_oob_people (p->data.b->OOB), p->data.b->miss_p2);
			v = QVariant (buf);
			break;
		case Qt::DecorationRole:
			v = QVariant (QIcon (*RES_flag (p->data.b->OOB)));
			break;
		case Qt::ForegroundRole:
		case Qt::BackgroundRole:
			v = item_brush (role, p);
			break;
		default:
			break;
	}
	return (v);
}

QVariant
ModelDossier::data_bturn (int role, MDLD_TREE_ITEM *p) const
{
	QVariant	v = QVariant();
	char		buf[256];

	if (!p || (p->type != MDLD_TREE_BTURN) || (!p->data.t)) return (v);
	memset (buf, 0, sizeof (buf));

	switch (role) {
		case Qt::DisplayRole:
			snprintf (buf, sizeof (buf) - 1, "%s, turn %u",
				p->data.t->snap->game.battle.strings.date, p->data.t->snap->game.battle.data.turn);
			v = QVariant (buf);
			break;
		case Qt::ForegroundRole:
		case Qt::BackgroundRole:
			v = item_brush (role, p);
			break;
		default:
			break;
	}
	return (v);
}

QVariant
ModelDossier::data (const QModelIndex &index, int role) const
{
	MDLD_TREE_ITEM	*p;
	QVariant	v = QVariant();

	if (!index.isValid()) return (QVariant());

	if (role == Qt::UserRole)
		return (QVariant ((int)((MDLD_TREE_ITEM *)index.internalPointer())));

	p = (MDLD_TREE_ITEM *)index.internalPointer();
	if (!p) return (QVariant());

	switch (p->type) {
		case MDLD_TREE_DOSSIER:
			v = data_dossier (role, p);
			break;
		case MDLD_TREE_BATTLE:
			v = data_battle (role, p);
			break;
		case MDLD_TREE_BTURN:
			v = data_bturn (role, p);
			break;
		default:
			break;
	}
	return (v);
}

Qt::ItemFlags
ModelDossier::flags (const QModelIndex &index) const
{
	if (!index.isValid()) return (0);

	return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

QVariant
ModelDossier::headerData (int section, Qt::Orientation orientation, int role) const
{
	if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return (QVariant());

	return (header.value (section));
}

QModelIndex
ModelDossier::index (int row, int column, const QModelIndex &parent) const
{
	MDLD_TREE_ITEM	*p;

	if (!hasIndex (row, column, parent)) return (QModelIndex());
	if (!parent.isValid()) {
		if (row < 1)
			return (createIndex (row, column, (void *)d.tree));
		else
			return (QModelIndex());
	} else {
		p = (MDLD_TREE_ITEM *)parent.internalPointer();
		if (!p) return (QModelIndex());
		if (row < p->children.size()) {
			return (createIndex (row, column, p->children[row]));
		} else {
			return (QModelIndex());
		}
	}
}

QModelIndex
ModelDossier::index (MDLD_TREE_ITEM *item) const
{
	int	row = 0;

	if (item->parent) {
		row = item->parent->children.indexOf (item);
	} else {
		row = 0;
	}
	return (createIndex (row, 0, (void *)item));
}

QModelIndex
ModelDossier::parent (const QModelIndex &index) const
{
	MDLD_TREE_ITEM	*p;
	unsigned long	r;

	if (!index.isValid()) return (QModelIndex());
	p = (MDLD_TREE_ITEM *)index.internalPointer();
	if (!p) return (QModelIndex());
	if (!p->parent) return (QModelIndex());
	p = p->parent;
	if (!p->parent) return (createIndex (0, 0, (void *)d.tree));
	r = p->parent->children.indexOf (p);
	return (createIndex (r, 0, p));
}

int
ModelDossier::rowCount (const QModelIndex &parent) const
{
	MDLD_TREE_ITEM	*p;

	if (!parent.isValid()) return (1);
	p = (MDLD_TREE_ITEM *)parent.internalPointer();
	return (p ? p->children.size() : 0);
}

int
ModelDossier::columnCount (const QModelIndex &/*parent*/) const
{
	return (d.col_cnt);
}

void
ModelDossier::sort (int /*column*/, Qt::SortOrder /*order*/)
{
	// not implemented
}

//bool
//ModelDossier::removeRows (int row, int count, const QModelIndex &parent)
//{
//	MDLD_TREE_ITEM	*p = NULL;
//	int		i;
//
//	if ((row < 0) || (count <= 0)) return (false);
//	if (!parent.isValid()) return (false);
//
//	p = (MDLD_TREE_ITEM *)parent.internalPointer();
//	if (p->type != MDLD_TREE_BATTLE) return (false);
//	if (row >= p->children.size()) return (false);
//	if ((row + count - 1) >= p->children.size()) return (false);
//
//	for (i=0; i<count; i++)
//		SPWAW_dossier_del (p->parent->data.d, p->data.b->tlist[row]);
//
//	//if (count == p->children.size()) {
//	//	freeModelData();
//	//	setupModelData();
//	//	reset();
//	//} else {
//		beginRemoveRows (parent, row, row + count - 1);
//
//		i = row + count - 1;
//		while (i >= row) p->children.removeAt (i--);
//
//		endRemoveRows();
//
//		if (!p->children.size()) {
//			QModelIndex pp = p->parent->index;
//			p->index.row()
//			beginRemoveRows (pp, p->index.row(), p->index.row() + 1);
//			p->parent->children.removeAt(p->index.row());
//			endRemoveRows();
//		}
//	//}
//
//	return (true);
//}

MDLD_TREE_ITEM *
ModelDossier::item (const QModelIndex &idx)
{
	if (is_initial_empty_index (idx)) return (NULL);

	if (!idx.isValid()) {
		DBG_log ("[%s] invalid index!\n", __FUNCTION__);
		return (NULL);
	}

	return ((MDLD_TREE_ITEM *)idx.internalPointer());
}

QModelIndex
ModelDossier::root_index (void)
{
	if (!d.tree) return (QModelIndex());

	return (createIndex (0, 0, (void *)d.tree));
}

void
ModelDossier::clear (void)
{
	d.tree = NULL;
	reset();
}

void
ModelDossier::load (MDLD_TREE_ITEM *tree)
{
	d.tree = tree;
	reset();
}

void
ModelDossier::refresh (void)
{
	DBG_TRACE_FENTER;

	reset();

	DBG_TRACE_FLEAVE;
}

void
ModelDossier::set_selected (MDLD_TREE_ITEM *item)
{
	d.selected = item;
}

void
ModelDossier::set_cmpcurr (MDLD_TREE_ITEM *item)
{
	d.cmpcurr = item;
}

void
ModelDossier::set_cmpbase (MDLD_TREE_ITEM *item)
{
	d.cmpbase = item;
}
