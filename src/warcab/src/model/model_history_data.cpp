/** \file
 * The SPWaW war cabinet - data model handling - unit history.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "../common.h"
#include "model_sort.h"
#include "model_history.h"
#include "model_history_data.h"

static const char *MDLH_HILITE_names[MDLH_HILITE_LIMIT+1] = {
	"none",
	"rank",
	"status",
	"entrenchment",
	"experience",
};

const char *
MDLH_HILITE_lookup (MDLH_HILITE e)
{
	return (MDLH_HILITE_names[e]);
}

QVariant
ModelHistory::MDLH_data_display (int /*row*/, int col, MDLH_DATA *data, SPWDLT *dlt) const
{
	QVariant	v = QVariant();
	QString		s, d;
	char		buf[17];

	if (!data || !dlt) return (v);

	switch (col) {
		case MDLH_COLUMN_DATE:
			if (this->d.campaign) {
				SPWAW_date2str (&(data->date.bdate.date), buf, sizeof (buf));
				if (SPWAW_isMonthOnlyDate(&(data->date.bdate.date))) {
					s.sprintf ("#%d %s", data->date.bdate.btlidx+1, buf);
				} else {
					s.sprintf ("%s", buf);
				}
			} else {
				SPWAW_date2str (&(data->date.tdate.date), buf, sizeof (buf));
				if (SPWAW_isMonthOnlyDate(&(data->date.tdate.date))) {
					s.sprintf ("%s, turn %u", buf, data->date.tdate.turn);
				} else {
					s.sprintf ("%s", buf);
				}
			}
			break;
		case MDLH_COLUMN_CFLAG:
			switch (data->cflag) {
				case MDLH_CFLAG_REPLACED:
					s = "N";
					break;
				case MDLH_CFLAG_REASSIGNED:
					s = "R";
					break;
				case MDLH_CFLAG_PROMOTED:
					s = "P";
					break;
				default:
					break;
			}
			break;
		case MDLH_COLUMN_UID:
			s = data->uir->snap->strings.uid;
			break;
		case MDLH_COLUMN_UNIT:
			s = data->uir->snap->data.type;
			break;
		case MDLH_COLUMN_RNK:
			s = data->uir->snap->strings.rank;
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_LDR:
			s = data->uir->snap->data.name;
			break;
		case MDLH_COLUMN_KILL:
			s.setNum (data->uir->snap->attr.gen.kills);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_EXP:
			s.setNum (data->uir->snap->data.exp);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_MOR:
			s.setNum (data->uir->snap->data.mor);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_RAL:
			s.setNum (data->uir->snap->data.ral);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_INF:
			s.setNum (data->uir->snap->data.inf);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_ARM:
			s.setNum (data->uir->snap->data.arm);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_ART:
			s.setNum (data->uir->snap->data.art);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_TYPE:
			s = data->uir->snap->strings.utype;
			break;
		case MDLH_COLUMN_CLASS:
			s = data->uir->snap->strings.uclass;
			break;
		case MDLH_COLUMN_RDY:
			s.sprintf ("%6.2f %%", data->uir->snap->attr.gen.ready * 100.0);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+6.2f", SPWDLT_getdbl (dlt) * 100.0); s += d; }
			break;
		case MDLH_COLUMN_SUP:
			s.setNum (data->uir->snap->data.sup);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_STATUS:
			s = data->uir->snap->strings.status;
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_SEEN:
			s = data->uir->snap->data.spotted ? "spotted" : "hidden";
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getbool (dlt)); s += d; }
			break;
		case MDLH_COLUMN_ABAND:
			switch (data->uir->snap->data.aband) {
				case SPWAW_ANONE:
					s = "no";
					break;
				case SPWAW_ASTAY:
					s = "yes";
					break;
				case SPWAW_ALEFT:
					s = "left";
					break;
				default:
					s = "???";
					break;
			}
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_LOADED:
			s = data->uir->snap->data.loaded ? "yes" : "no";
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getbool (dlt)); s += d; }
			break;
		case MDLH_COLUMN_DMG:
			s.setNum (data->uir->snap->data.damage);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_COST:
			s.setNum (data->uir->snap->data.cost);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_SPEED:
			s.setNum (data->uir->snap->data.speed);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		default:
			break;
	}
	if (!s.isNull()) v = QVariant (s);
	return (v);
}

QVariant
ModelHistory::MDLH_data_foreground (int /*row*/, int col, MDLH_DATA *data, SPWDLT *dlt) const
{
	QVariant	v = QVariant();

	if (!data || !dlt) return (v);

	if (SPWDLT_check (dlt)) {
		switch (col) {
			case MDLH_COLUMN_RNK:
			case MDLH_COLUMN_KILL:
			case MDLH_COLUMN_EXP:
			case MDLH_COLUMN_MOR:
			case MDLH_COLUMN_RAL:
			case MDLH_COLUMN_INF:
			case MDLH_COLUMN_ARM:
			case MDLH_COLUMN_ART:
			case MDLH_COLUMN_RDY:
			case MDLH_COLUMN_SPEED:
				v = QBrush (*RES_color((SPWDLT_summary (dlt)<0) ? RID_GM_DLT_NEG : RID_GM_DLT_POS));
				break;
			case MDLH_COLUMN_SUP:
			case MDLH_COLUMN_STATUS:
			case MDLH_COLUMN_SEEN:
			case MDLH_COLUMN_ABAND:
			case MDLH_COLUMN_DMG:
				v = QBrush (*RES_color((SPWDLT_summary (dlt)<0) ? RID_GM_DLT_POS : RID_GM_DLT_NEG));
				break;
			default:
				v = QBrush (*RES_color(RID_GM_DLT_NTR));
				break;
		}
	} else {
		v = QBrush (*RES_color(RID_GMRC_FG_DEFAULT));
		switch (col) {
			case MDLH_COLUMN_CFLAG:
				switch (data->cflag) {
					case MDLH_CFLAG_NONE:
						v = QBrush (*RES_color(RID_GMRC_FG_DEFAULT));
						break;
					case MDLH_CFLAG_REPLACED:
						v = QBrush (*RES_color(RID_GM_DLT_NEG));
						break;
					case MDLH_CFLAG_PROMOTED:
						v = QBrush (*RES_color(RID_GM_DLT_POS));
						break;
					default:
						v = QBrush (*RES_color(RID_GM_DLT_NTR));
						break;
				}
				break;

			default:
				v = QBrush (*RES_color(RID_GMRC_FG_DEFAULT));
				break;
		}
	}
	return (v);
}

QVariant
ModelHistory::MDLH_data_background (int /*row*/, int /*col*/, MDLH_DATA *data, SPWDLT *dlt) const
{
	QVariant	v = QVariant();

	if (!data || !dlt) return (v);

	switch (d.hilite) {
		case MDLH_HILITE_NONE:
		default:
			v = QBrush (*RES_color(RID_GMRC_BG_DEFAULT));
			break;
		case MDLH_HILITE_RANK:
			v = QBrush (*RES_GUI_color (data->uir->snap->data.rank));
			break;
		case MDLH_HILITE_USTATUS:
			v = QBrush (*RES_GUI_color (data->uir->snap->data.status));
			break;
		case MDLH_HILITE_ENTR:
			v = QBrush (*RES_GUI_color (data->uir->snap->data.entr));
			break;
		case MDLH_HILITE_EXP:
			v = QBrush (*RES_GUI_color (data->uir->snap->data.eclass));
			break;
	}
	return (v);
}

QVariant
ModelHistory::MDLH_data_decoration (int /*row*/, int col, MDLH_DATA *data, SPWDLT *dlt) const
{
	QVariant	v = QVariant();

	if (!data || !dlt) return (v);

	switch (col) {
		case MDLH_COLUMN_RNK:
			if (data->uir->snap->data.rank == SPWAW_RKIA)
				v = QVariant (QIcon (*RES_PIXMAP_NONE));
			break;
		default:
			break;
	}
	return (v);
}

QVariant
ModelHistory::MDLH_data (int role, int row, int col) const
{
	MDLH_DATA	*data = NULL;
	SPWDLT		*dlt = NULL;
	QVariant	v = QVariant();

	if ((row < 0) || (row >= d.row_cnt)) return (v);

	data = &(d.list[row]);
	if (!data->uir || !data->dlt) return (v);
	dlt = &(data->dlt[col]);

	switch (role) {
		case Qt::DisplayRole:
			v = MDLH_data_display (row, col, data, dlt);
			break;
		case Qt::ForegroundRole:
			v = MDLH_data_foreground (row, col, data, dlt);
			break;
		case Qt::BackgroundRole:
			v = MDLH_data_background (row, col, data, dlt);
			break;
		case Qt::DecorationRole:
			v = MDLH_data_decoration (row, col, data, dlt);
			break;
		default:
			break;
	}
	return (v);
}
