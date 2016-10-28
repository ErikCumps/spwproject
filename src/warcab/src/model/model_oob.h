/** \file
 * The SPWaW war cabinet - data model handling - order of battle data.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_OOB_H
#define	MODEL_OOB_H	1

#include "model.h"
#include "model/model_oob_data.h"
#include "model/model_oob_sort.h"

typedef enum e_MDLO_HILITE {
	MDLO_HILITE_NONE = 0,
	MDLO_HILITE_TYPE,
	MDLO_HILITE_LIMIT = MDLO_HILITE_TYPE
} MDLO_HILITE;
extern const char *MDLO_HILITE_lookup (MDLO_HILITE e);

class ModelOob	: public QAbstractItemModel
{
	Q_OBJECT

public:
	ModelOob	(QObject *parent = 0);
	~ModelOob	(void);

public:
	QVariant	data		(const QModelIndex &index, int role)					const;
	Qt::ItemFlags	flags		(const QModelIndex &index)						const;
	QVariant	headerData	(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)	const;
	QModelIndex	index		(int row, int column, const QModelIndex &parent = QModelIndex())	const;
	QModelIndex	parent		(const QModelIndex &index)						const;
	int		rowCount	(const QModelIndex &parent = QModelIndex())				const;
	int		columnCount	(const QModelIndex &parent = QModelIndex())				const;
	void		sort		(int column, Qt::SortOrder order = Qt::AscendingOrder);
	void		resort		(void);

public:
	void	clear		(void);
	void	load		(SPWAW_BATTLE *current, SPWAW_BATTLE *start, bool isplayer, bool iscore);
	void	load		(SPWAW_BATTLE *battle, int current, int start, bool isplayer, bool iscore);
	void	load		(SPWAW_BTURN *current, SPWAW_BTURN *start, bool isplayer, bool iscore);
	void	highlight	(MDLO_HILITE h);
	int	max_width	(int column);
	void	refresh		(void);
	void	set_expanded	(const QModelIndex &index, bool flag);
	bool	is_expanded	(const QModelIndex &index);
	void	set_dltsort	(bool sort);

private:
	QList<QVariant>		header;
	struct s_data {
		int			col_cnt;
		SPWAW_DOSSIER_BIR	*birs;
		int			birs_idx;
		int			birs_cnt;
		SPWAW_DOSSIER_BIR	*base;
		int			base_idx;
		int			base_cnt;
		bool			pflag;
		int			row_cnt;
		MDLO_DATA		*tree;
		int			tree_cnt;
		SPWDLT			*dlts;
		MDLO_SMAP		*smap;
		int			smap_cnt;
		MDLO_EXPLIST		explist;

		int			hilite;
		bool			dltsort;
		bool			revsort;
		int			scol;
		int			sord;
	} d;

private:
	void		setupModelData	(void);
	void		freeModelData	(bool all);
	int		find		(MDLO_DATA *f)	const;
	void		smap_swap	(int i1, int i2);

	QVariant	MDLO_data			(int role, int col, MDLO_DATA *p)				const;
	QVariant	MDLO_data_form			(int role, int col, SPWAW_DOSSIER_FIR *fir, SPWDLT *dlt)	const;
	QVariant	MDLO_data_form_display		(int col, SPWAW_DOSSIER_FIR *fir, SPWDLT *dlt)			const;
	QVariant	MDLO_data_form_foreground	(int col, SPWAW_DOSSIER_FIR *fir, SPWDLT *dlt)			const;
	QVariant	MDLO_data_form_background	(int col, SPWAW_DOSSIER_FIR *fir, SPWDLT *dlt)			const;
	QVariant	MDLO_data_form_decoration	(int col, SPWAW_DOSSIER_FIR *fir, SPWDLT *dlt)			const;
	QVariant	MDLO_data_unit			(int role, int col, SPWAW_DOSSIER_UIR *uir, SPWDLT *dlt)	const;
	QVariant	MDLO_data_unit_display		(int col, SPWAW_DOSSIER_UIR *uir, SPWDLT *dlt)			const;
	QVariant	MDLO_data_unit_foreground	(int col, SPWAW_DOSSIER_UIR *uir, SPWDLT *dlt)			const;
	QVariant	MDLO_data_unit_background	(int col, SPWAW_DOSSIER_UIR *uir, SPWDLT *dlt)			const;
	QVariant	MDLO_data_unit_decoration	(int col, SPWAW_DOSSIER_UIR *uir, SPWDLT *dlt)			const;
};

#endif	/* MODEL_OOB_H */
