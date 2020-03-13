/** \file
 * The SPWaW war cabinet - data model handling - unit history.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
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
	{ MDLH_COLUMN_UID,	"#",		SPWDLT_CHR,	OFFS(strings.uid)	},
	{ MDLH_COLUMN_UNIT,	"Unit",		SPWDLT_STR,	OFFS(data.dname)	}, // FIXME: designation, actually
	{ MDLH_COLUMN_RNK,	"Rank",		SPWDLT_INT,	OFFS(data.brank)	},
	{ MDLH_COLUMN_LDR,	"Leader",	SPWDLT_STR,	OFFS(data.lname)	},
	{ MDLH_COLUMN_STATUS,	"Status",	SPWDLT_INT,	OFFS(data.status)	},
	{ MDLH_COLUMN_KILL,	"Kills",	SPWDLT_INT,	OFFS(attr.gen.kills)	},
	{ MDLH_COLUMN_EXP,	"Exp",		SPWDLT_INT,	OFFS(data.exp)		},
	{ MDLH_COLUMN_MOR,	"Mor",		SPWDLT_INT,	OFFS(data.mor)		},
	{ MDLH_COLUMN_SUP,	"Sup",		SPWDLT_INT,	OFFS(data.sup)		},
	{ MDLH_COLUMN_RAL,	"Ral",		SPWDLT_INT,	OFFS(data.ral)		},
	{ MDLH_COLUMN_INF,	"Inf",		SPWDLT_INT,	OFFS(data.inf)		},
	{ MDLH_COLUMN_ARM,	"Arm",		SPWDLT_INT,	OFFS(data.arm)		},
	{ MDLH_COLUMN_ART,	"Art",		SPWDLT_INT,	OFFS(data.art)		},
	{ MDLH_COLUMN_MEN,	"Men",		SPWDLT_INT,	OFFS(data.hcnt)		},
	{ MDLH_COLUMN_RDY,	"Ready",	SPWDLT_DBL,	OFFS(attr.gen.ready)	},
	{ MDLH_COLUMN_KIA,	"KIA",		SPWDLT_NONE,	0			},	/* unstored item */
	{ MDLH_COLUMN_DMG,	"Dmg",		SPWDLT_INT,	OFFS(data.damage)	},
	{ MDLH_COLUMN_SEEN,	"Spotted",	SPWDLT_BOOL,	OFFS(data.spotted)	},
	{ MDLH_COLUMN_ABAND,	"Abandoned",	SPWDLT_INT,	OFFS(data.aband)	},
	{ MDLH_COLUMN_LOADED,	"Loaded",	SPWDLT_BOOL,	OFFS(data.loaded)	},
	{ MDLH_COLUMN_TYPE,	"Type",		SPWDLT_INT,	OFFS(data.utype)	},
	{ MDLH_COLUMN_CLASS,	"Class",	SPWDLT_INT,	OFFS(data.uclass)	},
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

ModelHistory::ModelHistory (QFont *rgfont, QFont *dcfont, QObject *parent)
	: QAbstractTableModel (parent)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.rgfont = rgfont; d.dcfont = dcfont;

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
ModelHistory::setupModelDataStorage (void)
{
	bool		fill = false;
	MDLH_DATA	*hd;

	if (d.list_cnt != d.row_cnt) {
		/* Free previously allocated data storage */
		if (d.list) SL_SAFE_FREE (d.list);
		if (d.dlts) SL_SAFE_FREE (d.dlts);

		/* Allocate new data storage */
		d.list_cnt = d.row_cnt;
		SL_SAFE_CALLOC (d.list, d.list_cnt, sizeof (MDLH_DATA));
		SL_SAFE_CALLOC (d.dlts, d.list_cnt * d.col_cnt, sizeof (SPWDLT));

		fill = true;
	}

	/* Update reference */
	if (d.campaign) {
		if (d.dref != d.dptr.d) {
			d.dref = d.dptr.d;
			fill = true;
		}
	} else {
		if (d.dref != d.dptr.b) {
			d.dref = d.dptr.b;
			fill = true;
		}
	}

	if (fill) {
		/* Fill data */
		for (int i=0; i<d.list_cnt; i++) {
			hd = &(d.list[i]);

			if (d.campaign) {
				hd->date.bdate	= d.dptr.d->blist[i]->bdate;
			} else {
				hd->date.tdate	= d.dptr.b->tlist[i]->tdate;
			}

			hd->idx		= i;
			hd->dlt		= &(d.dlts[i*d.col_cnt]);
		}
	}
}

void
ModelHistory::freeModelDataStorage (void)
{
	d.dref = NULL;
	if (d.list) SL_SAFE_FREE (d.list); d.list_cnt = 0;
	if (d.dlts) SL_SAFE_FREE (d.dlts);
}

void
ModelHistory::addModelData (unsigned int idx, SPWAW_UHTE *uhte, SPWAW_DOSSIER_UIR *uir, SPWAW_DOSSIER_UIR *buir)
{
	MDLH_DATA		*u;
	SPWAW_SNAP_OOB_UEL	*s, *bs;
	SPWDLT			*dlt;

	u = &(d.list[idx]);

	u->uhte = uhte; u->uir = uir;
	u->decomm = u->uhte?SPWAW_UHT_is_decommissioned (u->uhte):false;

	s = uir ? uir->snap : NULL;
	bs = buir ? buir->snap : NULL;

	for (int j=0; j<d.col_cnt; j++) {
		dlt = &(d.dlts[idx*d.col_cnt+j]);
		SPWDLT_prep (dlt, MDLH_coldef(j)->dtype, s, bs, MDLH_coldef(j)->doffs);
	}
}

void
ModelHistory::setupModelData (void)
{
	bool			bad_uidx = false;
	SPWAW_UHTE		*uhte = NULL;

	DBG_TRACE_FENTER;

	freeModelData (false);

	if (d.campaign) {
		/* Check data availability */
		if (!d.dptr.d || (!(d.row_cnt = d.dptr.d->bcnt))) return;

		/* Check selected UHTE index */
		if (d.uidx >= d.dptr.d->uht.cnt) {
			bad_uidx = true;
		} else {
			/* Record UIR */
			uhte = d.dptr.d->uht.smap[d.uidx];
			d.id.uir = SPWAW_UHT_lookup_SOBUIR (uhte, &(uhte->FBD), true);
			d.id.uhte = uhte;
		}
	} else {
		SPWAW_DOSSIER_BIR	*bbir = NULL;
		unsigned int		bbir_cnt = 0;

		/* Check data availability */
		if (!d.dptr.b || (!(d.row_cnt = d.dptr.b->tcnt))) return;

		/* Setup bbir */
		bbir = d.pflag
			? ( d.cflag ? &(d.dptr.b->info_sob->pbir_core) : &(d.dptr.b->info_sob->pbir_support))
			: &(d.dptr.b->info_sob->obir_battle);
		bbir_cnt = bbir->ucnt;

		/* Check selected unit index */
		if (d.uidx >= bbir_cnt) {
			bad_uidx = true;
		} else {
			/* Record UIR */
			d.id.uir = &(bbir->uir[d.uidx]);
			d.id.uhte = NULL;
		}
	}

	/* Early exit if invalid uhte or unit index */
	if (bad_uidx) {
		d.dref = NULL;
		d.uidx = (unsigned int)-1; d.id.uir = NULL; d.id.uhte = NULL;
		d.row_cnt = 0;
		return;
	}

	setupModelDataStorage();

	if (d.campaign) {
		SPWAW_DOSSIER_UIR *base = NULL;
		for (int i=0; i<d.list_cnt; i++) {
			SPWAW_UHTE *buhte; SPWAW_DOSSIER_UIR *sobuir, *eobuir;
			SPWAW_UHT_lookup (uhte, &(d.dptr.d->blist[i]->bdate), true, &buhte, &sobuir, &eobuir);

			if (!base) base = eobuir ? d.id.uir : NULL;

			addModelData (i, buhte, eobuir, base);

			if (d.pvcmp) {
				base = eobuir;
			}
		}
	} else {
		SPWAW_DOSSIER_BIR *base = NULL;
		for (int i=0; i<d.list_cnt; i++) {
			SPWAW_DOSSIER_BIR *bir = d.pflag
				? ( d.cflag ? &(d.dptr.b->tlist[i]->info.pbir_core) : &(d.dptr.b->tlist[i]->info.pbir_support))
				: &(d.dptr.b->tlist[i]->info.obir_battle);

			if (!base) base = bir;

			addModelData (i, NULL, &(bir->uir[d.uidx]), &(base->uir[d.uidx]));

			if (d.pvcmp) {
				base = bir;
			}
		}
	}

	DBG_TRACE_FLEAVE;
}

void
ModelHistory::freeModelData (bool all)
{
	if (all) freeModelDataStorage();

	d.id.uir = NULL; d.id.uhte = NULL;
	d.row_cnt = 0;
}

void
ModelHistory::clear (void)
{
	d.dptr.d = NULL; d.campaign = d.pflag = d.cflag = d.pvcmp = true; d.uidx = 0;

	setupModelData();
	reset();
}

void
ModelHistory::load (SPWAW_DOSSIER *dossier,  bool prevcmp, int uidx)
{
	d.dptr.d	= dossier;
	d.dref		= NULL;
	d.campaign	= true;
	d.pflag		= true;
	d.cflag		= true;
	d.pvcmp		= prevcmp;
	d.uidx		= uidx;

	setupModelData();
	reset();
}

void
ModelHistory::load (SPWAW_BATTLE *battle, bool isplayer, bool iscore, bool prevcmp, int uidx)
{
	d.dptr.b	= battle;
	d.dref		= NULL;
	d.campaign	= false;
	d.pflag		= isplayer;
	d.cflag		= iscore;
	d.pvcmp		= prevcmp;
	d.uidx		= uidx;

	setupModelData();
	reset();
}

void
ModelHistory::set_marking (bool mark)
{
	d.mflag = mark;
}

void
ModelHistory::select (USHORT uidx)
{
	d.uidx = uidx;

	setupModelData();
	reset();
}

void
ModelHistory::identity (MDLH_IDENTITY &id)
{
	id = d.id;
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
