/** \file
 * The SPWaW war cabinet - data model handling - tabled data.
 *
 * Copyright (C) 2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_TABLE_H
#define	MODEL_TABLE_H	1

#include "model.h"

typedef enum e_MDLT_DATA_TYPE {
	MDLT_DATA_INT = 0,
	MDLT_DATA_DBL,
	MDLT_DATA_PERC,
	MDLT_DATA_STR
} MDLT_DATA_TYPE;

typedef struct s_MDLT_COLDEF {
	char		*name;
	MDLT_DATA_TYPE	type;
	int		width;
} MDLT_COLDEF;

typedef struct s_MDLT_DEF {
	int		col_cnt;
	MDLT_COLDEF	*col_lst;
} MDLT_DEF;

typedef struct s_MDLT_DATA_ROW {
	QVariant	*data;
	QColor		bg;
} MDLT_DATA_ROW;

typedef struct s_MDLT_DATA {
	int		row_cnt;
	int		col_cnt;
	MDLT_DATA_ROW	*row;
} MDLT_DATA;

class ModelTable	: public QAbstractTableModel
{
	Q_OBJECT

public:
	ModelTable	(QObject *parent = 0);
	~ModelTable	(void);

public:
	QVariant	data		(const QModelIndex &index, int role)					const;
	Qt::ItemFlags	flags		(const QModelIndex &index)						const;
	QVariant	headerData	(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)	const;
	int		rowCount	(const QModelIndex &parent = QModelIndex())				const;
	int		columnCount	(const QModelIndex &parent = QModelIndex())				const;

public:
	void		setup		(MDLT_DEF *def);
	void		load		(MDLT_DATA *dat);
	void		clear		(void);
	void		commit		(void);

public:
	MDLT_DEF	*def_info	(void);
	MDLT_COLDEF	*col_info	(int col);

signals:
	void		model_updated	(void);
	void		def_updated	(void);
	void		dat_updated	(void);

private:
	QList<QVariant>		header;
	struct s_data {
		MDLT_DEF	def;
		bool		updated_def;
		MDLT_DATA	dat;
		bool		updated_dat;
	} d;

private:
	void		free_def		(void);
	void		free_dat		(void);
	QVariant	MDLT_data		(int role, int row, int col)				const;
	QVariant	MDLT_data_display	(QVariant *item, MDLT_DATA_TYPE type)			const;
	QVariant	MDLT_data_foreground	(QVariant *item, MDLT_DATA_TYPE type)			const;
	QVariant	MDLT_data_background	(QVariant *item, MDLT_DATA_TYPE type, QColor col)	const;
};

extern void	MDLT_alloc_data	(MDLT_DATA &data, int row_cnt, int col_cnt);
extern void	MDLT_free_data	(MDLT_DATA &data);

#endif	/* MODEL_TABLE_H */
