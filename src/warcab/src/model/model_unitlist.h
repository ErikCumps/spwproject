/** \file
 * The SPWaW war cabinet - data model handling - unit list.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_UNITLIST_H
#define	MODEL_UNITLIST_H	1

#include "model.h"
#include "model/model_unitlist_data.h"

class ModelUnitlist	: public QAbstractTableModel
{
	Q_OBJECT

public:
	ModelUnitlist	(QObject *parent = 0);
	~ModelUnitlist	(void);

public:
	QVariant	data		(const QModelIndex &index, int role)					const;
	Qt::ItemFlags	flags		(const QModelIndex &index)						const;
	QVariant	headerData	(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)	const;
	int		rowCount	(const QModelIndex &parent = QModelIndex())				const;
	int		columnCount	(const QModelIndex &parent = QModelIndex())				const;

public:
	void	load		(SPWAW_DOSSIER_BIR *bir, int cnt);
	void	clear		(void);
	int	max_width	(void);
	void	refresh		(void);

private:
	QList<QVariant>		header;
	struct s_data {
		SPWAW_DOSSIER_BIR	*data_bir;
		int			data_cnt;
	} d;

private:
	QVariant	MDLUL_data	(int role, int row, int col)	const;
};

#endif	/* MODEL_UNITLIST_H */
