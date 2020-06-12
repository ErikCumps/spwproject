/** \file
 * The SPWaW war cabinet - data model handling - min-max-average-spread data.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "model_mmas.h"
#include "model_mmas_data.h"
#include "plot/plot_timeline.h"

static const char *MDLMMAS_TYPE_names[MDLMMAS_TYPE_LIMIT+1] = {
	"experience",
	"morale",
	"suppression",
	"rally rating",
	"infantry rating",
	"armor rating",
	"artillery rating",
	"readiness",
	"kills",
	"cost",
	"speed",
};

const char *
MDLMMAS_TYPE_lookup (MDLMMAS_TYPE e)
{
	return (MDLMMAS_TYPE_names[e]);
}

#define	IF_NOT_FULL_INTEL(v_)							\
	do {									\
		if (!this->d.pflag && (this->d.intel_mode != INTEL_MODE_FULL))	\
			return ((v_));						\
	} while (0)

QVariant
ModelMMAS::MDLMMAS_data_display (int /*row*/, int col, MDLMMAS_DATA *data, SPWDLT *dlt) const
{
	QVariant		v = QVariant();
	MDLMMAS_COLUMN_DEF	*cd;
	QString			s, d;
	char			buf[17];

	if (!data || !dlt) return (v);

	cd = MDLMMAS_coldef (col);
	switch (col) {
		case MDLMMAS_COLUMN_DATE:
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
		case MDLMMAS_COLUMN_MAXID:
		case MDLMMAS_COLUMN_MINID:
			break;
		default:
			IF_NOT_FULL_INTEL(v);
			switch (cd->dtype) {
				case SPWDLT_INT:
					s.setNum (SPWDLT_int (dlt));
					if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
					break;
				case SPWDLT_DBL:
					s.sprintf ("%.2f", SPWDLT_dbl (dlt));
					if (SPWDLT_check (dlt)) { d.sprintf (" %+.2f", SPWDLT_getdbl (dlt)); s += d; }
					break;
				default:
					break;
			}
			break;
	}
	if (!s.isNull()) v = QVariant (s);
	return (v);
}

QVariant
ModelMMAS::MDLMMAS_data_foreground (int /*row*/, int col, MDLMMAS_DATA *data, SPWDLT *dlt) const
{
	QVariant	v = QVariant(QBrush (*RES_color(RID_GMSC_FG_DEFAULT)));

	if (!data || !dlt) return (v);
	IF_NOT_FULL_INTEL(v);

	if (SPWDLT_check (dlt)) {
		switch (col) {
			case MDLMMAS_COLUMN_MAX:
			case MDLMMAS_COLUMN_MIN:
			case MDLMMAS_COLUMN_AVG:
				v = QBrush (*RES_color((SPWDLT_summary (dlt)<0) ? RID_GM_DLT_NEG : RID_GM_DLT_POS));
				break;
			case MDLMMAS_COLUMN_SPR:
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
ModelMMAS::MDLMMAS_data_background (int /*row*/, int /*col*/, MDLMMAS_DATA * /*data*/, SPWDLT * /*dlt*/) const
{
	QVariant	v = QVariant(QBrush (*RES_color(RID_GMSC_BG_DEFAULT)));

	return (v);
}

QVariant
ModelMMAS::MDLMMAS_data (int role, int row, int col) const
{
	MDLMMAS_DATA	*data = NULL;
	SPWDLT		*dlt = NULL;
	QVariant	v = QVariant();

	if ((row < 0) || (row >= d.row_cnt)) return (v);

	data = &(d.list[row]);
	if (!data->force || !data->dlt) return (v);
	dlt = &(data->dlt[col]);

	switch (role) {
		case Qt::DisplayRole:
			v = MDLMMAS_data_display (row, col, data, dlt);
			break;
		case Qt::ForegroundRole:
			v = MDLMMAS_data_foreground (row, col, data, dlt);
			break;
		case Qt::BackgroundRole:
			v = MDLMMAS_data_background (row, col, data, dlt);
			break;
		default:
			break;
	}
	return (v);
}

double
ModelMMAS::MDLMMAS_data_plot (int /*row*/, int col, MDLMMAS_DATA *data, SPWDLT *dlt) const
{
	double	v = 0.0;

	if (!data || !dlt) return (v);

	switch (col) {
		case MDLMMAS_COLUMN_DATE:
			if (d.campaign) {
				PLOT_date2timeline (data->date.bdate, v);
			} else {
				PLOT_date2timeline (data->date.tdate, v);
			}
			break;
		case MDLMMAS_COLUMN_MAX:
		case MDLMMAS_COLUMN_MIN:
		case MDLMMAS_COLUMN_AVG:
		case MDLMMAS_COLUMN_SPR:
		case MDLMMAS_COLUMN_COUNT:
			IF_NOT_FULL_INTEL(v);
			switch ((MDLMMAS_coldef(col))->dtype) {
				case SPWDLT_INT:
					v = SPWDLT_int (dlt);
					break;
				case SPWDLT_DBL:
					v = SPWDLT_dbl (dlt);
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	return (v);
}

SPWAW_SNAP_OOB_UEL *
ModelMMAS::MDLMMAS_data_id (int /*row*/, int col, MDLMMAS_DATA *data, SPWDLT *dlt) const
{
	SPWAW_SNAP_OOB_UEL	*v = NULL;
	int			i = -1;

	if (!data || !dlt) return (v);
	IF_NOT_FULL_INTEL(v);

	switch (col) {
		case MDLMMAS_COLUMN_MAXID:
		case MDLMMAS_COLUMN_MINID:
			i = SPWDLT_int (dlt);
			break;
		default:
			break;
	}
	if (i != -1) {
		v = &(data->force->units.list[i]);
	}
	return (v);
}
