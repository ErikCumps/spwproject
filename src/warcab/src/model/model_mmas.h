/** \file
 * The SPWaW war cabinet - data model handling - min-max-average-spread data.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	PLOT_MODEL_MMAS_H
#define	PLOT_MODEL_MMAS_H	1

#include "model.h"
#include "model/model_mmas_data.h"
#include "model/model_filter.h"

typedef enum e_MDLMMAS_TYPE {
	MDLMMAS_TYPE_EXP,
	MDLMMAS_TYPE_MOR,
	MDLMMAS_TYPE_SUP,
	MDLMMAS_TYPE_RAL,
	MDLMMAS_TYPE_INF,
	MDLMMAS_TYPE_ARM,
	MDLMMAS_TYPE_ART,
	MDLMMAS_TYPE_RDY,
	MDLMMAS_TYPE_KILLS,
	MDLMMAS_TYPE_COST,
	MDLMMAS_TYPE_SPEED,
	MDLMMAS_TYPE_LIMIT = MDLMMAS_TYPE_SPEED
} MDLMMAS_TYPE;
extern const char *MDLMMAS_TYPE_lookup (MDLMMAS_TYPE e);

#define	MDLMMAS_TYPE_CNT	(MDLMMAS_TYPE_LIMIT + 1)

class ModelMMAS	: public QAbstractTableModel
{
	Q_OBJECT

public:
	ModelMMAS	(QObject *parent = 0);
	~ModelMMAS	(void);

public:
	QVariant	data		(const QModelIndex &index, int role)					const;
	Qt::ItemFlags	flags		(const QModelIndex &index)						const;
	QVariant	headerData	(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)	const;
	int		rowCount	(const QModelIndex &parent = QModelIndex())				const;
	int		columnCount	(const QModelIndex &parent = QModelIndex())				const;

public:
	void			clear		(void);
	void			load		(SPWAW_DOSSIER *dossier, MDLMMAS_TYPE type, bool player, bool prevcmp);
	void			load		(SPWAW_BATTLE *battle, MDLMMAS_TYPE type, bool player, bool iscore, bool prevcmp);
	int			max_width	(int column);
	void			refresh		(void);
	char *			title		(void);
	void			column		(MDLMMAS_COLUMN col, double *dst, int cnt);
	SPWAW_SNAP_OOB_UEL *	unitid		(MDLMMAS_COLUMN col, int row);
	MDL_MFLST *		filters		(void);
	MDL_MF_TGTLST *		targets		(MDL_MFID id);
	void			filter		(MDL_MFID fid, MDL_MF_TGTID tid);
	bool			filtered	(void);

private:
	QList<QVariant>		header;
	struct s_data {
		int			col_cnt;
		MDL_MFLST		filter_list;
		MDL_MF_TGTLST		**target_list;
		MDL_MFID		filter_id;
		MDL_MF_TGTID		target_id;

		union u_dptr {
			SPWAW_DOSSIER	*d;
			SPWAW_BATTLE	*b;
		}	dptr;
		MDLMMAS_TYPE		tflag;
		bool			campaign;
		bool			pflag;
		bool			cflag;
		bool			pvcmp;
		int			row_cnt;
		SPWAW_BATTLE		*list_ref;
		MDLMMAS_DATA		*list;
		int			list_cnt;
		SPWDLT			*dlts;
	} d;

private:
	void		setupModelData	(void);
	void		freeModelData	(bool all);

	void		setup_filter_data	(void);
	void		free_filter_data	(void);

	QVariant	MDLMMAS_data		(int role, int row, int col)				const;
	QVariant	MDLMMAS_data_display	(int row, int col, MDLMMAS_DATA *data, SPWDLT *dlt)	const;
	QVariant	MDLMMAS_data_foreground	(int row, int col, MDLMMAS_DATA *data, SPWDLT *dlt)	const;
	QVariant	MDLMMAS_data_background	(int row, int col, MDLMMAS_DATA *data, SPWDLT *dlt)	const;

	double			MDLMMAS_data_plot	(int row, int col, MDLMMAS_DATA *data, SPWDLT *dlt)	const;
	SPWAW_SNAP_OOB_UEL *	MDLMMAS_data_id		(int row, int col, MDLMMAS_DATA *data, SPWDLT *dlt)	const;
};

#endif	/* PLOT_MODEL_MMAS_H */
