/** \file
 * The SPWaW war cabinet - data model handling - tabled data.
 *
 * Copyright (C) 2016-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "model_table.h"
#include "plot/plot_timeline.h"

static MDLT_COLDEF default_coldef = { "N/A", MDLT_DATA_INT, 0 };

ModelTable::ModelTable (QObject *parent)
	: QAbstractTableModel (parent)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));
}

ModelTable::~ModelTable (void)
{
	DBG_TRACE_DESTRUCT;

	free_dat();
	free_def();
}

QVariant
ModelTable::data (const QModelIndex &index, int role) const
{
	if (!index.isValid()) return (QVariant());

	return (MDLT_data (role, index.row(), index.column()));
}

Qt::ItemFlags
ModelTable::flags (const QModelIndex &index) const
{
	if (!index.isValid()) return (0);

	return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

QVariant
ModelTable::headerData (int section, Qt::Orientation orientation, int role) const
{
	if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return (QVariant());

	return (header.value (section));
}

int
ModelTable::rowCount (const QModelIndex &/*parent*/) const
{
	return (d.dat.row_cnt);
}

int
ModelTable::columnCount (const QModelIndex &/*parent*/) const
{
	return (d.def.col_cnt);
}

void
ModelTable::setup (MDLT_DEF *def)
{
	free_dat();
	free_def();

	if (def) {
		memcpy (&(d.def), def, sizeof (d.def));

		if (def->col_cnt && def->col_lst) {
			SL_SAFE_CALLOC (d.def.col_lst, d.def.col_cnt, sizeof (MDLT_COLDEF));
			memcpy (d.def.col_lst, def->col_lst, d.def.col_cnt * sizeof (MDLT_COLDEF));

			for (int i=0; i<d.def.col_cnt; i++) {
				SL_SAFE_STRDUP (d.def.col_lst[i].name, def->col_lst[i].name);
				header << d.def.col_lst[i].name;
			}
		}
	}

	reset();
	d.updated_def = true;
}

void
ModelTable::load (MDLT_DATA *data)
{
	free_dat();

	if (data) {
		MDLT_alloc_data	(d.dat, data->row_cnt, d.def.col_cnt);
		int todo = data->col_cnt; if (todo > d.def.col_cnt) todo = d.def.col_cnt;

		for (int i=0; i<d.dat.row_cnt; i++) {
			d.dat.row[i].bg = data->row[i].bg;
			for (int j=0; j<todo; j++) {
				d.dat.row[i].data[j] = data->row[i].data[j];
			}
		}
	}

	reset();
	d.updated_dat = true;
}

void
ModelTable::clear (void)
{
	free_dat();
	reset();
	d.updated_dat = true;
}

void
ModelTable::commit (void)
{
	int	test = 0;

	if (d.updated_def) test |= 0x1;
	if (d.updated_dat) test |= 0x2;

	switch (test) {
		case 0x0:
			break;
		case 0x1:
			emit def_updated();
			break;
		case 0x2:
			emit dat_updated();
			break;
		case 0x3:
			emit model_updated();
			break;
	}
}

MDLT_DEF *
ModelTable::def_info (void)
{
	return (&(d.def));
}

MDLT_COLDEF *
ModelTable::col_info (int col)
{
	if ((col < 0) || (col >= d.def.col_cnt)) return (&default_coldef);
	return (&(d.def.col_lst[col]));
}

void
ModelTable::free_def (void)
{
	int	i;

	header.clear();

	for (i=0; i<d.def.col_cnt; i++) SL_SAFE_FREE (d.def.col_lst[i].name);
	if (d.def.col_lst) SL_SAFE_FREE (d.def.col_lst);

	memset (&(d.def), 0, sizeof (d.def));
	d.updated_def = true;
}

void
ModelTable::free_dat (void)
{
	MDLT_free_data (d.dat);
	d.updated_dat = true;
}

QVariant
ModelTable::MDLT_data_display (QVariant *item, MDLT_DATA_TYPE type) const
{
	QVariant	v = QVariant();
	QString		s;

	if (!item) return (v);
	if (item->isNull()) return (v);

	switch (type) {
			case MDLT_DATA_INT:
				s.sprintf ("%d", item->toInt());
				break;
			case MDLT_DATA_DBL:
				s.sprintf ("%f", item->toDouble());
				break;
			case MDLT_DATA_PERC:
				s.sprintf ("%6.2f %", item->toDouble() * 100.0);
				break;
			case MDLT_DATA_STR:
				s.sprintf ("%s", qPrintable(item->toString()));
				break;
			default:
				break;
	}
	if (!s.isNull()) v = QVariant (s);
	return (v);
}

QVariant
ModelTable::MDLT_data_foreground (QVariant *item, MDLT_DATA_TYPE /*type*/) const
{
	QVariant	v = QVariant();

	if (!item) return (v);

	v = QBrush (*RES_color(RID_GMTC_FG_DEFAULT));
	return (v);

}

QVariant
ModelTable::MDLT_data_background (QVariant *item, MDLT_DATA_TYPE /*type*/, QColor col) const
{
	QVariant	v = QVariant();

	if (!item) return (v);

	v = QBrush (col);
	return (v);
}

QVariant
ModelTable::MDLT_data (int role, int row, int col) const
{
	QVariant	*item = NULL;
	MDLT_DATA_TYPE	type;
	QVariant	v = QVariant();
	QString		s;

	if (!d.dat.row) return (v);
	if ((row < 0) || (row >= d.dat.row_cnt)) return (v);
	if ((col < 0) || (col >= d.def.col_cnt)) return (v);

	item = &(d.dat.row[row].data[col]);
	type = d.def.col_lst[col].type;

	switch (role) {
		case Qt::DisplayRole:
			v = MDLT_data_display (item, type);
			break;
		case Qt::ForegroundRole:
			v = MDLT_data_foreground (item, type);
			break;
		case Qt::BackgroundRole:
			v = MDLT_data_background (item, type, d.dat.row[row].bg);
			break;
		default:
			break;
	}
	return (v);
}

void
MDLT_alloc_data	(MDLT_DATA &data, int row_cnt, int col_cnt)
{
	int	i;

	data.row_cnt = row_cnt; data.col_cnt = col_cnt;
	SL_SAFE_CALLOC (data.row, data.row_cnt, sizeof (*(data.row)));
	for (i=0; i<data.row_cnt; i++) {
		SL_SAFE_CALLOC (data.row[i].data, data.col_cnt, sizeof (*(data.row[i].data)));
	}
}

void
MDLT_free_data	(MDLT_DATA &data)
{
	int	i;

	for (i=0; i<data.row_cnt; i++) SL_SAFE_FREE (data.row[i].data);
	if (data.row) SL_SAFE_FREE (data.row);
	memset (&data, 0, sizeof (data));
}
