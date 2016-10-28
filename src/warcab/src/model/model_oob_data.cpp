/** \file
 * The SPWaW war cabinet - data model handling - order of battle data.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "../common.h"
#include "model_sort.h"
#include "model_oob.h"
#include "model_oob_data.h"

static const char *MDLO_HILITE_names[MDLO_HILITE_LIMIT+1] = {
	"none",
	"type",
};

const char *
MDLO_HILITE_lookup (MDLO_HILITE e)
{
	return (MDLO_HILITE_names[e]);
}

QVariant
ModelOob::MDLO_data_form_display (int col, SPWAW_DOSSIER_FIR *fir, SPWDLT *dlt) const
{
	QVariant	v = QVariant();
	QString		s, d;

	if (!fir) return (v);

	switch (col) {
		case MDLO_COLUMN_FID:
			s = fir->snap->strings.name;
			break;
		case MDLO_COLUMN_TYPE:
			s = fir->snap->strings.utype;
			break;
		case MDLO_COLUMN_LDR:
			s = fir->snap->data.leader.up->strings.uid;
			break;
		case MDLO_COLUMN_HCMD:
			s = fir->snap->data.hcmd.up->strings.uid;
			break;
		case MDLO_COLUMN_STATUS:
			s = fir->snap->strings.fstatus;
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLO_COLUMN_KILLS:
			s.setNum (fir->snap->attr.gen.kills);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLO_COLUMN_RDY:
			s.sprintf ("%6.2f %%", fir->snap->attr.gen.ready * 100.0);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+6.2f", SPWDLT_getdbl (dlt) * 100.0); s += d; }
			break;
		case MDLO_COLUMN_COUNT:
			s.setNum (fir->snap->data.ucnt);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLO_COLUMN_EXP:
			s.sprintf ("%.2f", fir->snap->attr.mmas_exp.avg);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+.2f", SPWDLT_getdbl (dlt)); s += d; }
			break;
		case MDLO_COLUMN_MOR:
			s.sprintf ("%.2f", fir->snap->attr.mmas_mor.avg);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+.2f", SPWDLT_getdbl (dlt)); s += d; }
			break;
		case MDLO_COLUMN_SUP:
			s.sprintf ("%.2f", fir->snap->attr.mmas_sup.avg);
			//if (SPWDLT_check (dlt)) { d.sprintf (" %+.2f", SPWDLT_getdbl (dlt)); s += d; }
			break;
		case MDLO_COLUMN_RAL:
			s.sprintf ("%.2f", fir->snap->attr.mmas_ral.avg);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+.2f", SPWDLT_getdbl (dlt)); s += d; }
			break;
		case MDLO_COLUMN_INF:
			s.sprintf ("%.2f", fir->snap->attr.mmas_inf.avg);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+.2f", SPWDLT_getdbl (dlt)); s += d; }
			break;
		case MDLO_COLUMN_ARM:
			s.sprintf ("%.2f", fir->snap->attr.mmas_arm.avg);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+.2f", SPWDLT_getdbl (dlt)); s += d; }
			break;
		case MDLO_COLUMN_ART:
			s.sprintf ("%.2f", fir->snap->attr.mmas_art.avg);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+.2f", SPWDLT_getdbl (dlt)); s += d; }
			break;
		default:
			break;
	}
	if (!s.isNull()) v = QVariant (s);
	return (v);
}

QVariant
ModelOob::MDLO_data_form_foreground (int col, SPWAW_DOSSIER_FIR *fir, SPWDLT *dlt) const
{
	QVariant	v = QVariant();

	if (!fir) return (v);

	if (SPWDLT_check (dlt)) {
		switch (col) {
			case MDLO_COLUMN_KILLS:
			case MDLO_COLUMN_RDY:
			case MDLO_COLUMN_EXP:
			case MDLO_COLUMN_MOR:
			case MDLO_COLUMN_RAL:
			case MDLO_COLUMN_INF:
			case MDLO_COLUMN_ARM:
			case MDLO_COLUMN_ART:
				v = QBrush (*RES_color((SPWDLT_summary (dlt)<0) ? RID_GM_DLT_NEG : RID_GM_DLT_POS));
				break;
			case MDLO_COLUMN_STATUS:
			case MDLO_COLUMN_SUP:
				v = QBrush (*RES_color((SPWDLT_summary (dlt)<0) ? RID_GM_DLT_POS : RID_GM_DLT_NEG));
				break;
			default:
				v = QBrush (*RES_color(RID_GM_DLT_NTR));
				break;
		}
	} else {
		v = QBrush (*RES_color(RID_GMOC_FG_DEFAULT));
	}
	return (v);
}

QVariant
ModelOob::MDLO_data_form_background (int /*col*/, SPWAW_DOSSIER_FIR *fir, SPWDLT * /*dlt*/) const
{
	QVariant	v = QVariant();

	if (!fir) return (v);

	switch (d.hilite) {
		case MDLO_HILITE_NONE:
		default:
			v = QBrush (*RES_color(RID_GMOC_BG_DEFAULT));
			break;
		case MDLO_HILITE_TYPE:
			v = QBrush (*RES_GUI_color (fir->snap->data.type));
			break;
	}
	return (v);
}

QVariant
ModelOob::MDLO_data_form_decoration (int /*col*/, SPWAW_DOSSIER_FIR * /*fir*/, SPWDLT * /*dlt*/) const
{
	QVariant	v = QVariant();

	return (v);
}

QVariant
ModelOob::MDLO_data_form (int role, int col, SPWAW_DOSSIER_FIR *fir, SPWDLT *dlt) const
{
	QVariant	v = QVariant();

	if (!fir) return (v);

	switch (role) {
		case Qt::DisplayRole:
			v = MDLO_data_form_display (col, fir, dlt);
			break;
		case Qt::ForegroundRole:
			v = MDLO_data_form_foreground (col, fir, dlt);
			break;
		case Qt::BackgroundRole:
			v = MDLO_data_form_background (col, fir, dlt);
			break;
		case Qt::DecorationRole:
			v = MDLO_data_form_decoration (col, fir, dlt);
			break;
		default:
			break;
	}
	return (v);
}

QVariant
ModelOob::MDLO_data_unit_display (int col, SPWAW_DOSSIER_UIR *uir, SPWDLT *dlt) const
{
	QVariant	v = QVariant();
	QString		s, d;

	if (!uir) return (v);

	switch (col) {
		case MDLO_COLUMN_FID:
			s = uir->snap->strings.uid;
			break;
		case MDLO_COLUMN_TYPE:
			s = uir->snap->data.type;
			break;
		case MDLO_COLUMN_LDR:
			s = uir->snap->data.name;
			break;
		case MDLO_COLUMN_HCMD:
			s = (uir->snap->data.RID == uir->snap->data.formation->data.leader.up->data.RID) ? "*" : "";
			break;
		case MDLO_COLUMN_STATUS:
			s = uir->snap->strings.status;
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLO_COLUMN_KILLS:
			s.setNum (uir->snap->attr.gen.kills);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLO_COLUMN_RDY:
			s.sprintf ("%6.2f %%", uir->snap->attr.gen.ready * 100.0);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+6.2f", SPWDLT_getdbl (dlt) * 100.0); s += d; }
			break;
		case MDLO_COLUMN_COUNT:
			s.setNum (uir->snap->data.hcnt);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLO_COLUMN_EXP:
			s.setNum (uir->snap->data.exp);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLO_COLUMN_MOR:
			s.setNum (uir->snap->data.mor);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLO_COLUMN_SUP:
			s.setNum (uir->snap->data.sup);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLO_COLUMN_RAL:
			s.setNum (uir->snap->data.ral);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLO_COLUMN_INF:
			s.setNum (uir->snap->data.inf);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLO_COLUMN_ARM:
			s.setNum (uir->snap->data.arm);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLO_COLUMN_ART:
			s.setNum (uir->snap->data.art);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		default:
			break;
	}
	if (!s.isNull()) v = QVariant (s);
	return (v);
}

QVariant
ModelOob::MDLO_data_unit_foreground (int col, SPWAW_DOSSIER_UIR *uir, SPWDLT *dlt) const
{
	QVariant	v = QVariant();

	if (!uir) return (v);

	if (SPWDLT_check (dlt)) {
		switch (col) {
			case MDLO_COLUMN_KILLS:
			case MDLO_COLUMN_RDY:
			case MDLO_COLUMN_EXP:
			case MDLO_COLUMN_MOR:
			case MDLO_COLUMN_RAL:
			case MDLO_COLUMN_INF:
			case MDLO_COLUMN_ARM:
			case MDLO_COLUMN_ART:
				v = QBrush (*RES_color((SPWDLT_summary (dlt)<0) ? RID_GM_DLT_NEG : RID_GM_DLT_POS));
				break;
			case MDLO_COLUMN_STATUS:
			case MDLO_COLUMN_SUP:
				v = QBrush (*RES_color((SPWDLT_summary (dlt)<0) ? RID_GM_DLT_POS : RID_GM_DLT_NEG));
				break;
			default:
				v = QBrush (*RES_color(RID_GM_DLT_NTR));
				break;
		}
	} else {
		v = QBrush (*RES_color(RID_GMOC_FG_DEFAULT));
	}
	return (v);
}

QVariant
ModelOob::MDLO_data_unit_background (int /*col*/, SPWAW_DOSSIER_UIR * /*uir*/, SPWDLT * /*dlt*/) const
{
	QVariant	v = QVariant();

	v = QBrush (*RES_color(RID_GMOC_BG_DEFAULT));

	return (v);
}

QVariant
ModelOob::MDLO_data_unit_decoration (int /*col*/, SPWAW_DOSSIER_UIR * /*uir*/, SPWDLT * /*dlt*/) const
{
	QVariant	v = QVariant();

	return (v);
}

QVariant
ModelOob::MDLO_data_unit (int role, int col, SPWAW_DOSSIER_UIR *uir, SPWDLT *dlt) const
{
	QVariant	v = QVariant();

	if (!uir) return (v);

	switch (role) {
		case Qt::DisplayRole:
			v = MDLO_data_unit_display (col, uir, dlt);
			break;
		case Qt::ForegroundRole:
			v = MDLO_data_unit_foreground (col, uir, dlt);
			break;
		case Qt::BackgroundRole:
			v = MDLO_data_unit_background (col, uir, dlt);
			break;
		case Qt::DecorationRole:
			v = MDLO_data_unit_decoration (col, uir, dlt);
			break;
		default:
			break;
	}
	return (v);
}

QVariant
ModelOob::MDLO_data (int role, int col, MDLO_DATA *p) const
{
	QVariant	v = QVariant();

	if (!p) return (v);

	switch (p->type) {
		case MDLO_DATA_FORM:
			v = MDLO_data_form (role, col, p->data.f, &(p->dlt[col]));
			break;
		case MDLO_DATA_UNIT:
			v = MDLO_data_unit (role, col, p->data.u, &(p->dlt[col]));
			break;
		default:
			break;
	}
	return (v);
}

static void
MDLO_data_sort_form (SPWAW_DOSSIER_FIR *fir, int col, MDL_CMPDATA &v)
{
	if (!fir) return;

	switch (col) {
		case MDLO_COLUMN_FID:
			v.i = fir->snap->data.FID;
			break;
		case MDLO_COLUMN_TYPE:
			v.s = fir->snap->strings.type;
			break;
		case MDLO_COLUMN_LDR:
			v.i = fir->snap->data.leader.up->data.RID;
			break;
		case MDLO_COLUMN_HCMD:
			v.s = fir->snap->data.hcmd.up->strings.uid;
			break;
		case MDLO_COLUMN_STATUS:
			v.i = fir->snap->data.fstatus;
			break;
		case MDLO_COLUMN_KILLS:
			v.i = (unsigned int)fir->snap->attr.gen.kills;
			break;
		case MDLO_COLUMN_RDY:
			v.d = fir->snap->attr.gen.ready;
			break;
		case MDLO_COLUMN_COUNT:
			v.i = fir->snap->data.ucnt;
			break;
		case MDLO_COLUMN_EXP:
			v.d = fir->snap->attr.mmas_exp.avg;
			break;
		case MDLO_COLUMN_MOR:
			v.d = fir->snap->attr.mmas_mor.avg;
			break;
		case MDLO_COLUMN_SUP:
			v.d = fir->snap->attr.mmas_sup.avg;
			break;
		case MDLO_COLUMN_RAL:
			v.d = fir->snap->attr.mmas_ral.avg;
			break;
		case MDLO_COLUMN_INF:
			v.d = fir->snap->attr.mmas_inf.avg;
			break;
		case MDLO_COLUMN_ARM:
			v.d = fir->snap->attr.mmas_arm.avg;
			break;
		case MDLO_COLUMN_ART:
			v.d = fir->snap->attr.mmas_art.avg;
			break;
		default:
			break;
	}
}

void
MDLO_data_sort (int col, MDLO_DATA *p, MDL_CMPDATA &v)
{
	/* safe default value, effectively sets union memory to 0 */
	v.d = 0.0;

	if (!p) return;

	switch (p->type) {
		case MDLO_DATA_FORM:
			MDLO_data_sort_form (p->data.f, col, v);
			break;
		case MDLO_DATA_UNIT:
			break;
		default:
			break;
	}
}
