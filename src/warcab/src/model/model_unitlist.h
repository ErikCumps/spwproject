/** \file
 * The SPWaW war cabinet - data model handling - unit list.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_UNITLIST_H
#define	MODEL_UNITLIST_H	1

#include "model.h"
#include "model/model_unitlist_data.h"

#define	MDLU_UIDX_ROLE	Qt::UserRole

class ModelUnitlist	: public QAbstractTableModel
{
	Q_OBJECT

public:
	ModelUnitlist	(QFont *rgfont, QFont *dcfont, QObject *parent = 0);
	~ModelUnitlist	(void);

public:
	QVariant	data		(const QModelIndex &index, int role)					const;
	Qt::ItemFlags	flags		(const QModelIndex &index)						const;
	QVariant	headerData	(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)	const;
	int		rowCount	(const QModelIndex &parent = QModelIndex())				const;
	int		columnCount	(const QModelIndex &parent = QModelIndex())				const;

public:
	void	clear		(void);
	void	load		(SPWAW_DOSSIER *dossier, bool fch);
	void	load		(SPWAW_BATTLE *battle, bool isplayer, bool iscore);
	bool	dossier_mode	(void);

	int	max_width	(void);
	void	refresh		(void);

private:
	QList<QVariant>		header;
	struct s_data {
		QFont			*rgfont;
		QFont			*dcfont;
		SPWAW_DOSSIER		*d;
		bool			fchflag;
		SPWAW_BATTLE		*b;
		bool			pflag;
		bool			cflag;
		SPWAW_DOSSIER_BIR	*birs;
		int			birs_cnt;
		int			row_cnt;
		MDLU_DATA		*list;
		int			list_cnt;
		int			list_use;
	} d;

private:
	void		setupModelDataStorage	(void);
	void		freeModelDataStorage	(void);
	void		addModelData		(int uidx, SPWAW_UHTE *uhte, SPWAW_DOSSIER_UIR *uir);
	void		setupModelData		(void);
	void		freeModelData		(bool all);

	QVariant	MDLU_data		(int role, int row, int col)		const;
	QVariant	MDLU_data_display	(int row, int col, MDLU_DATA *data)	const;
	QVariant	MDLU_data_font		(int row, int col, MDLU_DATA *data)	const;
	QVariant	MDLU_data_foreground	(int row, int col, MDLU_DATA *data)	const;
	QVariant	MDLU_data_background	(int row, int col, MDLU_DATA *data)	const;
	QVariant	MDLU_data_decoration	(int row, int col, MDLU_DATA *data)	const;
};

#endif	/* MODEL_UNITLIST_H */
