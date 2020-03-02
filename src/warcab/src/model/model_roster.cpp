/** \file
 * The SPWaW war cabinet - data model handling - unit data.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "model_roster.h"
#include <spwawlib_uht.h>

typedef struct s_MDLR_COLUMN_DEF {
	MDLR_COLUMN	id;
	const char	*name;
	bool		biv;
	MDL_CMP		cmp;
	SPWDLT_TYPE	dtype;
	unsigned int	doffs;
} MDLR_COLUMN_DEF;

#define	OFFS(m_)	offsetof(SPWAW_SNAP_OOB_UEL,m_)

static MDLR_COLUMN_DEF coldef[MDLR_COLUMN_CNT] = {
	{ MDLR_COLUMN_CDSTATUS,		"",		true,	NULL,		SPWDLT_NONE,	0			},
	{ MDLR_COLUMN_UID,		"#",		true,	MDLR_cmp_uid,	SPWDLT_NONE,	0			},
	{ MDLR_COLUMN_UNIT,		"Unit",		true,	MDLR_cmp_unit,	SPWDLT_STR,	OFFS(data.dname)	}, // FIXME: designation, actually
	{ MDLR_COLUMN_RNK,		"Rank",		true,	MDLR_cmp_rnk,	SPWDLT_INT,	OFFS(data.brank)	},
	{ MDLR_COLUMN_LDR,		"Leader",	true,	MDLR_cmp_ldr,	SPWDLT_STR,	OFFS(data.lname)	},
	{ MDLR_COLUMN_STATUS,		"Status",	false,	MDLR_cmp_status,SPWDLT_INT,	OFFS(data.status)	},
	{ MDLR_COLUMN_KILL,		"Kills",	true,	MDLR_cmp_kill,	SPWDLT_INT,	OFFS(attr.gen.kills)	},
	{ MDLR_COLUMN_EXP,		"Exp",		true,	MDLR_cmp_exp,	SPWDLT_INT,	OFFS(data.exp)		},
	{ MDLR_COLUMN_MOR,		"Mor",		true,	MDLR_cmp_mor,	SPWDLT_INT,	OFFS(data.mor)		},
	{ MDLR_COLUMN_SUP,		"Sup",		false,	MDLR_cmp_sup,	SPWDLT_INT,	OFFS(data.sup)		},
	{ MDLR_COLUMN_RAL,		"Ral",		true,	MDLR_cmp_ral,	SPWDLT_INT,	OFFS(data.ral)		},
	{ MDLR_COLUMN_INF,		"Inf",		true,	MDLR_cmp_inf,	SPWDLT_INT,	OFFS(data.inf)		},
	{ MDLR_COLUMN_ARM,		"Arm",		true,	MDLR_cmp_arm,	SPWDLT_INT,	OFFS(data.arm)		},
	{ MDLR_COLUMN_ART,		"Art",		true,	MDLR_cmp_art,	SPWDLT_INT,	OFFS(data.art)		},
	{ MDLR_COLUMN_MEN,		"Men",		true,	MDLR_cmp_men,	SPWDLT_INT,	OFFS(data.hcnt)		},
	{ MDLR_COLUMN_RDY,		"Ready",	false,	MDLR_cmp_rdy,	SPWDLT_DBL,	OFFS(attr.gen.ready)	},
	{ MDLR_COLUMN_KIA,		"KIA",		false,	MDLR_cmp_kia,	SPWDLT_NONE,	0			},	/* unstored item */
	{ MDLR_COLUMN_DMG,		"Dmg",		false,	MDLR_cmp_dmg,	SPWDLT_INT,	OFFS(data.damage)	},
	{ MDLR_COLUMN_SEEN,		"Spotted",	false,	MDLR_cmp_seen,	SPWDLT_BOOL,	OFFS(data.spotted)	},
	{ MDLR_COLUMN_ABAND,		"Abandoned",	false,	MDLR_cmp_aband,	SPWDLT_INT,	OFFS(data.aband)	},
	{ MDLR_COLUMN_LOADED,		"Loaded",	false,	MDLR_cmp_loaded,SPWDLT_BOOL,	OFFS(data.loaded)	},
	{ MDLR_COLUMN_TYPE,		"Type",		true,	MDLR_cmp_type,	SPWDLT_INT,	OFFS(data.utype)	},
	{ MDLR_COLUMN_CLASS,		"Class",	true,	MDLR_cmp_class,	SPWDLT_INT,	OFFS(data.uclass)	},
	{ MDLR_COLUMN_COST,		"Cost",		true,	MDLR_cmp_cost,	SPWDLT_INT,	OFFS(data.cost)		},
	{ MDLR_COLUMN_SPEED,		"Speed",	false,	MDLR_cmp_speed,	SPWDLT_INT,	OFFS(data.speed)	},
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
ModelRoster::setupModelDataStorage (void)
{
	int			i;
	MDLR_DATA		*u;

	/* Check current unit count to see if data storage reallocation is required */
	if (d.list_cnt != d.row_cnt) {
		/* Free previously allocated data storage */
		if (d.list) SL_SAFE_FREE (d.list);
		if (d.dlts) SL_SAFE_FREE (d.dlts);
		if (d.smap) SL_SAFE_FREE (d.smap); d.smap_cnt = 0;

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
	d.list_use = 0;
}

void
ModelRoster::freeModelDataStorage (void)
{
	if (d.smap) SL_SAFE_FREE (d.smap); d.smap_cnt = 0;
	if (d.dlts) SL_SAFE_FREE (d.dlts);
	if (d.list) SL_SAFE_FREE (d.list); d.list_cnt = d.list_use = 0;

	d.row_cnt = 0;
}

void
ModelRoster::setupSortMap (void)
{
	int	i;

	DBG_TRACE_FENTER;

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
ModelRoster::addModelData (SPWAW_UHTE *uhte, SPWAW_DOSSIER_UIR *uir, SPWAW_DOSSIER_UIR *cuir, SPWAW_DOSSIER_UIR *ruir)
{
	unsigned int		idx;
	MDLR_DATA		*u;
	SPWAW_SNAP_OOB_UEL	*s, *cs, *rs;
	SPWDLT			*dlt;

	/* don't overflow the data list! */
	if (d.list_use >= d.list_cnt) return;

	idx = d.list_use++;

	u = &(d.list[idx]);

	u->uhte = uhte; u->uir = uir;
	u->decomm = u->uhte?SPWAW_UHT_is_decommissioned (u->uhte):false;

	s = uir->snap;
	cs = cuir ? cuir->snap : NULL;
	rs = ruir ? ruir->snap : NULL;

	for (int j=0; j<d.col_cnt; j++) {
		dlt = &(d.dlts[idx*d.col_cnt+j]);
		if (MDLR_coldef(j)->biv) {
			SPWDLT_prep (dlt, MDLR_coldef(j)->dtype, s, rs, MDLR_coldef(j)->doffs);
		} else {
			SPWDLT_prep (dlt, MDLR_coldef(j)->dtype, s, cs, MDLR_coldef(j)->doffs);
		}
	}
}

void
ModelRoster::setupModelData (void)
{
	bool			dossier_mode;
	SPWAW_UHTE		*cuhte, *ruhte;
	SPWAW_DOSSIER_UIR	*cuir, *buir, *ruir;

	DBG_TRACE_FENTER;

	freeModelData (false);

	dossier_mode = (d.d != NULL);

	if (dossier_mode) {
		if (!(d.row_cnt = d.d->uht.icnt)) return;
	} else {
		if (!d.birs || !d.cbrs || !d.rbrs) return;
		if (!(d.row_cnt = d.birs_cnt)) return;
	}

	setupModelDataStorage();

	/* Update unit data */
	if (dossier_mode) {
		for (unsigned int i=0; i<d.d->uht.cnt; i++) {
			SPWAW_BATTLE *cb, *bb;

			SPWAW_UHTE *uhte = d.d->uht.smap[i];
			if (!SPWAW_UHT_is_initial (uhte)) continue;
			if (!d.fchflag && SPWAW_UHT_is_decommissioned (uhte)) continue;

			SPWAW_UHT_lookup (SPWAW_UHT_last (uhte), &(d.d->blast->bdate), false, &cuhte, NULL, &cuir);
			SPWAW_dossier_find_battle (d.d, &(cuhte->LBD), &cb);
			if (!cb) continue;

			SPWAW_UHT_lookup (SPWAW_UHT_first (uhte), &(d.d->bfirst->bdate), false, &ruhte, &ruir, NULL);
			SPWAW_dossier_find_battle (d.d, &(ruhte->FBD), &bb);
			if (!bb) continue;

			buir = ruir;

			addModelData (cuhte, cuir, buir, ruir);
		}
	} else {
		for (int i=0; i<d.list_cnt; i++) {
			if (!d.tflag && d.pflag && d.cflag && d.cb != NULL) {
				cuhte = d.cb->uhtinfo->list[i];
				SPWAW_UHT_lookup (cuhte, &(d.cb->bdate), true, NULL, &buir, &cuir);
				if (d.bb && (d.bb != d.cb)) {
					SPWAW_UHT_lookup (SPWAW_UHT_first(cuhte), &(d.bb->bdate), true, NULL, NULL, &ruir);
					if (!ruir) ruir = buir;
				} else {
					ruir = buir;
				}
			} else {
				cuhte = NULL;
				cuir = &(d.birs->uir[i]);
				buir = &(d.cbrs->uir[i]);
				ruir = &(d.rbrs->uir[i]);
			}

			addModelData (cuhte, cuir, buir, ruir);
		}
	}

	/* Update final list and row counts */
	d.row_cnt = d.list_use;

	setupSortMap();

	DBG_TRACE_FLEAVE;
}

void
ModelRoster::freeModelData (bool all)
{
	if (all) freeModelDataStorage();

	d.row_cnt = 0;
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
	if (d.scol < 0) return;

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
	d.birs = d.cbrs = d.rbrs = NULL;
	d.birs_cnt = d.cbrs_cnt = d.rbrs_cnt = 0;
	d.d = NULL; d.cb = d.bb = NULL;
	d.fchflag = false; d.tflag = false; d.pflag = true; d.cflag = true; d.scol = d.sord = -1;

	setupModelData();
	reset();
}

void
ModelRoster::load (SPWAW_DOSSIER *dossier, bool fch)
{
	d.birs = d.cbrs = d.rbrs = NULL;
	d.birs_cnt = d.cbrs_cnt = d.rbrs_cnt = 0;
	d.d = dossier; d.cb = d.bb = NULL;

	d.fchflag = fch; d.tflag = false; d.pflag = true; d.cflag = true; d.scol = d.sord = -1;

	setupModelData();
	reset();
}

void
ModelRoster::load (SPWAW_BATTLE *current, SPWAW_BATTLE *start, bool isplayer, bool iscore)
{
	SPWAW_DOSSIER_BIR	*nbirs, *ncbrs, *nrbrs;

	if (!current || !start) {
		d.birs = d.cbrs = d.rbrs = NULL;
		d.birs_cnt = d.cbrs_cnt = d.rbrs_cnt = 0;
		d.d = NULL; d.cb = d.bb = NULL;
	} else {
		d.d = NULL; d.cb = current; d.bb = start;

		if (!isplayer || !iscore) { d.bb = d.cb; }

		nbirs = isplayer
			? (iscore ? &(d.cb->info_eob->pbir_core) : &(d.cb->info_eob->pbir_support))
			: &(d.cb->info_eob->obir_battle);

		ncbrs = isplayer
			? (iscore ? &(d.cb->info_sob->pbir_core) : &(d.cb->info_sob->pbir_support))
			: &(d.cb->info_sob->obir_battle);

		if (d.bb == d.cb) {
			nrbrs = ncbrs;
		} else {
			nrbrs = (isplayer && iscore) ? &(d.bb->info_eob->pbir_core) : ncbrs;
		}

		/* Early bailout if attempting to reload same data */
		if ((d.birs == nbirs) && (d.cbrs == ncbrs) && (d.rbrs == nrbrs)) return;
		d.birs = nbirs; d.birs_cnt = d.birs->ucnt;
		d.cbrs = ncbrs; d.cbrs_cnt = d.cbrs->ucnt;
		d.rbrs = nrbrs; d.rbrs_cnt = d.rbrs->ucnt;
	}

	d.fchflag = false; d.tflag = false; d.pflag = isplayer; d.cflag = iscore; d.scol = d.sord = -1;

	setupModelData();
	reset();
}

void
ModelRoster::load (SPWAW_BTURN *current, SPWAW_BTURN *start, bool isplayer, bool iscore)
{
	SPWAW_DOSSIER_BIR	*nbirs, *ncbrs, *nrbrs;

	if (!current || !start) {
		d.birs = d.cbrs = d.rbrs = NULL;
		d.birs_cnt = d.cbrs_cnt = d.rbrs_cnt = 0;
		d.d = NULL; d.cb = d.bb = NULL;
	} else {
		d.d = NULL; d.cb = d.bb = current->battle;
		nbirs = isplayer
			? (iscore ? &(current->info.pbir_core) : &(current->info.pbir_support))
			: &(current->info.obir_battle);
		ncbrs = isplayer
			? (iscore ? &(start->info.pbir_core) : &(start->info.pbir_support))
			: &(start->info.obir_battle);
		nrbrs = ncbrs;

		/* Early bailout if attempting to reload same data */
		if ((d.birs == nbirs) && (d.cbrs == ncbrs) && (d.rbrs == nrbrs)) return;
		d.birs = nbirs; d.birs_cnt = d.birs->ucnt;
		d.cbrs = ncbrs; d.cbrs_cnt = d.cbrs->ucnt;
		d.rbrs = nrbrs; d.rbrs_cnt = d.rbrs->ucnt;
	}

	d.fchflag = false; d.tflag = true; d.pflag = isplayer; d.cflag = iscore; d.scol = d.sord = -1;

	setupModelData();
	reset();
}

void
ModelRoster::set_marking (bool mark)
{
	d.mflag = mark;
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
		data[i].idx    = d.smap[r].data->idx;
		data[i].uhte   = d.smap[r].data->uhte;
		data[i].decomm = d.smap[r].data->decomm;
		data[i].uir    = d.smap[r].data->uir;
		data[i].dlt    = &(d.smap[r].data->dlt[col]);
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
