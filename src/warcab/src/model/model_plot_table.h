/** \file
 * The SPWaW war cabinet - data model handling - plot table data.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_PLOT_TABLE_H
#define	MODEL_PLOT_TABLE_H	1

#include "model.h"
#include "plot/plot.h"

typedef enum e_MDLPT_DATA_TYPE {
	MDLPT_DATA_INT = 0,
	MDLPT_DATA_BDATE,
	MDLPT_DATA_TDATE,
} MDLPT_DATA_TYPE;

typedef struct s_MDLPT_COLDEF {
	char		*name;
	MDLPT_DATA_TYPE	type;
	int		width;
	QColor		*plot_color;
} MDLPT_COLDEF;

typedef struct s_MDLPT_DEF {
	char		*title;
	PLOT_TYPE	plot_type;
	bool		plot_stacked;
	AXIS_TYPE	axis_type;
	int		col_cnt;
	MDLPT_COLDEF	*col_lst;
} MDLPT_DEF;

typedef struct s_MDLPT_DATA_ITEM {
	MDLPT_DATA_TYPE			type;
	union u_u {
		SPWAW_BATTLE_DATE	bdate;
		SPWAW_TURN_DATE		tdate;
		int			i;
	} u;
	SPWDLT				dlt;
} MDLPT_DATA_ITEM;

typedef struct s_MDLPT_DATA_ROW {
	MDLPT_DATA_ITEM	*data;
} MDLPT_DATA_ROW;

typedef struct s_MDLPT_DATA {
	int		row_cnt;
	int		col_cnt;
	MDLPT_DATA_ROW	*row;
} MDLPT_DATA;

class ModelPlotTable	: public QAbstractTableModel
{
	Q_OBJECT

public:
	ModelPlotTable	(QObject *parent = 0);
	~ModelPlotTable	(void);

public:
	QVariant	data		(const QModelIndex &index, int role)					const;
	Qt::ItemFlags	flags		(const QModelIndex &index)						const;
	QVariant	headerData	(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)	const;
	int		rowCount	(const QModelIndex &parent = QModelIndex())				const;
	int		columnCount	(const QModelIndex &parent = QModelIndex())				const;

public:
	void		setup		(MDLPT_DEF *def);
	void		load		(MDLPT_DATA *data);
	void		clear		(void);
	void		commit		(void);

public:
	MDLPT_DEF	*def_info	(void);
	MDLPT_COLDEF	*col_info	(int col);
	void		plot_data	(int col, double *dst, int cnt);

signals:
	void		model_updated	(void);
	void		def_updated	(void);
	void		dat_updated	(void);

private:
	QList<QVariant>		header;
	struct s_data {
		MDLPT_DEF	def;
		bool		updated_def;
		MDLPT_DATA	dat;
		bool		updated_dat;
	} d;

private:
	void		free_def		(void);
	void		free_dat		(void);
	QVariant	MDLPT_data		(int role, int row, int col)	const;
	QVariant	MDLPT_data_display	(MDLPT_DATA_ITEM *item)		const;
	QVariant	MDLPT_data_foreground	(MDLPT_DATA_ITEM *item)		const;
	SPWDLT_TYPE	dlt_type		(MDLPT_DATA_TYPE type);
};

extern void	MDLPT_alloc_data	(MDLPT_DATA &data, int row_cnt, int col_cnt);
extern void	MDLPT_free_data		(MDLPT_DATA &data);

#endif	/* MODEL_PLOT_TABLE_H */
