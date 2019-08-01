/** \file
 * The SPWaW war cabinet - data model handling - unit list.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"
#include "model_sort.h"
#include "model_unitlist.h"
#include "model_unitlist_data.h"

QVariant
ModelUnitlist::MDLUL_data (int role, int row, int col) const
{
	QVariant	v = QVariant();
	QString		s;

	if (!d.data_bir) return (v);
	if ((row < 0) || (row >= d.data_cnt)) return (v);
	if (role != Qt::DisplayRole) return (v);

	switch (col) {
		case MDLUL_COLUMN_ID:
			s.sprintf ("%s: %s",
				d.data_bir->uir[row].snap->strings.uid,
				d.data_bir->uir[row].snap->data.lname);
			break;
		default:
			break;
	}
	if (!s.isNull()) v = QVariant (s);
	return (v);
}
