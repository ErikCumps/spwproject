/** \file
 * The SPWaW war cabinet - data model handling - savegame list.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_SAVELIST_H
#define	MODEL_SAVELIST_H	1

#include "model.h"

class ModelSaveList	: public QAbstractItemModel
{
	Q_OBJECT

public:
	ModelSaveList	(SPWAW_GAME_TYPE gametype, char *path, SPWAW_SAVELIST *ignore, QObject *parent = 0);
	~ModelSaveList	(void);

public:
	QVariant	data		(const QModelIndex &index, int role)					const;
	Qt::ItemFlags	flags		(const QModelIndex &index)						const;
	QVariant	headerData	(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)	const;
	QModelIndex	index		(int row, int column, const QModelIndex &parent = QModelIndex())	const;
	QModelIndex	parent		(const QModelIndex &index)						const;
	int		rowCount	(const QModelIndex &parent = QModelIndex())				const;
	int		columnCount	(const QModelIndex &parent = QModelIndex())				const;
	void		sort		(int column, Qt::SortOrder order = Qt::AscendingOrder);

private:
	typedef struct s_NODE_DATA {
		QList<QVariant>		data;
		SPWAW_SAVELIST_NODE	*node;
	} NODE_DATA;

	typedef struct s_NODE_IDX {
		NODE_DATA		*ptr;
	} NODE_IDX;

	QList<QVariant>			header;
	QList<NODE_DATA>		data_list;
	QList<NODE_IDX>			data_idx;
	struct s_data {
		unsigned long			row_cnt;
		unsigned long			col_cnt;
		SPWAW_SAVELIST			*save_list;
	} d;

private:
	void	setupModelData	(SPWAW_GAME_TYPE gametype, char *path, SPWAW_SAVELIST *ignore);
	void	freeModelData	(void);
	QString	sort_transform	(NODE_DATA *ptr, int col, int idx);
};

#endif	/* MODEL_SAVELIST_H */
