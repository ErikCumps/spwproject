/** \file
 * The SPWaW war cabinet - data model handling - unit data.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "../common.h"
#include "model_sort.h"
#include "model_roster.h"
#include "model_roster_data.h"

static const char *MDLR_FILTER_names[MDLR_FILTER_LIMIT+1] = {
	"none",
	"unit class",
	"unit type",
	"unit name",
	"rank",
	"experience",
	"status",
	"entrenchment",
	"readiness",
	"suppression",
	"losses",
	"damage",
	"abandoned",
	"spotted",
	"loaded",
};
const char *
MDLR_FILTER_lookup (MDLR_FILTER e)
{
	return (MDLR_FILTER_names[e]);
}

static const char *MDLR_HILITE_names[MDLR_HILITE_LIMIT+1] = {
	"none",
	"rank",
	"status",
	"entrenchment",
	"experience",
};
const char *
MDLR_HILITE_lookup (MDLR_HILITE e)
{
	return (MDLR_HILITE_names[e]);
}

QVariant
ModelRoster::MDLR_data_display (int /*row*/, int col, MDLR_DATA *data, SPWDLT *dlt) const
{
	QVariant		v = QVariant();
	SPWAW_DOSSIER_UIR	*uir;
	QString			s, d;

	if (!data) return (v);
	uir = data->uir;

	switch (col) {
		case MDLR_COLUMN_UID:
			s = uir->snap->strings.uid;
			break;
		case MDLR_COLUMN_UNIT: // FIXME: designation, actually
			s = uir->snap->data.dname;
			break;
		case MDLR_COLUMN_RNK:
			s = uir->snap->strings.rank;
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLR_COLUMN_LDR:
			s = uir->snap->data.lname;
			break;
		case MDLR_COLUMN_TYPE:
			s = uir->snap->strings.utype;
			break;
		case MDLR_COLUMN_CLASS:
			s = uir->snap->strings.uclass;
			break;
		case MDLR_COLUMN_KILL:
			s.setNum (uir->snap->attr.gen.kills);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLR_COLUMN_STATUS:
			s = uir->snap->strings.status;
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLR_COLUMN_SEEN:
			s = uir->snap->data.spotted ? "spotted" : "hidden";
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getbool (dlt)); s += d; }
			break;
		case MDLR_COLUMN_EXP:
			s.setNum (uir->snap->data.exp);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLR_COLUMN_MOR:
			s.setNum (uir->snap->data.mor);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLR_COLUMN_SUP:
			s.setNum (uir->snap->data.sup);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLR_COLUMN_RAL:
			s.setNum (uir->snap->data.ral);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLR_COLUMN_INF:
			s.setNum (uir->snap->data.inf);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLR_COLUMN_ARM:
			s.setNum (uir->snap->data.arm);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLR_COLUMN_ART:
			s.setNum (uir->snap->data.art);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLR_COLUMN_MEN:
			s.setNum (uir->snap->data.hcnt);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLR_COLUMN_KIA:
			s.setNum (uir->snap->data.hcnt - uir->snap->data.hcnt_left);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLR_COLUMN_DMG:
			s.setNum (uir->snap->data.damage);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLR_COLUMN_RDY:
			s.sprintf ("%6.2f %%", uir->snap->attr.gen.ready * 100.0);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+6.2f", SPWDLT_getdbl (dlt) * 100.0); s += d; }
			break;
		case MDLR_COLUMN_ABAND:
			switch (uir->snap->data.aband) {
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
		case MDLR_COLUMN_LOADED:
			s = uir->snap->data.loaded ? "yes" : "no";
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getbool (dlt)); s += d; }
			break;
		case MDLR_COLUMN_COST:
			s.setNum (uir->snap->data.cost);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		case MDLR_COLUMN_SPEED:
			s.setNum (uir->snap->data.speed);
			if (SPWDLT_check (dlt)) { d.sprintf (" %+d", SPWDLT_getint (dlt)); s += d; }
			break;
		default:
			break;
	}
	if (!s.isNull()) v = QVariant (s);
	return (v);
}

QVariant
ModelRoster::MDLR_data_font (int /*row*/, int /*col*/, MDLR_DATA *data, SPWDLT * /*dlt*/) const
{
	QVariant	v = QVariant();

	if (!data) return (v);

	if (data->decomm) {
		// TODO: this should be parameterized?
		v = QFont ("Courier", 8, QFont::Normal, true);
	}
	return (v);
}

QVariant
ModelRoster::MDLR_data_foreground (int /*row*/, int col, MDLR_DATA *data, SPWDLT *dlt) const
{
	QVariant		v = QVariant();
	SPWAW_DOSSIER_UIR	*uir;

	if (!data) return (v);
	uir = data->uir;

	if (data->decomm) {
		v = QBrush (*RES_color(RID_GM_DLT_INA));
	} else if (SPWDLT_check (dlt)) {
		switch (col) {
			case MDLR_COLUMN_RNK:
			case MDLR_COLUMN_KILL:
			case MDLR_COLUMN_EXP:
			case MDLR_COLUMN_MOR:
			case MDLR_COLUMN_RAL:
			case MDLR_COLUMN_INF:
			case MDLR_COLUMN_ARM:
			case MDLR_COLUMN_ART:
			case MDLR_COLUMN_MEN:
			case MDLR_COLUMN_RDY:
			case MDLR_COLUMN_SPEED:
				v = QBrush (*RES_color((SPWDLT_summary (dlt)<0) ? RID_GM_DLT_NEG : RID_GM_DLT_POS));
				break;
			case MDLR_COLUMN_STATUS:
			case MDLR_COLUMN_SEEN:
			case MDLR_COLUMN_SUP:
			case MDLR_COLUMN_KIA:
			case MDLR_COLUMN_DMG:
			case MDLR_COLUMN_ABAND:
				v = QBrush (*RES_color((SPWDLT_summary (dlt)<0) ? RID_GM_DLT_POS : RID_GM_DLT_NEG));
				break;
			default:
				v = QBrush (*RES_color(RID_GM_DLT_NTR));
				break;
		}
	} else {
		v = QBrush (*RES_color(RID_GMRC_FG_DEFAULT));
	}
	return (v);
}

QVariant
ModelRoster::MDLR_data_background (int /*row*/, int /*col*/, MDLR_DATA *data, SPWDLT * /*dlt*/) const
{
	QVariant		v = QVariant();
	SPWAW_DOSSIER_UIR	*uir;

	if (!data) return (v);
	uir = data->uir;

	switch (d.hilite) {
		case MDLR_HILITE_NONE:
		default:
			v = QBrush (*RES_color(RID_GMRC_BG_DEFAULT));
			break;
		case MDLR_HILITE_RANK:
			v = QBrush (*RES_GUI_color (uir->snap->data.brank));
			break;
		case MDLR_HILITE_USTATUS:
			v = QBrush (*RES_GUI_color (uir->snap->data.status));
			break;
		case MDLR_HILITE_ENTR:
			v = QBrush (*RES_GUI_color (uir->snap->data.entr));
			break;
		case MDLR_HILITE_EXP:
			v = QBrush (*RES_GUI_color (uir->snap->data.eclass));
			break;
	}
	return (v);
}

QVariant
ModelRoster::MDLR_data_decoration (int /*row*/, int col, MDLR_DATA *data, SPWDLT * /*dlt*/) const
{
	QVariant		v = QVariant();
	SPWAW_DOSSIER_UIR	*uir;

	if (!data) return (v);
	uir = data->uir;

	switch (col) {
		case MDLR_COLUMN_UID:
			if (d.mflag && !d.tflag) {
				if (d.cb && SPWAW_UHT_is_commissioned (data->uhte, &(d.cb->bdate)))
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_COMMISSIONED)));
				else if (d.cb && SPWAW_UHT_is_reassigned (data->uhte, &(d.cb->bdate)))
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_REASSIGNED)));
				else if (!d.cb && SPWAW_UHT_has_reassignment (data->uhte))
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_REASSIGNED)));
				else
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_EMPTY)));
			}
			break;
		case MDLR_COLUMN_UNIT:
			if (d.mflag && !d.tflag) {
				if (d.cb && SPWAW_UHT_is_upgraded (data->uhte, &(d.cb->bdate)))
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_UPGRADED)));
				else if (!d.cb && SPWAW_UHT_has_upgrade (data->uhte))
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_UPGRADED)));
				else
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_EMPTY)));
			}
			break;
		case MDLR_COLUMN_RNK:
			if (d.mflag && !d.tflag) {
				if (d.cb && SPWAW_UHT_is_promoted (data->uhte,&(d.cb->bdate)))
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_PROMOTED)));
				else if (!d.cb && SPWAW_UHT_has_promotion (data->uhte))
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_PROMOTED)));
				else if (d.cb && SPWAW_UHT_is_demoted (data->uhte,&(d.cb->bdate)))
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_DEMOTED)));
				else if (!d.cb && SPWAW_UHT_has_demotion (data->uhte))
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_DEMOTED)));
				else
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_EMPTY)));
			}
			break;
		case MDLR_COLUMN_LDR:
			if (d.mflag && !d.tflag) {
				if (d.cb && SPWAW_UHT_is_replaced (data->uhte,&(d.cb->bdate)))
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_REPLACED)));
				else if (!d.cb && SPWAW_UHT_has_replacement (data->uhte))
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_REPLACED)));
				else
					v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_EMPTY)));
			}
			break;
		case MDLR_COLUMN_STATUS:
			if (uir->snap->data.rank == SPWAW_RKIA)
				v = QVariant (QIcon (*RES_pixmap (RID_ICON_UHT_DESTROYED)));
			else
				v = QVariant (QIcon (*RES_pixmap(RID_ICON_UHT_EMPTY)));
			break;
		default:
			break;
	}
	return (v);
}

QVariant
ModelRoster::MDLR_data (int role, int row, int col) const
{
	QVariant	v = QVariant();

	if ((row < 0) || (row >= d.row_cnt)) return (v);

	switch (role) {
		case Qt::DisplayRole:
			v = MDLR_data_display (row, col, d.smap[row].data, &(d.smap[row].data->dlt[col]));
			break;
		case Qt::FontRole:
			v= MDLR_data_font (row, col, d.smap[row].data, &(d.smap[row].data->dlt[col]));
			break;
		case Qt::ForegroundRole:
			v = MDLR_data_foreground (row, col, d.smap[row].data, &(d.smap[row].data->dlt[col]));
			break;
		case Qt::BackgroundRole:
			v = MDLR_data_background (row, col, d.smap[row].data, &(d.smap[row].data->dlt[col]));
			break;
		case Qt::DecorationRole:
			v = MDLR_data_decoration (row, col, d.smap[row].data, &(d.smap[row].data->dlt[col]));
			break;
		case Qt::TextAlignmentRole:
			//v = QVariant (Qt::AlignVCenter | Qt::AlignRight);
			v = QVariant (Qt::AlignVCenter);
			break;
		default:
			break;
	}
	return (v);
}

void
MDLR_data_sort (int col, MDLR_DATA *p, MDL_CMPDATA &v)
{
	/* safe default value, effectively sets union memory to 0 */
	v.d = 0.0;

	if (!p) return;

	switch (col) {
		case MDLR_COLUMN_UID:
			v.u = MDL_PACK_UID(p->uir);
			break;
		case MDLR_COLUMN_UNIT: // FIXME: designation, actually
			v.s = p->uir->snap->data.dname;
			break;
		case MDLR_COLUMN_RNK:
			//v.i = p->uir->snap->data.rank;
			v.i = p->uir->snap->data.brank;
			break;
		case MDLR_COLUMN_LDR:
			v.s = p->uir->snap->data.lname;
			break;
		case MDLR_COLUMN_TYPE:
			v.i = p->uir->snap->data.utype;
			break;
		case MDLR_COLUMN_CLASS:
			v.i = p->uir->snap->data.uclass;
			break;
		case MDLR_COLUMN_KILL:
			v.i = p->uir->snap->attr.gen.kills;
			break;
		case MDLR_COLUMN_SEEN:
			v.i = p->uir->snap->data.spotted;
			break;
		case MDLR_COLUMN_STATUS:
			v.i = p->uir->snap->data.status;
			break;
		case MDLR_COLUMN_EXP:
			v.i = p->uir->snap->data.exp;
			break;
		case MDLR_COLUMN_MOR:
			v.i = p->uir->snap->data.mor;
			break;
		case MDLR_COLUMN_SUP:
			v.i = p->uir->snap->data.sup;
			break;
		case MDLR_COLUMN_RAL:
			v.i = p->uir->snap->data.ral;
			break;
		case MDLR_COLUMN_INF:
			v.i = p->uir->snap->data.inf;
			break;
		case MDLR_COLUMN_ARM:
			v.i = p->uir->snap->data.arm;
			break;
		case MDLR_COLUMN_ART:
			v.i = p->uir->snap->data.art;
			break;
		case MDLR_COLUMN_MEN:
			v.i = p->uir->snap->data.hcnt;
			break;
		case MDLR_COLUMN_KIA:
			v.i = p->uir->snap->data.hcnt - p->uir->snap->data.hcnt_left;
			break;
		case MDLR_COLUMN_DMG:
			v.i = p->uir->snap->data.damage;
			break;
		case MDLR_COLUMN_RDY:
			v.d = p->uir->snap->attr.gen.ready;
			break;
		case MDLR_COLUMN_ABAND:
			v.i = p->uir->snap->data.aband;
			break;
		case MDLR_COLUMN_LOADED:
			v.i = p->uir->snap->data.loaded;
			break;
		case MDLR_COLUMN_COST:
			v.i = p->uir->snap->data.cost;
			break;
		case MDLR_COLUMN_SPEED:
			v.i = p->uir->snap->data.speed;
			break;
		default:
			break;
	}
}
