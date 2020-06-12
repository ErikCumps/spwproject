/** \file
 * The SPWaW war cabinet - data model handling - unit history.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
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

#define	IF_NOT_FULL_INTEL(v_)							\
	do {									\
		if (!this->d.pflag && (this->d.intel_mode != INTEL_MODE_FULL))	\
			return ((v_));						\
	} while (0)

QVariant
ModelHistory::MDLH_data_display (int /*row*/, int col, MDLH_DATA *data, SPWDLT *dlt) const
{
	QVariant	v = QVariant();
	QString		s, d;
	char		buf[17];

	if (!data || !dlt) return (v);

	if (col == MDLH_COLUMN_DATE) {
			if (this->d.campaign) {
				SPWAW_bdate2str (&(data->date.bdate), buf, sizeof (buf), false);
			} else {
				SPWAW_tdate2str (&(data->date.tdate), buf, sizeof (buf), false);
			}
			s.sprintf ("%s", buf);
			return (QVariant (s));
	}

	if (!data->uir) return (v);

	switch (col) {
		case MDLH_COLUMN_UID:
			s = data->uir->snap->strings.uid;
			break;
		case MDLH_COLUMN_UNIT: // FIXME: designation, actually
			s = data->uir->snap->data.dname;
			break;
		case MDLH_COLUMN_RNK:
			s = data->uir->snap->strings.rank;
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_LDR:
			s = data->uir->snap->data.lname;
			break;
		case MDLH_COLUMN_STATUS:
			IF_NOT_FULL_INTEL(v);
			s = data->uir->snap->strings.status;
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_KILL:
			IF_NOT_FULL_INTEL(v);
			s.setNum (data->uir->snap->attr.gen.kills);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_EXP:
			IF_NOT_FULL_INTEL(v);
			s.setNum (data->uir->snap->data.exp);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_MOR:
			IF_NOT_FULL_INTEL(v);
			s.setNum (data->uir->snap->data.mor);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_SUP:
			IF_NOT_FULL_INTEL(v);
			s.setNum (data->uir->snap->data.sup);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_RAL:
			IF_NOT_FULL_INTEL(v);
			s.setNum (data->uir->snap->data.ral);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_INF:
			IF_NOT_FULL_INTEL(v);
			s.setNum (data->uir->snap->data.inf);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_ARM:
			IF_NOT_FULL_INTEL(v);
			s.setNum (data->uir->snap->data.arm);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_ART:
			IF_NOT_FULL_INTEL(v);
			s.setNum (data->uir->snap->data.art);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_MEN:
			IF_NOT_FULL_INTEL(v);
			s.setNum (data->uir->snap->data.hcnt);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_RDY:
			IF_NOT_FULL_INTEL(v);
			s.sprintf ("%6.2f %%", data->uir->snap->attr.gen.ready * 100.0);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+6.2f", SPWDLT_getdbl (dlt) * 100.0); s += d; }
			break;
		case MDLH_COLUMN_KIA:
			IF_NOT_FULL_INTEL(v);
			s.setNum (data->uir->snap->data.hcnt - data->uir->snap->data.hcnt_left);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_DMG:
			IF_NOT_FULL_INTEL(v);
			s.setNum (data->uir->snap->data.damage);
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
			IF_NOT_FULL_INTEL(v);
			s = data->uir->snap->data.loaded ? "yes" : "no";
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getbool (dlt)); s += d; }
			break;
		case MDLH_COLUMN_TYPE:
			s = data->uir->snap->strings.utype;
			break;
		case MDLH_COLUMN_CLASS:
			s = data->uir->snap->strings.uclass;
			break;
		case MDLH_COLUMN_COST:
			s.setNum (data->uir->snap->data.cost);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_SPEED:
			IF_NOT_FULL_INTEL(v);
			s.setNum (data->uir->snap->data.speed);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLH_COLUMN_DATE:
		default:
			break;
	}
	if (!s.isNull()) v = QVariant (s);
	return (v);
}

QVariant
ModelHistory::MDLH_data_font (int /*row*/, int /*col*/, MDLH_DATA *data, SPWDLT * /*dlt*/) const
{
	QVariant	v = QVariant();

	if (d.rgfont) v = *d.rgfont;

	if (!data) return (v);
	IF_NOT_FULL_INTEL(v);

	if (data->decomm) {
		if (d.dcfont) v = *d.dcfont;
	}
	return (v);
}

QVariant
ModelHistory::MDLH_data_foreground (int /*row*/, int col, MDLH_DATA *data, SPWDLT *dlt) const
{
	QVariant	v = QVariant(QBrush (*RES_color(RID_GMRC_FG_DEFAULT)));

	if (!data || !dlt) return (v);
	IF_NOT_FULL_INTEL(v);

	if (data->decomm) {
		v = QBrush (*RES_color(RID_GM_DLT_INA));
	} else  if (SPWDLT_check (dlt)) {
		switch (col) {
			case MDLH_COLUMN_RNK:
			case MDLH_COLUMN_KILL:
			case MDLH_COLUMN_EXP:
			case MDLH_COLUMN_MOR:
			case MDLH_COLUMN_RAL:
			case MDLH_COLUMN_INF:
			case MDLH_COLUMN_ARM:
			case MDLH_COLUMN_ART:
			case MDLH_COLUMN_MEN:
			case MDLH_COLUMN_RDY:
			case MDLH_COLUMN_SPEED:
				v = QBrush (*RES_color((SPWDLT_summary (dlt)<0) ? RID_GM_DLT_NEG : RID_GM_DLT_POS));
				break;
			case MDLH_COLUMN_STATUS:
			case MDLH_COLUMN_SUP:
			case MDLH_COLUMN_KIA:
			case MDLH_COLUMN_DMG:
			case MDLH_COLUMN_SEEN:
			case MDLH_COLUMN_ABAND:
				v = QBrush (*RES_color((SPWDLT_summary (dlt)<0) ? RID_GM_DLT_POS : RID_GM_DLT_NEG));
				break;
			default:
				v = QBrush (*RES_color(RID_GM_DLT_NTR));
				break;
		}
	}
	return (v);
}

QVariant
ModelHistory::MDLH_data_background (int /*row*/, int /*col*/, MDLH_DATA *data, SPWDLT *dlt) const
{
	QVariant	v = QVariant(QBrush (*RES_color(RID_GMRC_BG_DEFAULT)));

	if (!data || !dlt) return (v);

	switch (d.hilite) {
		case MDLH_HILITE_NONE:
		default:
			if (data->uir == NULL) v = QBrush (*RES_color(RID_RGB_GRAY_75));
			break;
		case MDLH_HILITE_RANK:
			v = QBrush (*RES_GUI_color (data->uir->snap->data.rank));
			break;
		case MDLH_HILITE_USTATUS:
			IF_NOT_FULL_INTEL(v);
			v = QBrush (*RES_GUI_color (data->uir->snap->data.status));
			break;
		case MDLH_HILITE_ENTR:
			IF_NOT_FULL_INTEL(v);
			v = QBrush (*RES_GUI_color (data->uir->snap->data.entr));
			break;
		case MDLH_HILITE_EXP:
			IF_NOT_FULL_INTEL(v);
			v = QBrush (*RES_GUI_color (data->uir->snap->data.eclass));
			break;
	}
	return (v);
}

QVariant
ModelHistory::MDLH_data_decoration (int /*row*/, int col, MDLH_DATA *data, SPWDLT * /*dlt*/) const
{
	QVariant	v = QVariant();

	if (!data) return (v);

	switch (col) {
		case MDLH_COLUMN_UID:
			if (d.mflag && data->uhte) {
				if (SPWAW_UHT_is_reassigned (data->uhte, &(data->date.bdate)))
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_REASSIGNED)));
				else
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_EMPTY)));
			}
			break;
		case MDLH_COLUMN_UNIT:
			if (d.mflag && data->uhte) {
				if (SPWAW_UHT_is_upgraded (data->uhte, &(data->date.bdate)))
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_UPGRADED)));
				else
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_EMPTY)));
			}
			break;
		case MDLH_COLUMN_RNK:
			if (d.mflag && data->uhte) {
				if (SPWAW_UHT_is_promoted (data->uhte, &(data->date.bdate)))
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_PROMOTED)));
				else if (SPWAW_UHT_is_demoted (data->uhte, &(data->date.bdate)))
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_DEMOTED)));
				else
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_EMPTY)));
			}
			break;
		case MDLH_COLUMN_LDR:
			if (d.mflag && data->uhte) {
				if (SPWAW_UHT_is_replaced (data->uhte, &(data->date.bdate)))
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_REPLACED)));
				else
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_EMPTY)));
			}
			break;
		case MDLH_COLUMN_STATUS:
			if (data->uir) {
				if (!data->uir->snap->data.alive)
					v = QVariant (QIcon (*RES_pixmap (RID_ICON_UHT_DESTROYED)));
				else
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_EMPTY)));
			}
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
	if (!data->dlt) return (v);
	dlt = &(data->dlt[col]);

	switch (role) {
		case Qt::DisplayRole:
			v = MDLH_data_display (row, col, data, dlt);
			break;
		case Qt::FontRole:
			v = MDLH_data_font (row, col, data, dlt);
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
