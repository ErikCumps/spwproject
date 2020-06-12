/** \file
 * The SPWaW war cabinet - data model handling - unit data.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_ROSTER_H
#define	MODEL_ROSTER_H	1

#include "model.h"
#include "model_filter.h"
#include "model_roster_data.h"
#include "model_roster_sort.h"
#include "util/util.h"

typedef enum e_MDLR_FILTER {
	MDLR_FILTER_NONE = 0,
	MDLR_FILTER_CLASS,	// ENUM (SPWOOB_UCLASS_LIMIT+1)
	MDLR_FILTER_TYPE,	// ENUM (SPWOOB_UTYPE_LIMIT+1)
	MLDR_FILTER_NAME,	// string
	MDLR_FILTER_RANK,	// ENUM (SPWAW_RLASTCODE+1)
	MDLR_FILTER_EXP,	// ENUM (SPWAW_ELASTCODE+1)
	MDLR_FILTER_USTATUS,	// ENUM (SPWAW_ULASTCODE+1)
	MDLR_FILTER_ENTR,	// ENUM (SPWAW_ENTRLASTCODE+1)
	MDLR_FILTER_RDY,	// boolean (100% or not)
	MDLR_FILTER_SUP,	// boolean (0 or not)
	MDLR_FILTER_KIA,	// boolean (0 or not)
	MDLR_FILTER_DMG,	// boolean (0 or not)
	MDLR_FILTER_ABAND,	// ENUM (SPWAW_ALASTCODE+1)
	MDLR_FILTER_SEEN,	// boolean (spotted or not)
	MDLR_FILTER_LOADED,	// boolean (loaded or not)
	MDLR_FILTER_LIMIT = MDLR_FILTER_LOADED
} MDLR_FILTER;
extern const char *MDLR_FILTER_lookup (MDLR_FILTER e);

typedef enum e_MDLR_HILITE {
	MDLR_HILITE_NONE = 0,
	MDLR_HILITE_RANK,
	MDLR_HILITE_USTATUS,
	MDLR_HILITE_ENTR,
	MDLR_HILITE_EXP,
	MDLR_HILITE_LIMIT = MDLR_HILITE_EXP
} MDLR_HILITE;
extern const char *MDLR_HILITE_lookup (MDLR_HILITE e);

typedef struct s_ModelRosterRawData {
	int			idx;
	SPWAW_UHTE		*uhte;
	bool			decomm;
	SPWAW_DOSSIER_UIR	*uir;
	SPWDLT			*dlt;
} ModelRosterRawData;

class ModelRoster	: public QAbstractTableModel
{
	Q_OBJECT

public:
	ModelRoster	(QFont *rgfont = NULL, QFont *dcfont = NULL, QObject *parent = 0);
	~ModelRoster	(void);

public:
	QVariant	data		(const QModelIndex &index, int role)					const;
	Qt::ItemFlags	flags		(const QModelIndex &index)						const;
	QVariant	headerData	(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)	const;
	QModelIndex	index		(int row, int column, const QModelIndex &parent = QModelIndex())	const;
	int		rowCount	(const QModelIndex &parent = QModelIndex())				const;
	int		columnCount	(const QModelIndex &parent = QModelIndex())				const;
	void		sort		(int column, Qt::SortOrder order = Qt::AscendingOrder);
	void		resort		(void);

public:
	void	clear		(void);
	void	load		(SPWAW_DOSSIER *dossier, bool fch, INTEL_MODE mode);
	void	load		(SPWAW_BATTLE *current, SPWAW_BATTLE *start, bool isplayer, bool iscore, INTEL_MODE mode);
	void	load		(SPWAW_BTURN *current, SPWAW_BTURN *start, bool isplayer, bool iscore, INTEL_MODE mode);
	void	set_marking	(bool mark);
	void	highlight	(MDLR_HILITE h);
	int	max_width	(int column);
	void	refresh		(void);
	void	set_dltsort	(bool sort);
	int	rawdata		(int row, MDLR_COLUMN col, ModelRosterRawData *data, int cnt = 1);
	void	filter_targets	(MDLR_FILTER spec, xMDL_FILTER_LIST *tgt);

public slots:
	void	intel_mode_set	(INTEL_MODE mode);

private:
	QList<QVariant>		header;
	struct s_data {
		QFont			*rgfont;
		QFont			*dcfont;
		int			col_cnt;
		SPWAW_DOSSIER_BIR	*birs;
		int			birs_cnt;
		SPWAW_DOSSIER_BIR	*cbrs;
		int			cbrs_cnt;
		SPWAW_DOSSIER_BIR	*rbrs;
		int			rbrs_cnt;
		SPWAW_DOSSIER		*d;
		bool			fchflag;
		SPWAW_BATTLE		*cb;
		SPWAW_BATTLE		*bb;
		bool			tflag;
		bool			pflag;
		bool			cflag;
		bool			mflag;
		int			row_cnt;
		MDLR_DATA		*list;
		int			list_cnt;
		int			list_use;
		SPWDLT			*dlts;
		MDLR_SMAP		*fsmap;
		int			fsmap_cnt;
		MDLR_SMAP		*ksmap;
		int			ksmap_cnt;
		MDLR_SMAP		*smap;
		int			smap_cnt;
		MDLR_HILITE		hilite;
		bool			dltsort;
		bool			revsort;
		INTEL_MODE		intel_mode;
		int			scol;
		int			sord;
	} d;

private:
	void		setupModelDataStorage	(void);
	void		freeModelDataStorage	(void);
	void		setupSortMap		(void);
	void		selectSortMap		(void);
	void		addModelData		(SPWAW_UHTE *uhte, SPWAW_DOSSIER_UIR *uir, SPWAW_DOSSIER_UIR *cuir, SPWAW_DOSSIER_UIR *ruir);
	void		setupModelData		(void);
	void		freeModelData		(bool all);

	QVariant	MDLR_data		(int role, int row, int col)				const;
	QVariant	MDLR_data_display	(int row, int col, MDLR_DATA *data, SPWDLT *dlt)	const;
	QVariant	MDLR_data_font		(int row, int col, MDLR_DATA *data, SPWDLT *dlt)	const;
	QVariant	MDLR_data_foreground	(int row, int col, MDLR_DATA *data, SPWDLT *dlt)	const;
	QVariant	MDLR_data_background	(int row, int col, MDLR_DATA *data, SPWDLT *dlt)	const;
	QVariant	MDLR_data_decoration	(int row, int col, MDLR_DATA *data, SPWDLT *dlt)	const;
};

#endif	/* MODEL_ROSTER_H */
