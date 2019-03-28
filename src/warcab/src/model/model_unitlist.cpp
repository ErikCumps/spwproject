/** \file
 * The SPWaW war cabinet - data model handling - unit list.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "model_unitlist.h"

ModelUnitlist::ModelUnitlist (QObject *parent)
	: QAbstractTableModel (parent)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	header << "Unit";
}

ModelUnitlist::~ModelUnitlist (void)
{
	DBG_TRACE_DESTRUCT;
}

QVariant
ModelUnitlist::data (const QModelIndex &index, int role) const
{
	if (!d.data_bir || !d.data_cnt) return (QVariant());
	if (!index.isValid()) return (QVariant());

	return (MDLUL_data (role, index.row(), index.column()));
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
	return (d.data_bir ? d.data_cnt : 0);
}

int
ModelUnitlist::columnCount (const QModelIndex &/*parent*/) const
{
	return (1);
}

void
ModelUnitlist::load (SPWAW_DOSSIER_BIR *bir, int cnt)
{
	d.data_bir = bir;
	d.data_cnt = cnt;

	reset();
}

void
ModelUnitlist::clear (void)
{
	d.data_bir = NULL;
	d.data_cnt = 0;

	reset();
}

int
ModelUnitlist::max_width (void)
{
	int	max = 0;

	if (!d.data_bir) return (0);

	for (int i=0; i<d.data_cnt; i++) {
		int w = (MDLUL_data (Qt::DisplayRole, i, MDLUL_COLUMN_ID)).toString().length();
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
