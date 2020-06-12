/** \file
 * The SPWaW war cabinet - data model handling - unit list.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "model_unitlist.h"

ModelUnitlist::ModelUnitlist (QFont *rgfont, QFont *dcfont, QObject *parent)
	: QAbstractTableModel (parent)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.rgfont = rgfont; d.dcfont = dcfont;

	header << "Unit";
}

ModelUnitlist::~ModelUnitlist (void)
{
	DBG_TRACE_DESTRUCT;

	freeModelData(true);
}

QVariant
ModelUnitlist::data (const QModelIndex &index, int role) const
{
	if (!d.row_cnt) return (QVariant());
	if (!index.isValid()) return (QVariant());

	return (MDLU_data (role, index.row(), index.column()));
}

Qt::ItemFlags
ModelUnitlist::flags (const QModelIndex &index) const
{
	if (!index.isValid()) return (0);

	return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

QVariant
ModelUnitlist::headerData (int section, Qt::Orientation orientation, int role) const
{
	if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return (QVariant());

	return (header.value (section));
}

int
ModelUnitlist::rowCount (const QModelIndex &/*parent*/) const
{
	if (!d.pflag && (d.intel_mode == INTEL_MODE_NONE)) return (0);

	return (d.row_cnt);
}

int
ModelUnitlist::columnCount (const QModelIndex &/*parent*/) const
{
	return (1);
}

QModelIndex
ModelUnitlist::index (int row, int column, const QModelIndex &parent) const
{
	QModelIndex	idx = QModelIndex();

	if (!d.pflag && (d.intel_mode == INTEL_MODE_NONE)) return (QModelIndex());

	if (!hasIndex (row, column, parent)) return (QModelIndex());

	if (row < d.row_cnt) idx = createIndex (row, column, &(d.list[row]));
	return (idx);
}

void
ModelUnitlist::setupModelDataStorage (void)
{
	int			i;
	MDLU_DATA		*u;

	/* Check current unit count to see if data storage reallocation is required */
	if (d.list_cnt != d.row_cnt) {
		/* Free previously allocated data storage */
		if (d.list) SL_SAFE_FREE (d.list);

		/* Allocate new data storage */
		d.list_cnt = d.row_cnt;
		SL_SAFE_CALLOC (d.list, d.list_cnt, sizeof (MDLU_DATA));

		/* Fill data */
		for (i=0; i<d.list_cnt; i++) {
			u = &(d.list[i]);

			u->idx     = i;
		}
	}
	d.list_use = 0;
}

void
ModelUnitlist::freeModelDataStorage (void)
{
	if (d.list) SL_SAFE_FREE (d.list); d.list_cnt = d.list_use = 0;

	d.row_cnt = 0;
}

void
ModelUnitlist::addModelData (int uidx, SPWAW_UHTE *uhte, SPWAW_DOSSIER_UIR *uir)
{
	unsigned int		idx;
	MDLU_DATA		*u;

	/* don't overflow the data list! */
	if (d.list_use >= d.list_cnt) return;

	idx = d.list_use++;

	u = &(d.list[idx]);

	u->uidx = uidx;
	u->uhte = uhte;
	u->uir = uir;
	u->comm = u->uhte?SPWAW_UHT_is_commissioned (u->uhte):false;
	u->decomm = u->uhte?SPWAW_UHT_is_decommissioned (u->uhte):false;
}

void
ModelUnitlist::setupModelData (void)
{
	bool			dossier_mode;
	SPWAW_UHTE		*uhte;
	SPWAW_DOSSIER_UIR	*uir;

	DBG_TRACE_FENTER;

	freeModelData (true);

	dossier_mode = (d.d != NULL);

	if (dossier_mode) {
		if (!(d.row_cnt = d.d->uht.icnt)) return;
	} else {
		if (!d.b) return;
		if (d.pflag && d.cflag) {
			if (d.b->uhtinfo) {
				if (!(d.row_cnt = d.b->uhtinfo->cnt)) return;
			} else {
				if (!(d.row_cnt = d.birs_cnt)) return;
			}
		} else {
			if (!(d.row_cnt = d.birs_cnt)) return;
		}
	}

	setupModelDataStorage();

	/* Update unit data */
	if (dossier_mode) {
		for (unsigned int i=0; i<d.d->uht.cnt; i++) {
			uhte = d.d->uht.smap[i];

			if (!SPWAW_UHT_is_initial (uhte)) continue;
			if (!d.fchflag && SPWAW_UHT_is_decommissioned (uhte)) continue;
			if (!SPWAW_UHT_lookup (uhte, &(uhte->FBD), true, NULL, &uir, NULL)) continue;

			addModelData (i, uhte, uir);
		}
		d.cdflag = false;
		for (int i=0; i<d.list_use; i++) {
			if (d.list[i].comm || d.list[i].decomm) {
				d.cdflag = true;
				break;
			}
		}
	} else {
		if (d.pflag && d.cflag) {
			for (int i=0; i<d.b->uhtinfo->cnt; i++) {
				uhte = d.b->uhtinfo->list[i];

				SPWAW_UHT_lookup (uhte, &(d.b->bdate), true, NULL, &uir, NULL);

				addModelData (i, uhte, uir);
			}
		} else {
			for (int i=0; i<d.birs_cnt; i++) {
				addModelData (i, NULL, &(d.birs->uir[i]));
			}
		}
	}

	/* Update final list and row counts */
	d.row_cnt = d.list_use;

	DBG_TRACE_FLEAVE;
}

void
ModelUnitlist::freeModelData (bool all)
{
	if (all) freeModelDataStorage();

	d.row_cnt = 0;
}


void
ModelUnitlist::clear (void)
{
	d.d = NULL; d.b = NULL; d.fchflag = false;

	setupModelData();
	reset();
}

void
ModelUnitlist::load (SPWAW_DOSSIER *dossier, bool fch, INTEL_MODE mode)
{
	d.d = dossier; d.b = NULL; d.birs = NULL;

	d.fchflag = fch; d.pflag = d.cflag = true; d.cdflag = false; d.intel_mode = mode;

	setupModelData();
	reset();
}

void
ModelUnitlist::load (SPWAW_BATTLE *battle, bool isplayer, bool iscore, INTEL_MODE mode)
{
	d.d = NULL;

	d.fchflag = false; d.pflag = isplayer; d.cflag = iscore; d.cdflag = false; d.intel_mode = mode;

	if (!battle) {
		d.b = NULL;
		d.birs = NULL;
		d.birs_cnt = 0;
	} else {
		d.b = battle;
		d.birs = isplayer
			? (iscore ? &(d.b->info_sob->pbir_core) : &(d.b->info_sob->pbir_support))
			: &(d.b->info_sob->obir_battle);
		d.birs_cnt = d.birs->ucnt;
	}

	setupModelData();
	reset();
}

bool
ModelUnitlist::dossier_mode (void)
{
	return (d.d != NULL);
}

bool
ModelUnitlist::cd_mode (void)
{
	return (d.cdflag);
}

int
ModelUnitlist::max_width (void)
{
	int	max = 0;

	for (int i=0; i<d.row_cnt; i++) {
		int w = (MDLU_data (Qt::DisplayRole, i, MDLU_COLUMN_ID)).toString().length();
		if (w > max) max = w;
	}
	return (max);
}

void
ModelUnitlist::refresh (void)
{
	//DBG_TRACE_FENTER;
	//DBG_TRACE_FLEAVE;
}

void
ModelUnitlist::intel_mode_set (INTEL_MODE mode)
{
	d.intel_mode = mode;

	reset();
}