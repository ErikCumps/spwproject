/** \file
 * The SPWaW war cabinet - data model handling - snapshot list.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "model_snaplist.h"

ModelSnapList::ModelSnapList (SPWAW_SNAPLIST_TARGET &target, char *path, SPWAW_SNAPLIST *ignore, QObject *parent)
	: QAbstractItemModel (parent)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	header << "filename" << "game" << "type" << "battle date" << "battle location" << "comment";
	d.col_cnt = 6;

	d.target = target;

	setupModelData (path, ignore);
}

ModelSnapList::~ModelSnapList (void)
{
	DBG_TRACE_DESTRUCT;

	freeModelData ();
}

QVariant
ModelSnapList::data (const QModelIndex &index, int role) const
{
	QVariant	v;

	if (!index.isValid()) return (QVariant());
	switch (role) {
		case Qt::DisplayRole:
			v = QVariant ((((NODE_IDX *)index.internalPointer())->ptr->data[index.column()]));
			break;
		case Qt::UserRole:
			v = QVariant ((int)(((NODE_IDX *)index.internalPointer())->ptr->node));
			break;
		default:
			v = QVariant();
			break;
	}

	return (v);
}

Qt::ItemFlags
ModelSnapList::flags (const QModelIndex &index) const
{
	if (!index.isValid()) return (0);

	return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

QVariant
ModelSnapList::headerData (int section, Qt::Orientation orientation, int role) const
{
	if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return (QVariant());

	return (header.value (section));
}

QModelIndex
ModelSnapList::index (int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex (row, column, parent)) return (QModelIndex());

	return (createIndex (row, column, (void *)&(data_idx[row])));
}

QModelIndex
ModelSnapList::parent (const QModelIndex &/*index*/) const
{
	return (QModelIndex());
}

int
ModelSnapList::rowCount (const QModelIndex &/*parent*/) const
{
	return (data_list.count ());
}

int
ModelSnapList::columnCount (const QModelIndex &/*parent*/) const
{
	return (d.col_cnt);
}

void
ModelSnapList::setupModelData (char *path, SPWAW_SNAPLIST *ignore)
{
	SPWAW_ERROR		rc;
	unsigned long		i;
	SPWAW_SNAPLIST_NODE	*p = NULL;

	DBG_TRACE_FENTER;

	freeModelData();

	rc = SPWAW_snaplist (&(d.target), path, ignore, &(d.snap_list));
	if (rc != SPWERR_OK) return;

	d.row_cnt = d.snap_list->cnt;
	if (!d.row_cnt) return;

	for (i=0; i<d.snap_list->cnt; i++) {
		NODE_DATA	node;
		NODE_IDX	idx;

		p = d.snap_list->list[i];

		node.data	<< p->filename
				<< SPWAW_gametype2str(p->info.gametype)
				<< SPWAW_battletype2str(p->info.type)
				<< p->info.stamp
				<< p->info.location
				<< p->info.title;
		node.node = p;

		data_list.append (node);
		idx.ptr = &(data_list[i]);
		data_idx.append (idx);
	}

	DBG_TRACE_FLEAVE;
}

void
ModelSnapList::freeModelData (void)
{
	data_idx.clear();
	data_list.clear();
	SPWAW_snaplist_free (&d.snap_list);
}

QString
ModelSnapList::sort_transform (NODE_DATA *ptr, int col, int idx)
{
	QString	turn = "turn ";
	int	pos;
	QString	s1, s2, col1, col2, col3, col4;
	QString	out;

	col1 = ptr->data[1].toString();
	col2 = ptr->data[2].toString();
	col4 = ptr->data[4].toString();

	pos = ptr->data[3].toString().indexOf (turn, 0, Qt::CaseSensitive);
	if (pos != -1) {
		s1 = ptr->data[3].toString().mid(0, pos + turn.length());
		s2 = ptr->data[3].toString().mid (pos + turn.length());
		col3.sprintf ("%s%06.6u", qPrintable(s1), s2.toUInt());
	} else {
		col3 = ptr->data[3].toString();
	}

	switch (col) {
		case 1:
			out = col1 + col3;
			break;
		case 2:
			out = col2 + col3;
			break;
		case 3:
			out = col3;
			break;
		case 4:
			out = col4 + col3;
			break;
		case 5:
			out = ptr->data[5].toString() + col3;
			break;
		default:
			out = ptr->data[col].toString();
			break;
	}
	out += QString().setNum (idx);

	return (out);
}

void
ModelSnapList::sort (int column, Qt::SortOrder order)
{
	QMap<QString, NODE_IDX>	map;
	unsigned long		i;

	emit layoutAboutToBeChanged ();

	for (i=0; i<d.row_cnt; i++)
		map.insert (sort_transform (data_idx[i].ptr, column, i), data_idx[i]);

	data_idx = map.values();

	if (order == Qt::DescendingOrder) {
		for (i=0; i<d.row_cnt/2; i++) data_idx.swap (i, d.row_cnt-i-1);
	}

	emit layoutChanged ();
}
