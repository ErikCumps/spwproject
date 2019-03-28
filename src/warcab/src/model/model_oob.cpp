/** \file
 * The SPWaW war cabinet - data model handling - order of battle data.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "model_oob.h"

// TODO: get rid of COUNT column

typedef struct s_MDLO_COLUMN_DEF {
	MDLO_COLUMN	id;
	const char	*name;
	MDL_CMP		cmp;
	SPWDLT_TYPE	dtype;
	unsigned int	doffs;
} MDLO_COLUMN_DEF;

#define	FOFFS(m_)	offsetof(SPWAW_SNAP_OOB_FEL,m_)
#define	UOFFS(m_)	offsetof(SPWAW_SNAP_OOB_UEL,m_)

static MDLO_COLUMN_DEF fcoldef[MDLO_COLUMN_CNT] = {
	{ MDLO_COLUMN_FID,		"#",		MDLO_cmp_fid,	SPWDLT_NONE,	0			},
	{ MDLO_COLUMN_TYPE,		"Type",		MDLO_cmp_type,	SPWDLT_INT,	FOFFS(data.type)	},
	{ MDLO_COLUMN_LDR,		"Leader",	MDLO_cmp_ldr,	SPWDLT_NONE,	0			},	/* unstored item */
	{ MDLO_COLUMN_HCMD,		"HCmd",		MDLO_cmp_hcmd,	SPWDLT_NONE,	0			},	/* pointer item */
	{ MDLO_COLUMN_STATUS,		"Status",	MDLO_cmp_status,	SPWDLT_INT,	FOFFS(data.fstatus)	},
	{ MDLO_COLUMN_KILLS,		"Kills",	MDLO_cmp_kills,	SPWDLT_INT,	FOFFS(attr.gen.kills)	},
	{ MDLO_COLUMN_RDY,		"Ready",	MDLO_cmp_rdy,	SPWDLT_DBL,	FOFFS(attr.gen.ready)	},
	{ MDLO_COLUMN_COUNT,		"Count",	MDLO_cmp_cnt,	SPWDLT_INT,	FOFFS(data.ucnt)	},
	{ MDLO_COLUMN_EXP,		"Exp",		MDLO_cmp_exp,	SPWDLT_DBL,	FOFFS(attr.mmas_exp.avg)},
	{ MDLO_COLUMN_MOR,		"Mor",		MDLO_cmp_mor,	SPWDLT_DBL,	FOFFS(attr.mmas_mor.avg)},
	{ MDLO_COLUMN_SUP,		"Sup",		MDLO_cmp_sup,	SPWDLT_DBL,	FOFFS(attr.mmas_sup.avg)},
	{ MDLO_COLUMN_RAL,		"Ral",		MDLO_cmp_ral,	SPWDLT_DBL,	FOFFS(attr.mmas_ral.avg)},
	{ MDLO_COLUMN_INF,		"Inf",		MDLO_cmp_inf,	SPWDLT_DBL,	FOFFS(attr.mmas_inf.avg)},
	{ MDLO_COLUMN_ARM,		"Arm",		MDLO_cmp_arm,	SPWDLT_DBL,	FOFFS(attr.mmas_arm.avg)},
	{ MDLO_COLUMN_ART,		"Art",		MDLO_cmp_art,	SPWDLT_DBL,	FOFFS(attr.mmas_art.avg)},
};

static MDLO_COLUMN_DEF *fcoldef_cache[MDLO_COLUMN_CNT] = { 0 };

static MDLO_COLUMN_DEF ucoldef[MDLO_COLUMN_CNT] = {
	{ MDLO_COLUMN_FID,		NULL,	NULL,	SPWDLT_NONE,	0			},
	{ MDLO_COLUMN_TYPE,		NULL,	NULL,	SPWDLT_STR,	UOFFS(data.type)	},
	{ MDLO_COLUMN_LDR,		NULL,	NULL,	SPWDLT_NONE,	0			},	/* unstored item */
	{ MDLO_COLUMN_HCMD,		NULL,	NULL,	SPWDLT_NONE,	0			},	/* pointer item */
	{ MDLO_COLUMN_STATUS,		NULL,	NULL,	SPWDLT_INT,	UOFFS(data.status)	},
	{ MDLO_COLUMN_KILLS,		NULL,	NULL,	SPWDLT_INT,	UOFFS(attr.gen.kills)	},
	{ MDLO_COLUMN_RDY,		NULL,	NULL,	SPWDLT_DBL,	UOFFS(attr.gen.ready)	},
	{ MDLO_COLUMN_COUNT,		NULL,	NULL,	SPWDLT_INT,	UOFFS(data.hcnt)	},
	{ MDLO_COLUMN_EXP,		NULL,	NULL,	SPWDLT_INT,	UOFFS(data.exp)		},
	{ MDLO_COLUMN_MOR,		NULL,	NULL,	SPWDLT_INT,	UOFFS(data.mor)		},
	{ MDLO_COLUMN_SUP,		NULL,	NULL,	SPWDLT_INT,	UOFFS(data.sup)		},
	{ MDLO_COLUMN_RAL,		NULL,	NULL,	SPWDLT_INT,	UOFFS(data.ral)		},
	{ MDLO_COLUMN_INF,		NULL,	NULL,	SPWDLT_INT,	UOFFS(data.inf)		},
	{ MDLO_COLUMN_ARM,		NULL,	NULL,	SPWDLT_INT,	UOFFS(data.arm)		},
	{ MDLO_COLUMN_ART,		NULL,	NULL,	SPWDLT_INT,	UOFFS(data.art)		},
};

static MDLO_COLUMN_DEF *ucoldef_cache[MDLO_COLUMN_CNT] = { 0 };

MDLO_COLUMN_DEF *
MDLO_fcoldef (int col)
{
	MDLO_COLUMN_DEF	*p;

	if ((col < 0) || (col > MDLO_COLUMN_LAST)) return (NULL);

	if (p = fcoldef_cache[col]) return (p);

	for (int i=0; i<MDLO_COLUMN_CNT; i++)
		if (fcoldef[i].id == col) { p = &(fcoldef[i]); break; }

	return (fcoldef_cache[col] = p);
}

MDLO_COLUMN_DEF *
MDLO_ucoldef (int col)
{
	MDLO_COLUMN_DEF	*p;

	if ((col < 0) || (col > MDLO_COLUMN_LAST)) return (NULL);

	if (p = ucoldef_cache[col]) return (p);

	for (int i=0; i<MDLO_COLUMN_CNT; i++)
		if (ucoldef[i].id == col) { p = &(ucoldef[i]); break; }

	return (ucoldef_cache[col] = p);
}


ModelOob::ModelOob (QObject *parent)
	: QAbstractItemModel (parent)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.col_cnt = MDLO_COLUMN_CNT;
	for (int i=0; i<d.col_cnt; i++) header << MDLO_fcoldef(i)->name;

	d.scol = d.sord = -1;

	clear ();
}

ModelOob::~ModelOob (void)
{
	DBG_TRACE_DESTRUCT;

	if (d.explist.lst) SL_SAFE_FREE (d.explist.lst);
	freeModelData(true);
}

QVariant
ModelOob::data (const QModelIndex &index, int role) const
{
	MDLO_DATA	*p;

	if (!d.smap) return (QVariant());
	if (!index.isValid()) return (QVariant());

	if (parent (index) == QModelIndex()) {
		p = (MDLO_DATA *)index.internalPointer();
	} else {
		p = (MDLO_DATA *)index.internalPointer();
	}

	return (MDLO_data (role, index.column(), p));
}

Qt::ItemFlags
ModelOob::flags (const QModelIndex &index) const
{
	if (!index.isValid()) return (0);

	return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

QVariant
ModelOob::headerData (int section, Qt::Orientation orientation, int role) const
{
	if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return (QVariant());

	return (header.value (section));
}

int
ModelOob::rowCount (const QModelIndex &parent) const
{
	int		cnt = d.row_cnt;
	MDLO_DATA	*p;

	if (parent.isValid()) {
		p = (MDLO_DATA *)parent.internalPointer();
		cnt = p->ccnt;
	}

	return (cnt);
}

int
ModelOob::columnCount (const QModelIndex &/*parent*/) const
{
	return (d.col_cnt);
}

QModelIndex
ModelOob::index (int row, int column, const QModelIndex &parent) const
{
	QModelIndex	idx = QModelIndex();

	if (!hasIndex (row, column, parent)) return (QModelIndex());

	if (!parent.isValid()) {
		if (row < d.row_cnt) idx = createIndex (row, column, d.smap[row].data);
	} else {
		MDLO_DATA *p = (MDLO_DATA *)parent.internalPointer();
		if (row < p->ccnt) idx = createIndex (row, column, &(p->clst[row]));
	}
	return (idx);
}

int
ModelOob::find (MDLO_DATA *f) const
{
	int	idx = -1;

	for (int i=0; i<d.row_cnt; i++) {
		if (d.smap[i].data != f) continue;
		idx = i; break;
	}
	return (idx);
}

QModelIndex
ModelOob::parent (const QModelIndex &index) const
{
	QModelIndex	idx = QModelIndex();
	MDLO_DATA	*p;
	int		r;

	if (!index.isValid()) return (QModelIndex());

	p = (MDLO_DATA *)index.internalPointer();
	switch (p->type) {
		case MDLO_DATA_UNIT:
			r = find (p->parent);
			if (r >= 0) idx = createIndex (r, 0, p->parent);
			break;
		case MDLO_DATA_FORM:
		default:
			break;
	}
	return (idx);
}

static SPWAW_DOSSIER_UIR *
find_uir (SPWAW_DOSSIER_BIR *bir, int birs_cnt, int fidx, int uidx)
{
	SPWAW_DOSSIER_UIR	*u = NULL;
	SPWAW_SNAP_OOB_UEL	*q;

	if (!bir || (fidx < 0) || (fidx >= birs_cnt) || !bir->fir[fidx].snap) return (NULL);

	q = bir->fir[fidx].snap->data.ulist[uidx];
	for (int i=0; i<bir->ucnt; i++) {
		if (bir->uir[i].snap != q) continue;
		u = &(bir->uir[i]); break;
	}
	return (u);
}

void
ModelOob::setupModelData ()
{
	int			i, j, k;
	MDLO_DATA		*f, *u;
	SPWDLT			*dlt;
	SPWAW_DOSSIER_UIR	*uir, *buir;
	SPWAW_SNAP_OOB_FEL	*cs, *bs;

	DBG_TRACE_FENTER;

	freeModelData (false);
	if (!d.birs || !d.base) return;
	if (!(d.row_cnt = d.birs_cnt)) return;

	/* Check current formation count to see if data storage reallocation is required */
	if (d.tree_cnt != d.row_cnt) {
		/* Free previously allocated data storage */
		if (d.tree) SL_SAFE_FREE (d.tree);
		if (d.dlts) SL_SAFE_FREE (d.dlts);

		/* Allocate new data storage */
		d.tree_cnt = d.row_cnt;
		SL_SAFE_CALLOC (d.tree, d.tree_cnt, sizeof (MDLO_DATA));
		SL_SAFE_CALLOC (d.dlts, d.tree_cnt * d.col_cnt, sizeof (SPWDLT));

		/* Fill data */
		for (i=0; i<d.tree_cnt; i++) {
			f = &(d.tree[i]);

			f->type   = MDLO_DATA_FORM;
			f->idx    = i;
			f->dlt    = &(d.dlts[i*d.col_cnt]);
			f->dltsort = &(d.dltsort);
			f->revsort = &(d.revsort);
		}
	}

	/* Update formation data */
	for (i=0; i<d.tree_cnt; i++) {
		f = &(d.tree[i]);

		f->data.f = &(d.birs->fir[i]);
		f->ccnt   = f->data.f->snap->data.ucnt;

		for (j=0; j<d.col_cnt; j++) {
			dlt = &(d.dlts[i*d.col_cnt+j]);
			cs = d.birs->fir[i].snap;
			bs = (i < d.base_cnt) ? d.base->fir[i].snap : NULL;
			SPWDLT_prep (dlt, MDLO_fcoldef(j)->dtype, cs, bs, MDLO_fcoldef(j)->doffs);
		}
	}

	/* Populate formation unit lists */
	for (i=0; i<d.tree_cnt; i++) {
		f = &(d.tree[i]);

		SL_SAFE_CALLOC (f->clst, f->ccnt, sizeof (MDLO_DATA));
		SL_SAFE_CALLOC (f->cdlt, f->ccnt * d.col_cnt, sizeof (SPWDLT));
		for (j=0; j<f->ccnt; j++) {
			u = &(f->clst[j]);
			uir = find_uir (d.birs, d.birs_cnt, i, j);
			if (!uir) {
				uir = find_uir (d.birs, d.birs_cnt, i, j);
			}

			u->type   = MDLO_DATA_UNIT;
			u->parent = f;
			u->idx    = -1;
			u->data.u = uir;
			u->dlt    = &(f->cdlt[j*d.col_cnt]);
			u->dltsort = &(d.dltsort);
			u->revsort = &(d.revsort);

			for (k=0; k<d.col_cnt; k++) {
				dlt  = &(f->cdlt[j*d.col_cnt+k]);
				buir = find_uir (d.base, d.base_cnt, i, j);
				SPWDLT_prep (dlt, MDLO_ucoldef(k)->dtype, uir->snap, buir ? buir->snap : NULL, MDLO_ucoldef(k)->doffs);
			}
		}
	}

	if (d.smap_cnt != d.row_cnt) {
		/* Create new sort map */
		if (d.smap) SL_SAFE_FREE (d.smap);

		d.smap_cnt = d.row_cnt; SL_SAFE_CALLOC (d.smap, d.smap_cnt, sizeof (MDLO_SMAP));
		for (i=0; i<d.smap_cnt; i++) {
			d.smap[i].data = &(d.tree[i]);
		}
	} else {
		/* Update sort map */
		for (i=0; i<d.smap_cnt; i++) {
			d.smap[i].data = &(d.tree[d.smap[i].data->idx]);
		}
	}

	if ((d.explist.ref != d.base) || (d.explist.cnt != d.row_cnt)) {
		if (d.explist.lst) SL_SAFE_FREE (d.explist.lst);
		d.explist.ref = d.base;
		d.explist.cnt = d.row_cnt;
		if (d.explist.cnt) SL_SAFE_CALLOC (d.explist.lst, d.explist.cnt, sizeof (bool));
	}

	DBG_TRACE_FLEAVE;
}

void
ModelOob::freeModelData (bool all)
{
	int	i;

	for (i=0; i<d.row_cnt; i++) {
		SL_SAFE_FREE (d.tree[i].cdlt);
		SL_SAFE_FREE (d.tree[i].clst);
	}
	if (all) {
		if (d.smap) SL_SAFE_FREE (d.smap); d.smap_cnt = 0;
		if (d.dlts) SL_SAFE_FREE (d.dlts);
		if (d.tree) SL_SAFE_FREE (d.tree); d.tree_cnt = 0;
	}
	d.row_cnt = 0;
}

void
ModelOob::smap_swap (int i1, int i2)
{
	MDLO_SMAP	t;

	t = d.smap[i1]; d.smap[i1] = d.smap[i2]; d.smap[i2] = t;
}

void
ModelOob::sort (int column, Qt::SortOrder order)
{
	if (!d.row_cnt) return;

	if ((d.scol == column) && (d.sord == order)) return;
	d.scol = column; d.sord = order;

	resort();
}

void
ModelOob::resort (void)
{
	MDLO_COLUMN_DEF	*p = NULL;

	if (!d.row_cnt) return;

	p = MDLO_fcoldef (d.scol); DEVASSERT (p != NULL);
	if (!p->cmp) return;

	d.revsort = (d.sord == Qt::DescendingOrder);

	emit layoutAboutToBeChanged ();

	qsort (d.smap, d.row_cnt, sizeof (MDLO_SMAP), p->cmp);

	emit layoutChanged ();
}

void
ModelOob::clear (void)
{
	d.base = d.birs = NULL; d.pflag = true;
	d.scol = d.sord = -1;

	setupModelData();
	reset();
}

void
ModelOob::load (SPWAW_BATTLE *current, SPWAW_BATTLE *start, bool isplayer, bool iscore)
{
	SPWAW_DOSSIER_BIR	*nbirs, *nbase;

	if (!current || !start) {
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

		d.birs_cnt = d.birs->fcnt;
		d.base_cnt = d.base->fcnt;
	}

	d.pflag = isplayer; d.scol = d.sord = -1;
	setupModelData();
	reset();
}

void
ModelOob::load (SPWAW_BATTLE *battle, int current, int start, bool isplayer, bool iscore)
{
	SPWAW_DOSSIER_BIR	*nbirs, *nbase;

	if (!battle || (current < 0) || (start < 0) || (current < start) || (current >= battle->tcnt) || (start >= battle->tcnt)) {
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

		d.birs_cnt = d.birs->fcnt;
		d.base_cnt = d.base->fcnt;
	}

	d.pflag = isplayer; d.scol = d.sord = -1;
	setupModelData();
	reset();
}

void
ModelOob::load (SPWAW_BTURN *current, SPWAW_BTURN *start, bool isplayer, bool iscore)
{
	SPWAW_DOSSIER_BIR	*nbirs, *nbase;

	if (!current || !start) {
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

		d.birs_cnt = d.birs->fcnt;
		d.base_cnt = d.base->fcnt;
	}

	d.pflag = isplayer; d.scol = d.sord = -1;
	setupModelData();
	reset();
}

void
ModelOob::highlight (MDLO_HILITE h)
{
	d.hilite = h;
	reset();
}

int
ModelOob::max_width (int column)
{
	int	max = 0;

	for (int i=0; i<d.row_cnt; i++) {
		int w = (MDLO_data (Qt::DisplayRole, column, &(d.tree[i]))).toString().length();
		if (w > max) max = w;
	}
	return (max);
}

void
ModelOob::refresh (void)
{
	//DBG_TRACE_FENTER;
	//DBG_TRACE_FLEAVE;
}

void
ModelOob::set_expanded (const QModelIndex &index, bool flag)
{
	MDLO_DATA	*p;

	if (!d.smap) return;
	if (!index.isValid()) return;

	p = (MDLO_DATA *)index.internalPointer();
	d.explist.lst[p->idx] = flag;
}

bool
ModelOob::is_expanded (const QModelIndex &index)
{
	MDLO_DATA	*p;
	bool		f = false;

	if (!d.smap) return (false);
	if (!index.isValid()) return (false);

	p = (MDLO_DATA *)index.internalPointer();
	f = d.explist.lst[p->idx];

	return (f);
}

void
ModelOob::set_dltsort (bool sort)
{
	d.dltsort = sort;
}


