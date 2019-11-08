/** \file
 * The SPWaW war cabinet - data model handling - unit history.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "model_history.h"

typedef struct s_MDLH_COLUMN_DEF {
	MDLH_COLUMN	id;
	const char	*name;
	SPWDLT_TYPE	dtype;
	unsigned int	doffs;
} MDLH_COLUMN_DEF;

#define	OFFS(m_)	offsetof(SPWAW_SNAP_OOB_UEL,m_)

static MDLH_COLUMN_DEF coldef[MDLH_COLUMN_CNT] = {
	{ MDLH_COLUMN_DATE,	"Date",		SPWDLT_NONE,	0			},
	{ MDLH_COLUMN_CFLAG,	"",		SPWDLT_NONE,	0			},
	{ MDLH_COLUMN_UID,	"#",		SPWDLT_CHR,	OFFS(strings.uid)	},
	{ MDLH_COLUMN_UNIT,	"Unit",		SPWDLT_STR,	OFFS(data.dname)	}, // FIXME: designation, actually
	{ MDLH_COLUMN_RNK,	"Rank",		SPWDLT_INT,	OFFS(data.brank)	},
	{ MDLH_COLUMN_LDR,	"Leader",	SPWDLT_STR,	OFFS(data.lname)	},
	{ MDLH_COLUMN_KILL,	"Kills",	SPWDLT_INT,	OFFS(attr.gen.kills)	},
	{ MDLH_COLUMN_EXP,	"Exp",		SPWDLT_INT,	OFFS(data.exp)		},
	{ MDLH_COLUMN_MOR,	"Mor",		SPWDLT_INT,	OFFS(data.mor)		},
	{ MDLH_COLUMN_RAL,	"Ral",		SPWDLT_INT,	OFFS(data.ral)		},
	{ MDLH_COLUMN_INF,	"Inf",		SPWDLT_INT,	OFFS(data.inf)		},
	{ MDLH_COLUMN_ARM,	"Arm",		SPWDLT_INT,	OFFS(data.arm)		},
	{ MDLH_COLUMN_ART,	"Art",		SPWDLT_INT,	OFFS(data.art)		},
	{ MDLH_COLUMN_TYPE,	"Type",		SPWDLT_INT,	OFFS(data.utype)	},
	{ MDLH_COLUMN_CLASS,	"Class",	SPWDLT_INT,	OFFS(data.uclass)	},
	{ MDLH_COLUMN_RDY,	"Ready",	SPWDLT_DBL,	OFFS(attr.gen.ready)	},
	{ MDLH_COLUMN_SUP,	"Sup",		SPWDLT_INT,	OFFS(data.sup)		},
	{ MDLH_COLUMN_STATUS,	"Status",	SPWDLT_INT,	OFFS(data.status)	},
	{ MDLH_COLUMN_SEEN,	"Spotted",	SPWDLT_BOOL,	OFFS(data.spotted)	},
	{ MDLH_COLUMN_ABAND,	"Abandoned",	SPWDLT_INT,	OFFS(data.aband)	},
	{ MDLH_COLUMN_LOADED,	"Loaded",	SPWDLT_BOOL,	OFFS(data.loaded)	},
	{ MDLH_COLUMN_DMG,	"Dmg",		SPWDLT_INT,	OFFS(data.damage)	},
	{ MDLH_COLUMN_COST,	"Cost",		SPWDLT_INT,	OFFS(data.cost)		},
	{ MDLH_COLUMN_SPEED,	"Speed",	SPWDLT_INT,	OFFS(data.speed)	},
};

static MDLH_COLUMN_DEF *coldef_cache[MDLH_COLUMN_CNT] = { 0 };

MDLH_COLUMN_DEF *
MDLH_coldef (int col)
{
	MDLH_COLUMN_DEF	*p;

	if ((col < 0) || (col > MDLH_COLUMN_LAST)) return (NULL);

	if (p = coldef_cache[col]) return (p);

	for (int i=0; i<MDLH_COLUMN_CNT; i++)
		if (coldef[i].id == col) { p = &(coldef[i]); break; }

	return (coldef_cache[col] = p);
}

ModelHistory::ModelHistory (QObject *parent)
	: QAbstractTableModel (parent)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.col_cnt = MDLH_COLUMN_CNT;
	for (int i=0; i<d.col_cnt; i++) header << MDLH_coldef(i)->name;

	d.campaign = d.pflag = d.cflag = d.pvcmp = true;

	clear();
}

ModelHistory::~ModelHistory (void)
{
	DBG_TRACE_DESTRUCT;

	freeModelData(true);
}

QVariant
ModelHistory::data (const QModelIndex &index, int role) const
{
	if (!d.list) return (QVariant());
	if (!index.isValid()) return (QVariant());

	return (MDLH_data (role, index.row(), index.column()));
}

Qt::ItemFlags
ModelHistory::flags (const QModelIndex &index) const
{
	if (!index.isValid()) return (0);

	return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

QVariant
ModelHistory::headerData (int section, Qt::Orientation orientation, int role) const
{
	if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return (QVariant());

	return (header.value (section));
}

int
ModelHistory::rowCount (const QModelIndex &/*parent*/) const
{
	return (d.row_cnt);
}

int
ModelHistory::columnCount (const QModelIndex &/*parent*/) const
{
	return (d.col_cnt);
}

void
ModelHistory::setupModelData_campaign (void)
{
	int			i, j;
	MDLH_DATA		*hd;
	SPWAW_DOSSIER_BIR	*bir, *base;
	SPWAW_DOSSIER_UIR	*buir;
	int			cidx, bidx, pidx;

	/* Check data availability */
	if (!d.dptr.d || (!(d.row_cnt = d.dptr.d->bcnt))) return;

	/* Setup bbir */
	d.bbir = &(d.dptr.d->bfirst->info_sob->pbir_core);
	d.bbir_cnt = d.dptr.d->props.iucnt;

	/* Check start unit index */
	if (d.uidx >= d.bbir_cnt) { d.uidx = (USHORT)-1; d.row_cnt = 0; return; }

	/* Free and/or allocate data storage if required */
	if (d.list_cnt != d.row_cnt) {
		if (d.list) SL_SAFE_FREE (d.list);
		if (d.dlts) SL_SAFE_FREE (d.dlts);
	}
	if (!d.list) {
		d.list_ref = NULL; d.list_cnt = d.row_cnt;
		SL_SAFE_CALLOC (d.list, d.list_cnt, sizeof (MDLH_DATA));
		SL_SAFE_CALLOC (d.dlts, d.list_cnt * d.col_cnt, sizeof (SPWDLT));

		/* Fill data */
		for (i=0; i<d.list_cnt; i++) {
			hd = &(d.list[i]);

			hd->idx		= i;
			hd->date.bdate	= d.dptr.d->blist[i]->bdate;
			hd->dlt		= &(d.dlts[i*d.col_cnt]);
			hd->cflag	= MDLH_CFLAG_NONE;
		}
	}

	/* Update data */
	base = NULL; cidx = d.uidx;
	for (i=0; i<d.list_cnt; i++) {
		hd = &(d.list[i]);

		bir = &(d.dptr.d->blist[i]->info_sob->pbir_core);
		hd->uir = (cidx != SPWAW_BADIDX) ? &(bir->uir[cidx]) : NULL;

		if (!base) base = bir;
		bidx = cidx;
		if ((i != 0) && (bidx != SPWAW_BADIDX)) {
			if (d.pvcmp) {
				bidx = d.dptr.d->blist[i]->ra[cidx].src;
			} else {
				j = i; bidx = d.dptr.d->blist[j]->ra[cidx].src;
				while (j>1) bidx = d.dptr.d->blist[--j]->ra[bidx].src;
			}
		}
		buir = (bidx != SPWAW_BADIDX) ? &(base->uir[bidx]) : NULL;

		for (j=0; j<d.col_cnt; j++)
			SPWDLT_prep (&(hd->dlt[j]), MDLH_coldef(j)->dtype, hd->uir ? hd->uir->snap : NULL, buir ? buir->snap : NULL, MDLH_coldef(j)->doffs);

		if (d.pvcmp) {
			base = bir;
		}

		if (i != 0) {
			pidx = (cidx != SPWAW_BADIDX) ? d.dptr.d->blist[i]->ra[cidx].src : SPWAW_BADIDX;
			if ((pidx != SPWAW_BADIDX) && d.dptr.d->blist[i-1]->ra[pidx].rpl) {
				hd->cflag = MDLH_CFLAG_REPLACED;
			} else if (pidx != cidx) {
				hd->cflag = MDLH_CFLAG_REASSIGNED;
			} else if ((pidx != SPWAW_BADIDX) && (d.dptr.d->blist[i-1]->info_sob->pbir_core.uir[pidx].snap->data.rank != hd->uir->snap->data.rank)) {
				hd->cflag = MDLH_CFLAG_PROMOTED;
			} else {
				hd->cflag = MDLH_CFLAG_NONE;
			}
			cidx = (cidx != SPWAW_BADIDX) ? d.dptr.d->blist[i]->ra[cidx].dst : SPWAW_BADIDX;
			//DEVASSERT ((cidx >= 0) && (cidx < d.dptr.d->ucnt));
		}
	}
}

void
ModelHistory::setupModelData_battle (void)
{
	int			i, j;
	MDLH_DATA		*hd;
	SPWAW_DOSSIER_BIR	*bir, *base;
	SPWAW_DOSSIER_UIR	*buir;

	/* Check data availability */
	if (!d.dptr.b || (!(d.row_cnt = d.dptr.b->tcnt))) return;

	/* Setup bbir */
	d.bbir = d.pflag
		? ( d.cflag ? &(d.dptr.b->info_sob->pbir_core) : &(d.dptr.b->info_sob->pbir_support))
		: &(d.dptr.b->info_sob->obir_battle);
	d.bbir_cnt = d.bbir->ucnt;

	/* Check start unit index */
	if (d.uidx >= d.bbir_cnt) { d.uidx = (USHORT)-1; d.row_cnt = 0; return; }

	/* Free and/or allocate data storage if required */
	if ((d.list_ref != d.dptr.b) || (d.list_cnt != d.row_cnt)) {
		if (d.list) SL_SAFE_FREE (d.list);
		if (d.dlts) SL_SAFE_FREE (d.dlts);
	}
	if (!d.list) {
		d.list_ref = d.dptr.b; d.list_cnt = d.row_cnt;
		SL_SAFE_CALLOC (d.list, d.list_cnt, sizeof (MDLH_DATA));
		SL_SAFE_CALLOC (d.dlts, d.list_cnt * d.col_cnt, sizeof (SPWDLT));

		/* Fill data */
		for (i=0; i<d.list_cnt; i++) {
			hd = &(d.list[i]);

			hd->idx		= i;
			hd->date.tdate	= d.dptr.b->tlist[i]->tdate;
			hd->dlt		= &(d.dlts[i*d.col_cnt]);
			hd->cflag	= MDLH_CFLAG_NONE;
		}
	}

	/* Update data */
	base = NULL;
	for (i=0; i<d.list_cnt; i++) {
		hd = &(d.list[i]);

		bir = d.pflag
			? ( d.cflag ? &(d.dptr.b->tlist[i]->info.pbir_core) : &(d.dptr.b->tlist[i]->info.pbir_support))
			: &(d.dptr.b->tlist[i]->info.obir_battle);
		hd->uir = &(bir->uir[d.uidx]);

		if (!base) base = bir;
		buir = &(base->uir[d.uidx]);

		for (j=0; j<d.col_cnt; j++)
			SPWDLT_prep (&(hd->dlt[j]), MDLH_coldef(j)->dtype, hd->uir->snap, buir->snap, MDLH_coldef(j)->doffs);

		if (d.pvcmp) {
			base = bir;
		}
	}
}

void
ModelHistory::setupModelData (void)
{
	DBG_TRACE_FENTER;

	freeModelData (false);
	if (d.campaign)
		setupModelData_campaign();
	else
		setupModelData_battle();

	DBG_TRACE_FLEAVE;
}

void
ModelHistory::freeModelData (bool all)
{
	if (all) {
		if (d.dlts) SL_SAFE_FREE (d.dlts);
		if (d.list) SL_SAFE_FREE (d.list); d.list_cnt = 0;
	}

	d.row_cnt = 0;
	d.bbir = NULL; d.bbir_cnt = 0;
}

void
ModelHistory::clear (void)
{
	d.dptr.d = NULL; d.campaign = d.pflag = d.cflag = d.pvcmp = true; d.uidx = 0;

	setupModelData();
	reset();
}

void
ModelHistory::load (SPWAW_DOSSIER *dossier,  bool prevcmp, int unit)
{
	d.dptr.d	= dossier;
	d.campaign	= true;
	d.pflag		= true;
	d.cflag		= true;
	d.pvcmp		= prevcmp;
	d.uidx		= unit;

	setupModelData();
	reset();
}

void
ModelHistory::load (SPWAW_BATTLE *battle, bool player, bool iscore, bool prevcmp, int unit)
{
	d.dptr.b	= battle;
	d.campaign	= false;
	d.pflag		= player;
	d.cflag		= iscore;
	d.pvcmp		= prevcmp;
	d.uidx		= unit;

	setupModelData();
	reset();
}

void
ModelHistory::info (MDLH_INFO &info)
{
	info.uidx    = d.uidx;
	info.bir     = d.bbir;
	info.bir_cnt = d.bbir_cnt;
}

void
ModelHistory::select (USHORT uidx)
{
	d.uidx = uidx;

	setupModelData();
	reset();
}

void
ModelHistory::highlight (MDLH_HILITE h)
{
	d.hilite = h;
	reset();
}

int
ModelHistory::max_width (int column)
{
	int	max = 0;

	for (int i=0; i<d.row_cnt; i++) {
		int w = (MDLH_data (Qt::DisplayRole, i, column)).toString().length();
		if (w > max) max = w;
	}
	return (max);
}

void
ModelHistory::refresh (void)
{
	//DBG_TRACE_FENTER;
	//DBG_TRACE_FLEAVE;
}
