/** \file
 * The SPWaW war cabinet - data model handling - unit list.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "../common.h"
#include "model_sort.h"
#include "model_unitlist.h"
#include "model_unitlist_data.h"

QVariant
ModelUnitlist::MDLU_data_display (int /*row*/, int col, MDLU_DATA *data) const
{
	QVariant		v = QVariant();
	SPWAW_DOSSIER_UIR	*uir;
	QString			s;

	if (!data) return (v);
	uir = data->uir;

	switch (col) {
		case MDLU_COLUMN_ID:
			if (d.d != NULL)
				s.sprintf ("%s", uir->snap->strings.uid);
			else
				s.sprintf ("%s: %s", uir->snap->strings.uid, uir->snap->data.lname);
			break;
		default:
			break;
	}
	if (!s.isNull()) v = QVariant (s);
	return (v);
}

QVariant
ModelUnitlist::MDLU_data_font (int /*row*/, int /*col*/, MDLU_DATA *data) const
{
	QVariant	v = QVariant();

	if (!data) return (v);

	if (!data->decomm) {
		if (d.rgfont) v = *d.rgfont;
	} else {
		if (d.dcfont) v = *d.dcfont;
	}
	return (v);
}

QVariant
ModelUnitlist::MDLU_data_foreground (int /*row*/, int /*col*/, MDLU_DATA *data) const
{
	QVariant		v = QVariant();

	if (!data) return (v);

	if (data->decomm) {
		v = QBrush (*RES_color(RID_GM_DLT_INA));
	} else {
		v = QBrush (*RES_color(RID_GMRC_FG_DEFAULT));
	}
	return (v);
}

QVariant
ModelUnitlist::MDLU_data_background (int /*row*/, int /*col*/, MDLU_DATA *data) const
{
	if (!data) return (QVariant());
	return (QBrush (*RES_color(RID_GMRC_BG_DEFAULT)));
}

QVariant
ModelUnitlist::MDLU_data_decoration (int /*row*/, int col, MDLU_DATA *data) const
{
	QVariant		v = QVariant();

	if (!data) return (v);

	switch (col) {
		case MDLU_COLUMN_ID:
			if (d.cdflag) {
				if (data->decomm)
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_ULV_DECOMMISSIONED)));
				else if (data->comm)
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_ULV_COMMISSIONED)));
				else
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_ULV_EMPTY)));
			}
			break;
		default:
			break;
	}
	return (v);
}

QVariant
ModelUnitlist::MDLU_data (int role, int row, int col) const
{
	QVariant	v = QVariant();

	if ((row < 0) || (row >= d.row_cnt)) return (v);

	switch (role) {
		case Qt::DisplayRole:
			v = MDLU_data_display (row, col, &(d.list[row]));
			break;
		case Qt::FontRole:
			v= MDLU_data_font (row, col, &(d.list[row]));
			break;
		case Qt::ForegroundRole:
			v = MDLU_data_foreground (row, col, &(d.list[row]));
			break;
		case Qt::BackgroundRole:
			v = MDLU_data_background (row, col, &(d.list[row]));
			break;
		case Qt::DecorationRole:
			v = MDLU_data_decoration (row, col, &(d.list[row]));
			break;
		case Qt::TextAlignmentRole:
			v = QVariant (Qt::AlignVCenter);
			break;
		case MDLU_UIDX_ROLE:
			v = QVariant (d.list[row].uidx);
			break;
		default:
			break;
	}
	return (v);
}
