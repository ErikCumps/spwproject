/** \file
 * The SPWaW war cabinet - data model handling - dossier list.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "model_dosslist.h"

ModelDossList::ModelDossList (char *path, SPWAW_DOSSLIST *ignore, QObject *parent)
	: QAbstractItemModel (parent)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	header << "filename" << "game" << "type" << "nation" << "battles" << "name" << "comment";
	d.col_cnt = 7;

	setupModelData (path, ignore);
}

ModelDossList::~ModelDossList (void)
{
	DBG_TRACE_DESTRUCT;

	freeModelData ();
}

QVariant
ModelDossList::data (const QModelIndex &index, int role) const
{
	QVariant	v;

	if (!index.isValid()) return (QVariant());
	switch (role) {
		case Qt::DisplayRole:
			v = QVariant ((((NODE_IDX *)index.internalPointer())->ptr->data[index.column()]));
			break;
		case Qt::ToolTipRole:
			v = QVariant (QString(((NODE_IDX *)index.internalPointer())->ptr->node->info.comment));
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
ModelDossList::flags (const QModelIndex &index) const
{
	if (!index.isValid()) return (0);

	return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

QVariant
ModelDossList::headerData (int section, Qt::Orientation orientation, int role) const
{
	if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return (QVariant());

	return (header.value (section));
}

QModelIndex
ModelDossList::index (int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex (row, column, parent)) return (QModelIndex());

	return (createIndex (row, column, (void *)&(data_idx[row])));
}

QModelIndex
ModelDossList::parent (const QModelIndex &/*index*/) const
{
	return (QModelIndex());
}

int
ModelDossList::rowCount (const QModelIndex &/*parent*/) const
{
	return (data_list.count ());
}

int
ModelDossList::columnCount (const QModelIndex &/*parent*/) const
{
	return (d.col_cnt);
}

void
ModelDossList::setupModelData (char *path, SPWAW_DOSSLIST *ignore)
{
	SPWAW_ERROR		rc;
	unsigned long		i;
	SPWAW_DOSSLIST_NODE	*p = NULL;
	QString			cmt;

	DBG_TRACE_FENTER;

	freeModelData();

	rc = SPWAW_dosslist (path, ignore, &(d.doss_list));
	if (rc != SPWERR_OK) return;

	d.row_cnt = d.doss_list->cnt;
	if (!d.row_cnt) return;

	for (i=0; i<d.doss_list->cnt; i++) {
		NODE_DATA	node;
		NODE_IDX	idx;

		p = d.doss_list->list[i];

		cmt = QString (p->info.comment);
		int nl = cmt.indexOf('\n'); if (nl != -1) cmt.truncate(nl);
		node.data	<< p->filename
				<< SPWAW_gametype2str(p->info.gametype)
				<< SPWAW_dossiertype2str(p->info.type)
				<< ((p->info.type == SPWAW_CAMPAIGN_DOSSIER) ? SPWAW_oob_nation (p->info.gametype, p->info.props.OOB) : "")
				<< p->info.btlcnt
				<< p->info.name
				<< cmt;
		node.node = p;

		data_list.append (node);
		idx.ptr = &(data_list[i]);
		data_idx.append (idx);
	}

	DBG_TRACE_FLEAVE;
}

void
ModelDossList::freeModelData (void)
{
	data_idx.clear();
	data_list.clear();
	SPWAW_dosslist_free (&d.doss_list);
}

QString
ModelDossList::sort_transform (NODE_DATA *ptr, int col, int idx)
{
	QString	col1, col2, col3, col4, col5, col6;
	QString	out;

	col1 = ptr->data[1].toString();
	col2 = ptr->data[2].toString();
	col3 = ptr->data[3].toString();
	col4.sprintf ("%06.6u", ptr->data[4].toUInt());
	col5 = ptr->data[5].toString();
	col6 = ptr->data[6].toString();

	switch (col) {
		case 0:
			// col0 explicitely added to end of string later on
			break;
		case 1:
			out = col1 + col2 + col3 + col4 + col5 + col6;
			break;
		case 2:
			out = col2 + col1 + col3 + col4 + col5 + col6;
			break;
		case 3:
			out = col3 + col1 + col2 + col4 + col5  + col6;
			break;
		case 4:
			out = col4 + col1 + col2 + col3 + col5  + col6;
			break;
		case 5:
			out = col5 + col1 + col2 + col3 + col4 + col6;
			break;
		case 6:
			out = col6 + col1 + col2 + col3 + col4 + col5;
			break;
		default:
			// safe default
			out = col1 + col2 + col3 + col4 + col5 + col6;
			break;
	}
	out += ptr->data[0].toString();
	out += QString().setNum (idx);

	return (out);
}

void
ModelDossList::sort (int column, Qt::SortOrder order)
{
	QMap<QString, NODE_IDX>	map;
	unsigned long		i;

	map.clear();

	emit layoutAboutToBeChanged ();

	for (i=0; i<d.row_cnt; i++)
		map.insert (sort_transform (data_idx[i].ptr, column, i), data_idx[i]);

	data_idx = map.values();

	if (order == Qt::DescendingOrder) {
		for (i=0; i<d.row_cnt/2; i++) data_idx.swap (i, d.row_cnt-i-1);
	}

	emit layoutChanged ();
}
