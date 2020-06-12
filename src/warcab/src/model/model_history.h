/** \file
 * The SPWaW war cabinet - data model handling - unit history.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
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

typedef struct s_MDLH_IDENTITY {
	SPWAW_DOSSIER_UIR	*uir;
	SPWAW_UHTE		*uhte;
} MDLH_IDENTITY;

class ModelHistory	: public QAbstractTableModel
{
	Q_OBJECT

public:
	ModelHistory	(QFont *rgfont, QFont *dcfont, QObject *parent = 0);
	~ModelHistory	(void);

public:
	QVariant	data		(const QModelIndex &index, int role)					const;
	Qt::ItemFlags	flags		(const QModelIndex &index)						const;
	QVariant	headerData	(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)	const;
	QModelIndex	index		(int row, int column, const QModelIndex &parent = QModelIndex())	const;
	int		rowCount	(const QModelIndex &parent = QModelIndex())				const;
	int		columnCount	(const QModelIndex &parent = QModelIndex())				const;

public:
	void	clear		(void);
	void	load		(SPWAW_DOSSIER *dossier, bool prevcmp, int uidx, INTEL_MODE mode);
	void	load		(SPWAW_BATTLE *battle, bool isplayer, bool iscore, bool prevcmp, int uidx, INTEL_MODE mode);
	void	set_marking	(bool mark);
	void	select		(USHORT uidx);
	void	identity	(MDLH_IDENTITY &id);
	void	highlight	(MDLH_HILITE h);
	int	max_width	(int column);
	void	refresh		(void);

public slots:
	void	intel_mode_set	(INTEL_MODE mode);

private:
	QList<QVariant>		header;
	struct s_data {
		QFont			*rgfont;
		QFont			*dcfont;
		int			col_cnt;
		union u_dptr {
			SPWAW_DOSSIER	*d;
			SPWAW_BATTLE	*b;
		}	dptr;
		bool			campaign;
		bool			pflag;
		bool			cflag;
		bool			mflag;
		bool			pvcmp;
		void			*dref;
		unsigned int		uidx;
		MDLH_IDENTITY		id;
		int			row_cnt;
		MDLH_DATA		*list;
		int			list_cnt;
		SPWDLT			*dlts;
		MDLH_HILITE		hilite;
		INTEL_MODE		intel_mode;
	} d;

private:
	void		setupModelDataStorage	(void);
	void		freeModelDataStorage	(void);
	void		addModelData		(unsigned int idx, SPWAW_UHTE *uhte, SPWAW_DOSSIER_UIR *uir, SPWAW_DOSSIER_UIR *buir);
	void		setupModelData		(void);
	void		freeModelData		(bool all);

	QVariant	MDLH_data		(int role, int row, int col)				const;
	QVariant	MDLH_data_display	(int row, int col, MDLH_DATA *data, SPWDLT *dlt)	const;
	QVariant	MDLH_data_font		(int row, int col, MDLH_DATA *data, SPWDLT *dlt)	const;
	QVariant	MDLH_data_foreground	(int row, int col, MDLH_DATA *data, SPWDLT *dlt)	const;
	QVariant	MDLH_data_background	(int row, int col, MDLH_DATA *data, SPWDLT *dlt)	const;
	QVariant	MDLH_data_decoration	(int row, int col, MDLH_DATA *data, SPWDLT *dlt)	const;
};

#endif	/* MODEL_HISTORY_H */
