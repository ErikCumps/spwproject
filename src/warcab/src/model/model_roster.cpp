/** \file
 * The SPWaW war cabinet - data model handling - unit data.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "model_roster.h"

typedef struct s_MDLR_COLUMN_DEF {
	MDLR_COLUMN	id;
	const char	*name;
	MDL_CMP		cmp;
	SPWDLT_TYPE	dtype;
	unsigned int	doffs;
} MDLR_COLUMN_DEF;

#define	OFFS(m_)	offsetof(SPWAW_SNAP_OOB_UEL,m_)

static MDLR_COLUMN_DEF coldef[MDLR_COLUMN_CNT] = {
	{ MDLR_COLUMN_UID,		"#",		MDLR_cmp_uid,	SPWDLT_NONE,	0			},
	{ MDLR_COLUMN_UNIT,		"Unit",		MDLR_cmp_unit,	SPWDLT_STR,	OFFS(data.dname)	}, // FIXME: designation, actually
	{ MDLR_COLUMN_RNK,		"Rank",		MDLR_cmp_rnk,	SPWDLT_INT,	OFFS(data.brank)	},
	{ MDLR_COLUMN_LDR,		"Leader",	MDLR_cmp_ldr,	SPWDLT_STR,	OFFS(data.lname)	},
	{ MDLR_COLUMN_STATUS,		"Status",	MDLR_cmp_status,SPWDLT_INT,	OFFS(data.status)	},
	{ MDLR_COLUMN_KILL,		"Kills",	MDLR_cmp_kill,	SPWDLT_INT,	OFFS(attr.gen.kills)	},
	{ MDLR_COLUMN_EXP,		"Exp",		MDLR_cmp_exp,	SPWDLT_INT,	OFFS(data.exp)		},
	{ MDLR_COLUMN_MOR,		"Mor",		MDLR_cmp_mor,	SPWDLT_INT,	OFFS(data.mor)		},
	{ MDLR_COLUMN_SUP,		"Sup",		MDLR_cmp_sup,	SPWDLT_INT,	OFFS(data.sup)		},
	{ MDLR_COLUMN_RAL,		"Ral",		MDLR_cmp_ral,	SPWDLT_INT,	OFFS(data.ral)		},
	{ MDLR_COLUMN_INF,		"Inf",		MDLR_cmp_inf,	SPWDLT_INT,	OFFS(data.inf)		},
	{ MDLR_COLUMN_ARM,		"Arm",		MDLR_cmp_arm,	SPWDLT_INT,	OFFS(data.arm)		},
	{ MDLR_COLUMN_ART,		"Art",		MDLR_cmp_art,	SPWDLT_INT,	OFFS(data.art)		},
	{ MDLR_COLUMN_MEN,		"Men",		MDLR_cmp_men,	SPWDLT_INT,	OFFS(data.hcnt)		},
	{ MDLR_COLUMN_RDY,		"Ready",	MDLR_cmp_rdy,	SPWDLT_DBL,	OFFS(attr.gen.ready)	},
	{ MDLR_COLUMN_KIA,		"KIA",		MDLR_cmp_kia,	SPWDLT_NONE,	0			},	/* unstored item */
	{ MDLR_COLUMN_DMG,		"Dmg",		MDLR_cmp_dmg,	SPWDLT_INT,	OFFS(data.damage)	},
	{ MDLR_COLUMN_SEEN,		"Spotted",	MDLR_cmp_seen,	SPWDLT_BOOL,	OFFS(data.spotted)	},
	{ MDLR_COLUMN_ABAND,		"Abandoned",	MDLR_cmp_aband,	SPWDLT_INT,	OFFS(data.aband)	},
	{ MDLR_COLUMN_LOADED,		"Loaded",	MDLR_cmp_loaded,SPWDLT_BOOL,	OFFS(data.loaded)	},
	{ MDLR_COLUMN_TYPE,		"Type",		MDLR_cmp_type,	SPWDLT_INT,	OFFS(data.utype)	},
	{ MDLR_COLUMN_CLASS,		"Class",	MDLR_cmp_class,	SPWDLT_INT,	OFFS(data.uclass)	},
	{ MDLR_COLUMN_COST,		"Cost",		MDLR_cmp_cost,	SPWDLT_INT,	OFFS(data.cost)		},
	{ MDLR_COLUMN_SPEED,		"Speed",	MDLR_cmp_speed,	SPWDLT_INT,	OFFS(data.speed)	},
};

static MDLR_COLUMN_DEF *coldef_cache[MDLR_COLUMN_CNT] = { 0 };

MDLR_COLUMN_DEF *
MDLR_coldef (int col)
{
	MDLR_COLUMN_DEF	*p;

	if ((col < 0) || (col > MDLR_COLUMN_LAST)) return (NULL);

	if (p = coldef_cache[col]) return (p);

	for (int i=0; i<MDLR_COLUMN_CNT; i++)
		if (coldef[i].id == col) { p = &(coldef[i]); break; }

	return (coldef_cache[col] = p);
}

ModelRoster::ModelRoster (QObject *parent)
	: QAbstractTableModel (parent)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.col_cnt = MDLR_COLUMN_CNT;
	for (int i=0; i<d.col_cnt; i++) header << MDLR_coldef(i)->name;

	d.scol = d.sord = -1;

	clear();
}

ModelRoster::~ModelRoster (void)
{
	DBG_TRACE_DESTRUCT;

	freeModelData(true);
}

QVariant
ModelRoster::data (const QModelIndex &index, int role) const
{
	if (!d.smap) return (QVariant());
	if (!index.isValid()) return (QVariant());

	return (MDLR_data (role, index.row(), index.column()));
}

Qt::ItemFlags
ModelRoster::flags (const QModelIndex &index) const
{
	if (!index.isValid()) return (0);

	return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

QVariant
ModelRoster::headerData (int section, Qt::Orientation orientation, int role) const
{
	if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return (QVariant());

	return (header.value (section));
}

int
ModelRoster::rowCount (const QModelIndex &/*parent*/) const
{
	return (d.row_cnt);
}

int
ModelRoster::columnCount (const QModelIndex &/*parent*/) const
{
	return (d.col_cnt);
}

void
ModelRoster::setupModelData (void)
{
	int			i, j, bidx;
	MDLR_DATA		*u;
	SPWDLT			*dlt;
	SPWAW_SNAP_OOB_UEL	*cs, *bs;

	DBG_TRACE_FENTER;

	freeModelData (false);
	if (!d.birs || !d.base) return;
	if (!(d.row_cnt = d.birs_cnt)) return;

	/* Check current unit count to see if data storage reallocation is required */
	if (d.list_cnt != d.row_cnt) {
		/* Free previously allocated data storage */
		if (d.list) SL_SAFE_FREE (d.list);
		if (d.dlts) SL_SAFE_FREE (d.dlts);

		/* Allocate new data storage */
		d.list_cnt = d.row_cnt;
		SL_SAFE_CALLOC (d.list, d.list_cnt, sizeof (MDLR_DATA));
		SL_SAFE_CALLOC (d.dlts, d.list_cnt * d.col_cnt, sizeof (SPWDLT));

		/* Fill data */
		for (i=0; i<d.list_cnt; i++) {
			u = &(d.list[i]);

			u->idx     = i;
			u->dlt     = &(d.dlts[i*d.col_cnt]);
			u->dltsort = &(d.dltsort);
			u->revsort = &(d.revsort);
		}
	}

	/* Update unit data */
	for (i=0; i<d.list_cnt; i++) {
		u = &(d.list[i]);

		u->uir = &(d.birs->uir[i]);

		bidx = i;
		if (d.d && (d.cb != d.sb) && (d.cb != NULL) && (d.sb != NULL)) {
			SPWAW_BATTLE *p = d.cb;
			while (p != d.sb) {
				bidx = p->ra[bidx].src;
				p = p->prev;
			}
		}

		for (j=0; j<d.col_cnt; j++) {
			dlt = &(d.dlts[i*d.col_cnt+j]);
			cs = u->uir->snap;
			bs = (bidx < d.base_cnt) ? d.base->uir[bidx].snap : NULL;
			SPWDLT_prep (dlt, MDLR_coldef(j)->dtype, cs, bs, MDLR_coldef(j)->doffs);
		}
	}

	if (d.smap_cnt != d.row_cnt) {
		/* Create new sort map */
		if (d.smap) SL_SAFE_FREE (d.smap);

		d.smap_cnt = d.row_cnt; SL_SAFE_CALLOC (d.smap, d.smap_cnt, sizeof (MDLR_SMAP));
		for (i=0; i<d.smap_cnt; i++) {
			d.smap[i].data = &(d.list[i]);
		}
	} else {
		/* Update sort map */
		for (i=0; i<d.smap_cnt; i++) {
			d.smap[i].data = &(d.list[d.smap[i].data->idx]);
		}
	}

	DBG_TRACE_FLEAVE;
}

void
ModelRoster::freeModelData (bool all)
{
	if (all) {
		if (d.smap) SL_SAFE_FREE (d.smap); d.smap_cnt = 0;
		if (d.dlts) SL_SAFE_FREE (d.dlts);
		if (d.list) SL_SAFE_FREE (d.list); d.list_cnt = 0;
	}

	d.row_cnt = 0;
}

void
ModelRoster::smap_swap (int i1, int i2)
{
	MDLR_SMAP	t;

	t = d.smap[i1]; d.smap[i1] = d.smap[i2]; d.smap[i2] = t;
}

void
ModelRoster::sort (int column, Qt::SortOrder order)
{
	if (!d.row_cnt) return;

	if ((d.scol == column) && (d.sord == order)) return;
	d.scol = column; d.sord = order;

	resort();
}

void
ModelRoster::resort (void)
{
	MDLR_COLUMN_DEF	*p = NULL;

	if (!d.row_cnt) return;

	p = MDLR_coldef (d.scol); DEVASSERT (p != NULL);
	if (!p->cmp) return;

	d.revsort = (d.sord == Qt::DescendingOrder);

	emit layoutAboutToBeChanged ();

	qsort (d.smap, d.row_cnt, sizeof (MDLR_SMAP), p->cmp);

	emit layoutChanged ();
}

void
ModelRoster::clear (void)
{
	d.base = d.birs = NULL; d.pflag = true; d.d = NULL;
	d.cb = d.sb = NULL;
	d.scol = d.sord = -1;

	setupModelData();
	reset();
}

void
ModelRoster::load (SPWAW_BATTLE *current, SPWAW_BATTLE *start, bool isplayer, bool iscore)
{
	SPWAW_DOSSIER_BIR	*nbirs, *nbase;

	if (!current || !start) {
		d.d = NULL;
		d.birs = d.base = NULL;
		d.birs_cnt = d.base_cnt = 0;
	} else {
		if (current == start) {
			nbirs = isplayer
				? (iscore ? &(current->info_eob->pbir_core) : &(current->info_eob->pbir_support))
				: &(current->info_eob->obir_battle);
		} else {
			nbirs = isplayer
				? (iscore ? &(current->info_sob->pbir_core) : &(current->info_sob->pbir_support))
				: &(current->info_sob->obir_battle);
		}
		nbase = isplayer
			? (iscore ? &(start->info_sob->pbir_core) : &(start->info_sob->pbir_support))
			: &(start->info_sob->obir_battle);

		/* Early bailout if attempting to reload same data */
		if ((d.birs == nbirs) && (d.base == nbase)) return;
		d.birs = nbirs; d.base = nbase;

		d.d = NULL;
		if (isplayer && iscore) {
			d.d = current->dossier;
			d.cb = current;
			d.sb = start;
		}
		d.birs_cnt = d.birs->ucnt;
		d.base_cnt = d.base->ucnt;
	}

	d.pflag = isplayer; d.scol = d.sord = -1;
	setupModelData();
	reset();
}

void
ModelRoster::load (SPWAW_BATTLE *battle, int current, int start, bool isplayer, bool iscore)
{
	SPWAW_DOSSIER_BIR	*nbirs, *nbase;

	if (!battle || (current < 0) || (start < 0) || (current < start) || (current >= battle->tcnt) || (start >= battle->tcnt)) {
		d.d = NULL;
		d.birs = d.base = NULL;
		d.birs_cnt = d.base_cnt = 0;
	} else {
		nbirs = isplayer
			? (iscore ? &(battle->tlist[current]->info.pbir_core) : &(battle->tlist[current]->info.pbir_support))
			: &(battle->tlist[current]->info.obir_battle);
		nbase = isplayer
			? (iscore ? &(battle->tlist[start]->info.pbir_core) : &(battle->tlist[start]->info.pbir_support))
			: &(battle->tlist[start]->info.obir_battle);

		/* Early bailout if attempting to reload same data */
		if ((d.birs == nbirs) && (d.base == nbase)) return;
		d.birs = nbirs; d.base = nbase;

		d.d = NULL;
		d.birs_cnt = d.birs->ucnt;
		d.base_cnt = d.base->ucnt;
	}

	d.pflag = isplayer; d.scol = d.sord = -1;
	setupModelData();
	reset();
}

void
ModelRoster::load (SPWAW_BTURN *current, SPWAW_BTURN *start, bool isplayer, bool iscore)
{
	SPWAW_DOSSIER_BIR	*nbirs, *nbase;

	if (!current || !start) {
		d.d = NULL;
		d.birs = d.base = NULL;
		d.birs_cnt = d.base_cnt = 0;
	} else {
		nbirs = isplayer
			? (iscore ? &(current->info.pbir_core) : &(current->info.pbir_support))
			: &(current->info.obir_battle);
		nbase = isplayer
			? (iscore ? &(start->info.pbir_core) : &(start->info.pbir_support))
			: &(start->info.obir_battle);


		/* Early bailout if attempting to reload same data */
		if ((d.birs == nbirs) && (d.base == nbase)) return;
		d.birs = nbirs; d.base = nbase;

		d.d = NULL;
		d.birs_cnt = d.birs->ucnt;
		d.base_cnt = d.base->ucnt;
	}

	d.pflag = isplayer; d.scol = d.sord = -1;
	setupModelData();
	reset();
}

void
ModelRoster::highlight (MDLR_HILITE h)
{
	d.hilite = h;
	reset();
}

int
ModelRoster::max_width (int column)
{
	int	max = 0;

	for (int i=0; i<d.row_cnt; i++) {
		int w = (MDLR_data (Qt::DisplayRole, i, column)).toString().length();
		if (w > max) max = w;
	}
	return (max);
}

void
ModelRoster::refresh (void)
{
	//DBG_TRACE_FENTER;
	//DBG_TRACE_FLEAVE;
}

void
ModelRoster::set_dltsort (bool sort)
{
	d.dltsort = sort;
}

int
ModelRoster::rawdata (int row, MDLR_COLUMN col, ModelRosterRawData *data, int cnt)
{
	int	r, i;

	if (!data) return (0);
	for (i=0; i<cnt; i++) { data[i].idx = -1; data[i].uir = NULL; data[i].dlt = NULL; }

	if ((row < 0) || (row >= d.row_cnt)) return (0);

	r = row; i = 0;
	while (r < d.row_cnt) {
		data[i].idx = d.smap[r].data->idx;
		data[i].uir = d.smap[r].data->uir;
		data[i].dlt = &(d.smap[r].data->dlt[col]);
		r++; i++;
		if (i >= cnt) break;
	}
	return (i);
}

void
ModelRoster::filter_targets (MDLR_FILTER spec, xMDL_FILTER_LIST *tgt)
{
	xMDL_FILTER_clear (tgt);

	switch (spec) {
		case MDLR_FILTER_CLASS:
			xMDL_FILTER_add (tgt, "armor", QVariant (42));
			xMDL_FILTER_add (tgt, "infantry", QVariant (43));
			xMDL_FILTER_add (tgt, "C&C", QVariant (44));
			break;
		case MDLR_FILTER_SEEN:
			xMDL_FILTER_add (tgt, "spotted", QVariant (1));
			xMDL_FILTER_add (tgt, "hidden", QVariant (0));
		default:
			break;
	}
}
