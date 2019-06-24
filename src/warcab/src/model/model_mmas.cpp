/** \file
 * The SPWaW war cabinet - data model handling - min-max-average-spread data.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "model_mmas.h"
#include "model_mmas_data.h"

typedef enum e_FILTER_ID {
	FILTER_ID_NONE = 0,
	FILTER_ID_RANK,		// ENUM (SPWAW_RLASTCODE+1)
	FILTER_ID_EXP,		// ENUM (SPWAW_ELASTCODE+1)
	FILTER_ID_CLASS,	// ENUM (SPWOOB_UCLASS_LIMIT+1)
	FILTER_ID_LIMIT = FILTER_ID_CLASS
} FILTER_ID;

#define	FILTER_ID_CNT	(FILTER_ID_LIMIT+1)

static MDL_MF filterdef[FILTER_ID_CNT] = {
	{ FILTER_ID_NONE,	"show all"	},
	{ FILTER_ID_RANK,	"rank"		},
	{ FILTER_ID_EXP,	"experience"	},
	{ FILTER_ID_CLASS,	"unit class"	},
};

static MDL_MF *filterdef_cache[FILTER_ID_CNT] = { 0 };

static MDL_MF *
filterdef_lookup (MDL_MFID id)
{
	MDL_MF	*p;

	if (id >= FILTER_ID_CNT) return (NULL);

	if (p = filterdef_cache[id]) return (p);

	for (int i=0; i<FILTER_ID_CNT; i++)
		if (filterdef[i].id == id) { p = &(filterdef[i]); break; }

	return (filterdef_cache[id] = p);
}

typedef struct s_MDLMMAS_COLUMN_DEF_PARTIAL {
	bool		isint;
	unsigned int	item;
	unsigned int	base[FILTER_ID_CNT];
	unsigned int	member[MDLMMAS_COLUMN_CNT-1];
} MDLMMAS_COLUMN_DEF_PARTIAL;

#define	OFFSB(m_)	offsetof(SPWAW_SNAP_OOB_FORCE,m_)
#define	OFFSI(m_)	offsetof(SPWAW_IMMAS,m_)
#define	OFFSF(m_)	offsetof(SPWAW_FMMAS,m_)

static MDLMMAS_COLUMN_DEF coldef[MDLMMAS_COLUMN_CNT] = {
	{ MDLMMAS_COLUMN_DATE,	"Date",		SPWDLT_NONE,	0,	0,	0	},
	{ MDLMMAS_COLUMN_MAX,	"Maximum",	SPWDLT_INT,	0,	0,	0	},
	{ MDLMMAS_COLUMN_MAXID,	"Maximum ID",	SPWDLT_INT,	0,	0,	0	},
	{ MDLMMAS_COLUMN_MIN,	"Minimum",	SPWDLT_INT,	0,	0,	0	},
	{ MDLMMAS_COLUMN_MINID,	"Minimum ID",	SPWDLT_INT,	0,	0,	0	},
	{ MDLMMAS_COLUMN_AVG,	"Average",	SPWDLT_DBL,	0,	0,	0	},
	{ MDLMMAS_COLUMN_SPR,	"Spread",	SPWDLT_INT,	0,	0,	0	},
	{ MDLMMAS_COLUMN_COUNT,	"Count",	SPWDLT_INT,	0,	0,	0	},
};

static MDLMMAS_COLUMN_DEF_PARTIAL coldef_exp = { true, sizeof (SPWAW_IMMAS),
	OFFSB(attr.mmas_uexp.all), OFFSB(attr.mmas_uexp.rnk), OFFSB(attr.mmas_uexp.exp), OFFSB(attr.mmas_uexp.cls),
	OFFSI(max.v), OFFSI(max.i), OFFSI(min.v), OFFSI(min.i), OFFSI(avg), OFFSI(spr), OFFSI(cnt) };

static MDLMMAS_COLUMN_DEF_PARTIAL coldef_mor = { true, sizeof (SPWAW_IMMAS),
	OFFSB(attr.mmas_umor.all), OFFSB(attr.mmas_umor.rnk), OFFSB(attr.mmas_umor.exp), OFFSB(attr.mmas_umor.cls),
	OFFSI(max.v), OFFSI(max.i), OFFSI(min.v), OFFSI(min.i), OFFSI(avg), OFFSI(spr), OFFSI(cnt) };

static MDLMMAS_COLUMN_DEF_PARTIAL coldef_sup = { true, sizeof (SPWAW_IMMAS),
	OFFSB(attr.mmas_usup.all), OFFSB(attr.mmas_usup.rnk), OFFSB(attr.mmas_usup.exp), OFFSB(attr.mmas_usup.cls),
	OFFSI(max.v), OFFSI(max.i), OFFSI(min.v), OFFSI(min.i), OFFSI(avg), OFFSI(spr), OFFSI(cnt) };

static MDLMMAS_COLUMN_DEF_PARTIAL coldef_ral = { true, sizeof (SPWAW_IMMAS),
	OFFSB(attr.mmas_ural.all), OFFSB(attr.mmas_ural.rnk), OFFSB(attr.mmas_ural.exp), OFFSB(attr.mmas_ural.cls),
	OFFSI(max.v), OFFSI(max.i), OFFSI(min.v), OFFSI(min.i), OFFSI(avg), OFFSI(spr), OFFSI(cnt) };

static MDLMMAS_COLUMN_DEF_PARTIAL coldef_inf = { true, sizeof (SPWAW_IMMAS),
	OFFSB(attr.mmas_uinf.all), OFFSB(attr.mmas_uinf.rnk), OFFSB(attr.mmas_uinf.exp), OFFSB(attr.mmas_uinf.cls),
	OFFSI(max.v), OFFSI(max.i), OFFSI(min.v), OFFSI(min.i), OFFSI(avg), OFFSI(spr), OFFSI(cnt) };

static MDLMMAS_COLUMN_DEF_PARTIAL coldef_arm = { true, sizeof (SPWAW_IMMAS),
	OFFSB(attr.mmas_uarm.all), OFFSB(attr.mmas_uarm.rnk), OFFSB(attr.mmas_uarm.exp), OFFSB(attr.mmas_uarm.cls),
	OFFSI(max.v), OFFSI(max.i), OFFSI(min.v), OFFSI(min.i), OFFSI(avg), OFFSI(spr), OFFSI(cnt) };

static MDLMMAS_COLUMN_DEF_PARTIAL coldef_art = { true, sizeof (SPWAW_IMMAS),
	OFFSB(attr.mmas_uart.all), OFFSB(attr.mmas_uart.rnk), OFFSB(attr.mmas_uart.exp), OFFSB(attr.mmas_uart.cls),
	OFFSI(max.v), OFFSI(max.i), OFFSI(min.v), OFFSI(min.i), OFFSI(avg), OFFSI(spr), OFFSI(cnt) };

static MDLMMAS_COLUMN_DEF_PARTIAL coldef_ready = { false, sizeof (SPWAW_FMMAS),
	OFFSB(attr.mmas_uready.all), OFFSB(attr.mmas_uready.rnk), OFFSB(attr.mmas_uready.exp), OFFSB(attr.mmas_uready.cls),
	OFFSF(max.v), OFFSF(max.i), OFFSF(min.v), OFFSF(min.i), OFFSF(avg), OFFSF(spr), OFFSF(cnt) };

static MDLMMAS_COLUMN_DEF_PARTIAL coldef_kills = { true, sizeof (SPWAW_IMMAS),
	OFFSB(attr.mmas_ukills.all), OFFSB(attr.mmas_ukills.rnk), OFFSB(attr.mmas_ukills.exp), OFFSB(attr.mmas_ukills.cls),
	OFFSI(max.v), OFFSI(max.i), OFFSI(min.v), OFFSI(min.i), OFFSI(avg), OFFSI(spr), OFFSI(cnt) };

static MDLMMAS_COLUMN_DEF_PARTIAL coldef_cost = { true, sizeof (SPWAW_IMMAS),
	OFFSB(attr.mmas_ucost.all), OFFSB(attr.mmas_ucost.rnk), OFFSB(attr.mmas_ucost.exp), OFFSB(attr.mmas_ucost.cls),
	OFFSI(max.v), OFFSI(max.i), OFFSI(min.v), OFFSI(min.i), OFFSI(avg), OFFSI(spr), OFFSI(cnt) };

static MDLMMAS_COLUMN_DEF_PARTIAL coldef_speed = { true, sizeof (SPWAW_IMMAS),
	OFFSB(attr.mmas_uspeed.all), OFFSB(attr.mmas_uspeed.rnk), OFFSB(attr.mmas_uspeed.exp), OFFSB(attr.mmas_uspeed.cls),
	OFFSI(max.v), OFFSI(max.i), OFFSI(min.v), OFFSI(min.i), OFFSI(avg), OFFSI(spr), OFFSI(cnt) };

static MDLMMAS_COLUMN_DEF_PARTIAL *coldef_partial[MDLMMAS_TYPE_CNT] = {
	&coldef_exp,
	&coldef_mor,
	&coldef_sup,
	&coldef_ral,
	&coldef_inf,
	&coldef_arm,
	&coldef_art,
	&coldef_ready,
	&coldef_kills,
	&coldef_cost,
	&coldef_speed,
};

static MDLMMAS_COLUMN_DEF *coldef_cache[MDLMMAS_COLUMN_CNT] = { 0 };

MDLMMAS_COLUMN_DEF *
MDLMMAS_coldef (int col)
{
	MDLMMAS_COLUMN_DEF	*p;

	if ((col < 0) || (col > MDLMMAS_COLUMN_LAST)) return (NULL);

	if (p = coldef_cache[col]) return (p);

	for (int i=0; i<MDLMMAS_COLUMN_CNT; i++)
		if (coldef[i].id == col) { p = &(coldef[i]); break; }

	return (coldef_cache[col] = p);
}

static void
MDLMMAS_coldef_update (MDLMMAS_TYPE type, MDL_MFID fid)
{
	int				i;
	MDLMMAS_COLUMN_DEF_PARTIAL	*src = NULL;

	src = coldef_partial[type];

	for (i=1; i<MDLMMAS_COLUMN_CNT; i++) {
		switch ((MDLMMAS_COLUMN)i) {
			case MDLMMAS_COLUMN_AVG:
				coldef[i].dtype = SPWDLT_DBL;
				break;
			case MDLMMAS_COLUMN_COUNT:
				coldef[i].dtype = SPWDLT_INT;
				break;
			default:
				coldef[i].dtype = src->isint ? SPWDLT_INT : SPWDLT_DBL;
				break;
		}
		coldef[i].base = src->base[fid];
		coldef[i].item = src->item;
		coldef[i].member = src->member[i-1];
	}
}

ModelMMAS::ModelMMAS (QObject *parent)
	: QAbstractTableModel (parent)
{
	int	i;

	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.col_cnt = MDLMMAS_COLUMN_CNT;
	for (i=0; i<d.col_cnt; i++) header << MDLMMAS_coldef(i)->name;

	setup_filter_data();

	d.tflag = MDLMMAS_TYPE_EXP;
	d.campaign = d.pflag = d.cflag = d.pvcmp = true;

	clear();
}

ModelMMAS::~ModelMMAS (void)
{
	DBG_TRACE_DESTRUCT;

	freeModelData(true);

	free_filter_data();
}

QVariant
ModelMMAS::data (const QModelIndex &index, int role) const
{
	if (!d.list) return (QVariant());
	if (!index.isValid()) return (QVariant());

	return (MDLMMAS_data (role, index.row(), index.column()));
}

Qt::ItemFlags
ModelMMAS::flags (const QModelIndex &index) const
{
	if (!index.isValid()) return (0);

	return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

QVariant
ModelMMAS::headerData (int section, Qt::Orientation orientation, int role) const
{
	if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return (QVariant());

	return (header.value (section));
}

int
ModelMMAS::rowCount (const QModelIndex &/*parent*/) const
{
	return (d.row_cnt);
}

int
ModelMMAS::columnCount (const QModelIndex &/*parent*/) const
{
	return (d.col_cnt);
}

void
ModelMMAS::setupModelData (void)
{
	bool			newlist = true;
	int			i, j;
	MDLMMAS_DATA		*md;
	SPWAW_SNAP_OOB_FORCE	*curr, *base;
	SPWDLT			*dlt;
	unsigned int		boffs, ioffs, moffs;

	DBG_TRACE_FENTER;

	freeModelData (false);
	if (d.campaign) {
		if (!d.dptr.d || !d.pflag) return;
		if (!(d.row_cnt = d.dptr.d->bcnt)) return;

		if (d.list_cnt == d.row_cnt) newlist = false;
	} else {
		if (!d.dptr.b) return;
		if (!(d.row_cnt = d.dptr.b->tcnt)) return;

		if ((d.list_ref == d.dptr.b) && (d.list_cnt == d.row_cnt)) newlist = false;
	}

	/* Free data storage if required */
	if (newlist) {
		if (d.list) SL_SAFE_FREE (d.list);
		if (d.dlts) SL_SAFE_FREE (d.dlts);
	}

	/* Allocate data storage if required */
	if (!d.list) {
		d.list_ref = d.campaign ? NULL : d.dptr.b; d.list_cnt = d.row_cnt;
		SL_SAFE_CALLOC (d.list, d.list_cnt, sizeof (MDLMMAS_DATA));
		SL_SAFE_CALLOC (d.dlts, d.list_cnt * d.col_cnt, sizeof (SPWDLT));

		/* Fill data */
		for (i=0; i<d.list_cnt; i++) {
			md = &(d.list[i]);

			md->idx	 = i;
			if (d.campaign) {
				md->date.bdate = d.dptr.d->blist[i]->bdate;
			} else {
                                md->date.tdate = d.dptr.b->tlist[i]->tdate;
			}
			md->dlt	 = &(d.dlts[i*d.col_cnt]);
		}
	}

	/* Update coldef */
	MDLMMAS_coldef_update (d.tflag, d.filter_id);

	/* Update data */
	base = NULL;
	for (i=0; i<d.list_cnt; i++) {
		md = &(d.list[i]);

		if (d.campaign) {
			curr = &(d.dptr.d->blist[i]->snap->OOBp1.core);
		} else {
			if (d.cflag) {
				curr = d.pflag ? &(d.dptr.b->tlist[i]->snap->OOBp1.core) : &(d.dptr.b->tlist[i]->snap->OOBp2.battle);
			} else {
				curr = d.pflag ? &(d.dptr.b->tlist[i]->snap->OOBp1.support) : &(d.dptr.b->tlist[i]->snap->OOBp2.battle);
			}
		}
		md->force = curr;

		if (!base) base = curr;

		for (j=0; j<d.col_cnt; j++) {
			dlt = &(md->dlt[j]);

			boffs = MDLMMAS_coldef(j)->base;
			ioffs = d.filter_id ? (MDLMMAS_coldef(j)->item * d.target_id) : 0;
			moffs = MDLMMAS_coldef(j)->member;
			SPWDLT_prep (dlt, MDLMMAS_coldef(j)->dtype, curr, base, boffs + ioffs + moffs);
		}
		if (d.pvcmp) {
			base = curr;
		}
	}

	DBG_TRACE_FLEAVE;
}

void
ModelMMAS::freeModelData (bool all)
{
	if (all) {
		if (d.dlts) SL_SAFE_FREE (d.dlts);
		if (d.list) SL_SAFE_FREE (d.list); d.list_cnt = 0;
	}

	d.row_cnt = 0;
}

void
ModelMMAS::clear (void)
{
	d.dptr.d = NULL; d.tflag = MDLMMAS_TYPE_EXP;
	d.campaign = d.pflag = d.cflag = d.pvcmp = true;

	setupModelData();
	reset();
}

void
ModelMMAS::load (SPWAW_DOSSIER *dossier, MDLMMAS_TYPE type, bool player, bool prevcmp)
{
	d.dptr.d	= dossier;
	d.tflag		= type;
	d.campaign	= true;
	d.pflag		= player;
	d.cflag		= true;
	d.pvcmp		= prevcmp;

	setupModelData();
	reset();
}

void
ModelMMAS::load (SPWAW_BATTLE *battle, MDLMMAS_TYPE type, bool player, bool iscore, bool prevcmp)
{
	d.dptr.b	= battle;
	d.tflag		= type;
	d.campaign	= false;
	d.pflag		= player;
	d.cflag		= iscore;
	d.pvcmp		= prevcmp;

	setupModelData();
	reset();
}

int
ModelMMAS::max_width (int column)
{
	int	max = 0;

	for (int i=0; i<d.row_cnt; i++) {
		int w = (MDLMMAS_data (Qt::DisplayRole, i, column)).toString().length();
		if (w > max) max = w;
	}
	return (max);
}

void
ModelMMAS::refresh (void)
{
	//DBG_TRACE_FENTER;
	//DBG_TRACE_FLEAVE;
}

char *
ModelMMAS::title (void)
{
	return ((char *)MDLMMAS_TYPE_lookup (d.tflag));
}

void
ModelMMAS::column (MDLMMAS_COLUMN col, double *dst, int cnt)
{
	int		row;
	MDLMMAS_DATA	*data = NULL;
	SPWDLT		*dlt = NULL;

	if (!dst || !cnt) return;
	memset (dst, 0, sizeof (*dst) * cnt);

	if (cnt > d.row_cnt) cnt = d.row_cnt;
	for (row = 0; row < d.row_cnt; row++) {
		data = &(d.list[row]);
		if (!data->force || !data->dlt) continue;

		dlt = &(data->dlt[col]);
		dst[row] = MDLMMAS_data_plot (row, col, data, dlt);
	}
}

SPWAW_SNAP_OOB_UEL *
ModelMMAS::unitid (MDLMMAS_COLUMN col, int row)
{
	MDLMMAS_DATA		*data = NULL;
	SPWDLT			*dlt = NULL;
	SPWAW_SNAP_OOB_UEL	*v = NULL;

	if ((row < 0) || (row >= d.row_cnt)) return (v);

	data = &(d.list[row]);
	if (!data->force || !data->dlt) return (v);
	dlt = &(data->dlt[col]);

	return (MDLMMAS_data_id (row, col, data, dlt));
}

void
ModelMMAS::setup_filter_data (void)
{
	int	i, j;

	d.filter_list.cnt = FILTER_ID_LIMIT+1;
	d.filter_list.list = filterdef;

	SL_SAFE_CALLOC (d.target_list, d.filter_list.cnt, sizeof (MDL_MF_TGTLST *));
	for (i=FILTER_ID_RANK; i<FILTER_ID_CNT; i++) {
		MDL_MF_TGTLST_new (&(d.target_list[i]));
		switch (i) {
			case FILTER_ID_RANK:
				for (j=SPWAW_RGEN; j>=SPWAW_RPVT; j--) {
					MDL_MF_TGTLST_add (d.target_list[i], j, (char *)SPWAW_rank2str((SPWAW_RANK)j));
				}
				break;
			case FILTER_ID_EXP:
				for (j=SPWAW_ELASTCODE; j>=SPWAW_ESTARTCODE; j--) {
					MDL_MF_TGTLST_add (d.target_list[i], j, (char *)SPWAW_exp2str((SPWAW_EXP)j));
				}
				break;
			case FILTER_ID_CLASS:
				for (j=SPWOOB_UCLASS_INF; j<=SPWOOB_UCLASS_FRT; j++) {
					MDL_MF_TGTLST_add (d.target_list[i], j, (char *)SPWAW_oob_uclass((SPWOOB_UCLASS)j));
				}
				break;
			default:
				break;
		}
	}

	d.filter_id = d.target_id = 0;
}

void
ModelMMAS::free_filter_data (void)
{
	int	i;

	d.filter_id = d.target_id = 0;
	for (i=FILTER_ID_RANK; i<FILTER_ID_CNT; i++) {
		MDL_MF_TGTLST_free (&(d.target_list[i]));
	}
	SL_SAFE_FREE (d.target_list);
}

MDL_MFLST *
ModelMMAS::filters (void)
{
	return (&d.filter_list);
}

MDL_MF_TGTLST *
ModelMMAS::targets (MDL_MFID id)
{
	MDL_MF	*p = NULL;

	p = filterdef_lookup (id);
	if (!d.target_list || !p) return (NULL);

	return (d.target_list[p->id]);
}

void
ModelMMAS::filter (MDL_MFID fid, MDL_MF_TGTID tid)
{
	DBG_log ("[%s] (fid=%u, tid=%u)\n", __FUNCTION__, fid, tid);

	d.filter_id = fid;
	d.target_id = tid;

	setupModelData();
	reset();
}

bool
ModelMMAS::filtered (void)
{
	return (d.filter_id != 0);
}
