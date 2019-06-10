/** \file
 * The SPWaW war cabinet - data model handling - savegame list.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "model_savelist.h"

ModelSaveList::ModelSaveList (SPWAW_GAME_TYPE gametype, char *path, SPWAW_SAVELIST *ignore, QObject *parent)
	: QAbstractItemModel (parent)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	header << "filename" << "game" << "type" << "battle date" << "battle location" << "comment";
	d.col_cnt = 6;

	setupModelData (gametype, path, ignore);
}

ModelSaveList::~ModelSaveList (void)
{
	DBG_TRACE_DESTRUCT;

	freeModelData ();
}

QVariant
ModelSaveList::data (const QModelIndex &index, int role) const
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
ModelSaveList::flags (const QModelIndex &index) const
{
	if (!index.isValid()) return (0);

	return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

QVariant
ModelSaveList::headerData (int section, Qt::Orientation orientation, int role) const
{
	if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return (QVariant());

	return (header.value (section));
}

QModelIndex
ModelSaveList::index (int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex (row, column, parent)) return (QModelIndex());

	return (createIndex (row, column, (void *)&(data_idx[row])));
}

QModelIndex
ModelSaveList::parent (const QModelIndex &/*index*/) const
{
	return (QModelIndex());
}

int
ModelSaveList::rowCount (const QModelIndex &/*parent*/) const
{
	return (data_list.count ());
}

int
ModelSaveList::columnCount (const QModelIndex &/*parent*/) const
{
	return (d.col_cnt);
}

void
ModelSaveList::setupModelData (SPWAW_GAME_TYPE gametype, char *path, SPWAW_SAVELIST *ignore)
{
	SPWAW_ERROR		rc;
	unsigned long		i;
	SPWAW_SAVELIST_NODE	*p = NULL;

	DBG_TRACE_FENTER;

	freeModelData();

	rc = SPWAW_savelist (gametype, path, ignore, &(d.save_list));
	if (rc != SPWERR_OK) return;

	d.row_cnt = d.save_list->cnt;
	if (!d.row_cnt) return;

	for (i=0; i<d.save_list->cnt; i++) {
		NODE_DATA	node;
		NODE_IDX	idx;

		p = d.save_list->list[i];

		node.data	<< p->filename
				<< SPWAW_gametype2str(p->info.gametype)
				<< SPWAW_battletype2str(p->info.type)
				<< p->info.stamp
				<< p->info.location
				<< p->info.comment;
		node.node = p;

		data_list.append (node);
		idx.ptr = &(data_list[i]);
		data_idx.append (idx);
	}

	DBG_TRACE_FLEAVE;
}

void
ModelSaveList::freeModelData (void)
{
	data_idx.clear();
	data_list.clear();
	SPWAW_savelist_free (&d.save_list);
}

QString
ModelSaveList::sort_transform (NODE_DATA *ptr, int col, int idx)
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
ModelSaveList::sort (int column, Qt::SortOrder order)
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
