/** \file
 * The SPWaW war cabinet - data model handling - tabled data.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "model_table.h"
#include "plot/plot_timeline.h"

static MDLT_COLDEF default_coldef = { "N/A", MDLT_DATA_INT, 0, NULL };

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
	int	i;

	free_dat();
	free_def();

	if (def) {
		memcpy (&(d.def), def, sizeof (d.def));

		if (def->title) SL_SAFE_STRDUP (d.def.title, def->title);

		if (def->col_cnt && def->col_lst) {
			SL_SAFE_CALLOC (d.def.col_lst, d.def.col_cnt, sizeof (MDLT_COLDEF));
			memcpy (d.def.col_lst, def->col_lst, d.def.col_cnt * sizeof (MDLT_COLDEF));

			for (i=0; i<d.def.col_cnt; i++) {
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
	int		todo;
	int		i, j;

	free_dat();

	if (data) {
		MDLT_alloc_data	(d.dat, data->row_cnt, d.def.col_cnt);
		todo = data->col_cnt; if (todo > d.def.col_cnt) todo = d.def.col_cnt;

		for (i=0; i<d.dat.row_cnt; i++) {
			for (j=0; j<todo; j++) {
				d.dat.row[i].data[j] = data->row[i].data[j];
				if (!j) continue;

				SPWDLT_prep (	&(d.dat.row[i].data[j].dlt),
						dlt_type (d.dat.row[i].data[j].type),
						&(d.dat.row[i].data[j].u),
						i ? &(d.dat.row[i-1].data[j].u) : NULL,
						0);
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
ModelTable::plot_data (int col, double *dst, int cnt)
{
	int	i;
	double	dv;

	if (!dst || !cnt) return;
	memset (dst, 0, sizeof (*dst) * cnt);

	if ((col < 0) || (col >= d.def.col_cnt)) return;

	if (cnt > d.dat.row_cnt) cnt = d.dat.row_cnt;

	for (i = 0; i < cnt; i++) {
		switch (d.dat.row[i].data[col].type) {
			case MDLT_DATA_DATE:
				PLOT_date2timeline (d.dat.row[i].data[col].u.date, dv);
				break;
			case MDLT_DATA_INT:
				dv = (double)d.dat.row[i].data[col].u.i;
				break;
			default:
				dv = 0.0;
				break;
		}
		dst[i] = dv;
	}
}

void
ModelTable::free_def (void)
{
	int	i;

	header.clear();

	if (d.def.title) SL_SAFE_FREE (d.def.title);
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
ModelTable::MDLT_data_display (MDLT_DATA_ITEM *item) const
{
	QVariant	v = QVariant();
	QString		s, d;
	char		buf[32];

	if (!item) return (v);

	switch (item->type) {
			case MDLT_DATA_DATE:
				SPWAW_date2str (&(item->u.date), buf, sizeof (buf));
				s.sprintf ("%s", buf);
				break;
			case MDLT_DATA_INT:
				s.setNum (item->u.i);
				if (SPWDLT_check (&(item->dlt))) { d.sprintf (" %+d", SPWDLT_getint (&(item->dlt))); s += d; }
				break;
			default:
				break;
	}
	if (!s.isNull()) v = QVariant (s);
	return (v);
}

QVariant
ModelTable::MDLT_data_foreground (MDLT_DATA_ITEM *item) const
{
	QVariant	v = QVariant();

	if (!item) return (v);

	if (SPWDLT_check (&(item->dlt))) {
		v = QBrush (*RES_color((SPWDLT_summary (&(item->dlt))<0) ? RID_GM_DLT_NEG : RID_GM_DLT_POS));
	} else {
		v = QBrush (*RES_color(RID_GMTC_FG_DEFAULT));
	}
	return (v);

}

QVariant
ModelTable::MDLT_data (int role, int row, int col) const
{
	MDLT_DATA_ITEM	*item = NULL;
	QVariant	v = QVariant();
	QString		s;

	if (!d.dat.row) return (v);
	if ((row < 0) || (row >= d.dat.row_cnt)) return (v);
	if ((col < 0) || (col >= d.def.col_cnt)) return (v);

	item = &(d.dat.row[row].data[col]);

	switch (role) {
		case Qt::DisplayRole:
			v = MDLT_data_display (item);
			break;
		case Qt::ForegroundRole:
			v = MDLT_data_foreground (item);
			break;
		default:
			break;
	}
	return (v);
}

SPWDLT_TYPE
ModelTable::dlt_type (MDLT_DATA_TYPE t)
{
	SPWDLT_TYPE	type = SPWDLT_NONE;

	switch (t) {
		case MDLT_DATA_INT:
			type = SPWDLT_INT;
			break;
		default:
			break;
	}

	return (type);
}

void
MDLT_alloc_data	(MDLT_DATA &data, int row_cnt, int col_cnt)
{
	int	i;

	data.row_cnt = row_cnt; data.col_cnt = col_cnt;
	SL_SAFE_CALLOC (data.row, data.row_cnt, sizeof (MDLT_DATA_ROW));
	for (i=0; i<data.row_cnt; i++) {
		SL_SAFE_CALLOC (data.row[i].data, data.col_cnt, sizeof (MDLT_DATA_ITEM));
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
