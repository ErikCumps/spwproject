/** \file
 * The SPWaW war cabinet - data model handling - dossier.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_DOSSIER_H
#define	MODEL_DOSSIER_H	1

#include "warcab.h"
#include "model.h"

class ModelDossier	: public QAbstractItemModel
{
	Q_OBJECT

public:
	ModelDossier	(QObject *parent = 0);
	~ModelDossier	(void);

public:
	QVariant	data		(const QModelIndex &index, int role)					const;
	Qt::ItemFlags	flags		(const QModelIndex &index)						const;
	QVariant	headerData	(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)	const;
	QModelIndex	index		(int row, int column, const QModelIndex &parent = QModelIndex())	const;
	QModelIndex	index		(MDLD_TREE_ITEM *item)							const;
	QModelIndex	parent		(const QModelIndex &index)						const;
	int		rowCount	(const QModelIndex &parent = QModelIndex())				const;
	int		columnCount	(const QModelIndex &parent = QModelIndex())				const;
	void		sort		(int column, Qt::SortOrder order = Qt::AscendingOrder);
	//bool		removeRows	(int row, int count, const QModelIndex &parent = QModelIndex());
	MDLD_TREE_ITEM *item		(const QModelIndex &idx);

public:
	QModelIndex	root_index	(void);
	void		clear		(void);
	void		load		(MDLD_TREE_ITEM *tree);
	void		refresh		(void);
	void		set_selected	(MDLD_TREE_ITEM *item);
	void		set_cmpcurr	(MDLD_TREE_ITEM *item);
	void		set_cmpbase	(MDLD_TREE_ITEM *item);

private:
	QList<QVariant>		header;
	struct s_data {
		unsigned long		col_cnt;
		MDLD_TREE_ITEM		*tree;
		MDLD_TREE_ITEM		*selected;
		MDLD_TREE_ITEM		*cmpcurr;
		MDLD_TREE_ITEM		*cmpbase;
	} d;

private:
	QBrush		item_brush	(int role, MDLD_TREE_ITEM *item)	const;
	QVariant	data_dossier	(int role, MDLD_TREE_ITEM *p)		const;
	QVariant	data_battle	(int role, MDLD_TREE_ITEM *p)		const;
	QVariant	data_bturn	(int role, MDLD_TREE_ITEM *p)		const;
};

#endif	/* MODEL_DOSSIER_H */
