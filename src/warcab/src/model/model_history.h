/** \file
 * The SPWaW war cabinet - data model handling - unit history.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_HISTORY_H
#define	MODEL_HISTORY_H	1

#include "model.h"
#include "model/model_history_data.h"

typedef enum e_MDLH_HILITE {
	MDLH_HILITE_NONE = 0,
	MDLH_HILITE_RANK,
	MDLH_HILITE_USTATUS,
	MDLH_HILITE_ENTR,
	MDLH_HILITE_EXP,
	MDLH_HILITE_LIMIT = MDLH_HILITE_EXP
} MDLH_HILITE;
extern const char *MDLH_HILITE_lookup (MDLH_HILITE e);

typedef struct s_MDLH_INFO {
	int			uidx;
	SPWAW_DOSSIER_BIR	*bir;
	int			bir_idx;
	int			bir_cnt;
} MDLH_INFO;

class ModelHistory	: public QAbstractTableModel
{
	Q_OBJECT

public:
	ModelHistory	(QObject *parent = 0);
	~ModelHistory	(void);

public:
	QVariant	data		(const QModelIndex &index, int role)					const;
	Qt::ItemFlags	flags		(const QModelIndex &index)						const;
	QVariant	headerData	(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)	const;
	int		rowCount	(const QModelIndex &parent = QModelIndex())				const;
	int		columnCount	(const QModelIndex &parent = QModelIndex())				const;

public:
	void	clear		(void);
	void	load		(SPWAW_DOSSIER *dossier, bool prevcmp, int unit);
	void	load		(SPWAW_BATTLE *battle, bool player, bool iscore, bool prevcmp, int unit);
	void	info		(MDLH_INFO &info);
	void	select		(USHORT uidx);
	void	highlight	(MDLH_HILITE h);
	int	max_width	(int column);
	void	refresh		(void);

private:
	QList<QVariant>		header;
	struct s_data {
		int			col_cnt;
		union u_dptr {
			SPWAW_DOSSIER	*d;
			SPWAW_BATTLE	*b;
		}	dptr;
		bool			campaign;
		bool			pflag;
		bool			cflag;
		bool			pvcmp;
		USHORT			uidx;
		int			row_cnt;
		SPWAW_DOSSIER_BIR	*bbir;
		int			bbir_idx;
		int			bbir_cnt;
		SPWAW_BATTLE		*list_ref;
		MDLH_DATA		*list;
		int			list_cnt;
		SPWDLT			*dlts;

		MDLH_HILITE		hilite;
	} d;

private:
	void		setupModelData		(void);
	void		setupModelData_campaign	(void);
	void		setupModelData_battle	(void);
	void		freeModelData		(bool all);

	QVariant	MDLH_data		(int role, int row, int col)			const;
	QVariant	MDLH_data_display	(int row, int col, MDLH_DATA *data, SPWDLT *dlt)	const;
	QVariant	MDLH_data_foreground	(int row, int col, MDLH_DATA *data, SPWDLT *dlt)	const;
	QVariant	MDLH_data_background	(int row, int col, MDLH_DATA *data, SPWDLT *dlt)	const;
	QVariant	MDLH_data_decoration	(int row, int col, MDLH_DATA *data, SPWDLT *dlt)	const;
};

#endif	/* MODEL_HISTORY_H */
