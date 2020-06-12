/** \file
 * The SPWaW war cabinet - GUI - force losses report.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_report_losses.h"
#include "model/model_roster.h"
#include "gui_reports_dossier.h"
#include "gui_reports_battle.h"
#include "gui_reports_bturn.h"
#include <spwawlib_uht_job.h>

// TODO: consider externalizing
#define	LISTMAX	5

#define	RECORD_DETAILS	0

GuiRptLoss::GuiRptLoss (QWidget *P)
	: QScrollArea (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	GUINEW (d.model, ModelRoster (), ERR_GUI_REPORTS_INIT_FAILED, "data model");

	GUINEW (d.font, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_REPORTS_INIT_FAILED, "font");

	GUINEW (d.frame, QFrame (this), ERR_GUI_REPORTS_INIT_FAILED, "frame");
	GUINEW (d.layout, QGridLayout (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "layout");

	d.layout->setVerticalSpacing (20);

	GUINEW (d.label_nodata, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_nodata");
	d.label_nodata->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label_nodata->setWordWrap (true);
	d.label_nodata->setFont (*d.font);
	d.label_nodata->setText ("No information available yet.");

	GUINEW (d.label_intel, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_intel");
	d.label_intel->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label_intel->setWordWrap (true);

	GUINEW (d.label_khdr, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_khdr");
	d.label_khdr->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	//d.label_khdr->setWordWrap (true);
	d.label_khdr->setWordWrap (false);
	d.label_khdr->setFont (*d.font);

	GUINEW (d.label_klist, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_klist");
	d.label_klist->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label_klist->setWordWrap (true);
	d.label_klist->setFont (*d.font);

	GUINEW (d.label_ahdr, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_ahdr");
	d.label_ahdr->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	//d.label_ahdr->setWordWrap (true);
	d.label_ahdr->setWordWrap (false);
	d.label_ahdr->setFont (*d.font);

	GUINEW (d.label_alist, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_alist");
	d.label_alist->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label_alist->setWordWrap (true);
	d.label_alist->setFont (*d.font);

	GUINEW (d.label_dhdr, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_dhr");
	d.label_dhdr->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	//d.label_dhdr->setWordWrap (true);
	d.label_dhdr->setWordWrap (false);
	d.label_dhdr->setFont (*d.font);

	GUINEW (d.label_dlist, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label_dlist");
	d.label_dlist->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label_dlist->setWordWrap (true);
	d.label_dlist->setFont (*d.font);

	GUINEW (d.spacer, QSpacerItem (0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding), ERR_GUI_REPORTS_INIT_FAILED, "spacer");

	d.layout->addWidget (d.label_nodata,	0, 0);
	d.layout->addWidget (d.label_intel,	1, 0);
	d.layout->addWidget (d.label_khdr,	2, 0);
	d.layout->addWidget (d.label_klist,	3, 0);
	d.layout->addWidget (d.label_ahdr,	4, 0);
	d.layout->addWidget (d.label_alist,	5, 0);
	d.layout->addWidget (d.label_dhdr,	6, 0);
	d.layout->addWidget (d.label_dlist,	7, 0);
	d.layout->addItem   (d.spacer,		8, 0);

	setWidget(d.frame);
	setWidgetResizable (true);

	if (!connect (this, SIGNAL (cmpcurr(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpcurr(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <cmpcurr> to <dossier:set_cmpcurr>");

	if (!connect (this, SIGNAL (cmpbase(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpbase(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_OOB_INIT_FAILED, "failed to connect <cmpbase> to <dossier:set_cmpbase>");

	if (!connect (GUI_WIN, SIGNAL (selected_intel_mode(INTEL_MODE)), this, SLOT (intel_mode_set(INTEL_MODE))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <mainwindow:selected_intel_mode> to <intel_mode_set>");

	d.pflag = d.cflag = true;

	SET_GUICLS_NOERR;
}

GuiRptLoss::~GuiRptLoss (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
	delete d.model;
}

void
GuiRptLoss::set_parent (GuiRptDsr *parent, bool player)
{
	d.ptype	 = MDLD_TREE_DOSSIER;
	d.pptr.d = parent;
	d.pflag	 = player;
	d.cflag	 = true;
}

void
GuiRptLoss::set_parent (GuiRptBtl *parent, bool player, bool core)
{
	d.ptype	 = MDLD_TREE_BATTLE;
	d.pptr.b = parent;
	d.pflag	 = player;
	d.cflag	 = core;
}

void
GuiRptLoss::set_parent (GuiRptTrn *parent, bool player, bool core)
{
	d.ptype	 = MDLD_TREE_BTURN;
	d.pptr.t = parent;
	d.pflag	 = player;
	d.cflag	 = core;
}

bool
GuiRptLoss::update (bool forced)
{
	bool	skip;

	DBG_TRACE_FENTER;

	d.pcurr = d.pbase = NULL;
	switch (d.ptype) {
		case MDLD_TREE_DOSSIER:
			d.pdata = d.pptr.d ? d.pptr.d->current() : NULL;
			if (d.pdata) {
				if (!d.pdata->data.d || !d.pdata->children.size()) d.pdata = NULL;
			}
			break;
		case MDLD_TREE_BATTLE:
			d.pdata = d.pptr.b ? d.pptr.b->current() : NULL;
			if (d.pdata) {
				if (!d.pdata->data.b || !d.pdata->children.size()) d.pdata = NULL;
			}
			break;
		case MDLD_TREE_BTURN:
			d.pdata = d.pptr.t ? d.pptr.t->current() : NULL;
			if (d.pdata) {
				if (!d.pdata->data.t) d.pdata = NULL;
			}
			break;
		default:
			break;
	}

	skip  = !d.reftrack.changed (d.pdata);
	skip &= !forced;
	if (skip) goto skip_data_update;

	DBG_TRACE_UPDATE;

	if (d.pdata) {
		switch (d.ptype) {
			case MDLD_TREE_DOSSIER:
				d.pcurr = d.pdata->clast;
				d.pbase = d.pdata->cfirst;
				d.model->load (d.pdata->data.d, CFG_full_history(), d.Vintel_mode);
				break;
			case MDLD_TREE_BATTLE:
				d.pcurr = d.pbase = d.pdata;
				//d.pcurr = d.pdata;
				//d.pbase = d.pdata->prev ? d.pdata->prev : d.pdata;
				d.model->load (d.pcurr->data.b, d.pbase->data.b, d.pflag, d.cflag, d.Vintel_mode);
				break;
			case MDLD_TREE_BTURN:
				d.pcurr = d.pdata;
				if (d.pdata->prev) {
					d.pbase = d.pdata->prev;
					d.model->load (d.pcurr->data.t, d.pbase->data.t, d.pflag, d.cflag, d.Vintel_mode);
				} else {
					d.pbase = d.pdata->parent;
					d.model->load (d.pcurr->data.t, d.pcurr->data.t, d.pflag, d.cflag, d.Vintel_mode);
				}
			default:
				break;
		}
	} else {
		d.pbase = d.pcurr = NULL;
		d.model->clear();
	}

skip_data_update:
	/* Only emit these signals when widget is visible */
	if (isVisible()) {
		emit cmpcurr (d.pcurr);
		emit cmpbase (d.pbase);
	}

	DBG_TRACE_FLEAVE;

	return (skip);
}

static void
record_battle_date (SPWAW_UHT_LIST_CBCTX &context)
{
	UtilStrbuf	*sb;

	if (!*context.data) *context.data = new UtilStrbuf (true);
	sb = (UtilStrbuf *)(*context.data);

	SPWAW_BDATE (*context.bdate, bd, false);

	if (!context.first) sb->printf (", ");
	sb->printf ("%s", bd);
}

static void simple_reporter (SPWAW_UHT_LISTEL *listel, SPWAW_DOSSIER_UIR *uir, UtilStrbuf *sb)
{
	if (!listel || !uir || !sb) return;

	bool dc = listel->uhte?SPWAW_UHT_is_decommissioned (listel->uhte):false;

	if (dc) {
		sb->printf ("<font color=%s>", qPrintable(RES_htmlcolor (RID_GM_DLT_INA)));
		sb->printf ("<i>");
	}

	sb->printf ("%3.3s %s, %s %s (%u)",
		uir->snap->strings.uid, uir->snap->data.dname,
		uir->snap->strings.rank, uir->snap->data.lname,
		listel->count);

#if	RECORD_DETAILS
	if (listel->data) {
		sb->printf (" <i><small>{%s}</small></i>", ((UtilStrbuf *)(listel->data))->data());
		delete (UtilStrbuf *)(listel->data);
	}
#endif	/* RECORD_DETAILS */

	if (dc) {
		sb->printf (" <small>decommissioned</small></i>");
		sb->printf ("</font>");
	}

	sb->add ("\n");
}

void
GuiRptLoss::list_killed_dossier (bool sort)
{
	UHT_LIST_JOB job = { };

	job.what		= "Destroyed units";
	job.type		= UHT_LIST_DOSSIER;
	job.in.d.dossier	= d.pdata->data.d;
	job.how.status		= UHT_DESTROYED;
	job.how.allow_decomm	= CFG_full_history();
	job.dext.data		= record_battle_date;
	job.dext.counted	= true;
	job.sort.sorted		= sort;
	if (job.sort.sorted) {
		job.sort.type		= SPWAW_UHT_SORT_UID;
		job.sort.ascending	= true;
		job.sort.cmp		= NULL;
	}
	job.out.visitor		= simple_reporter;
	job.out.hdrpre		= "<pre><h3><u>";
	job.out.hdrpst		= "</u></h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.hdr		= d.label_khdr;
	job.out.lst		= d.label_klist;

	UHT_list_job (job);
}

void
GuiRptLoss::list_abandoned_dossier (bool sort)
{
	UHT_LIST_JOB job = { };

	job.what		= "Abandoned units, but not destroyed";
	job.type		= UHT_LIST_DOSSIER;
	job.in.d.dossier	= d.pdata->data.d;
	job.how.status		= UHT_ABANDONED;
	job.how.allow_decomm	= CFG_full_history();
	job.dext.data		= record_battle_date;
	job.dext.counted	= true;
	job.sort.sorted		= sort;
	if (job.sort.sorted) {
		job.sort.type		= SPWAW_UHT_SORT_COUNT;
		job.sort.ascending	= false;
		job.sort.cmp		= NULL;
	}
	job.out.visitor		= simple_reporter;
	job.out.hdrpre		= "<pre><h3><u>";
	job.out.hdrpst		= "</u></h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.hdr		= d.label_ahdr;
	job.out.lst		= d.label_alist;

	UHT_list_job (job);
}

class DamageData {
public:
	DamageData () { memset (&d, 0, sizeof (d)); d.sb = new UtilStrbuf (true); }
	~DamageData() { delete d.sb; }

	struct s_data {
		UtilStrbuf	*sb;
		unsigned long	dmg;
	} d;
};

static void
record_damage_data (SPWAW_UHT_LIST_CBCTX &context)
{
	DamageData	*data;

	if (!*context.data) *context.data = new DamageData();
	data = (DamageData *)(*context.data);

	data->d.dmg += context.uhte->v_damage;

	SPWAW_BDATE (*context.bdate, bd, false);

#if	RECORD_DETAILS
	if (!context.first) data->d.sb->printf (", ");
	data->d.sb->printf ("%d in %s", context.uhte->v_damage, bd);
#endif	/* RECORD_DETAILS */
}

static int
cmp_damage (const void *a, const void *b)
{
	SPWAW_UHT_LISTEL	*ea = *((SPWAW_UHT_LISTEL **)a);
	SPWAW_UHT_LISTEL	*eb = *((SPWAW_UHT_LISTEL **)b);

	DamageData	*da = (DamageData *)(ea->data);
	DamageData	*db = (DamageData *)(eb->data);

	if (da->d.dmg == db->d.dmg) return (SPWAW_UHT_list_cmp_UID_ascending(a,b));
	return ((da->d.dmg < db->d.dmg)?+1:-1);
}

static void damage_data_reporter (SPWAW_UHT_LISTEL *listel, SPWAW_DOSSIER_UIR *uir, UtilStrbuf *sb)
{
	if (!listel || !uir || !sb) return;

	DamageData *data = (DamageData *)(listel->data); listel->data = NULL;
	if (data) {
		bool dc = listel->uhte?SPWAW_UHT_is_decommissioned (listel->uhte):false;

		if (dc) {
			sb->printf ("<font color=%s>", qPrintable(RES_htmlcolor (RID_GM_DLT_INA)));
			sb->printf ("<i>");
		}

		sb->printf ("%3.3s %s, %s %s (%u)",
			uir->snap->strings.uid, uir->snap->data.dname,
			uir->snap->strings.rank, uir->snap->data.lname,
			data->d.dmg);

#if	RECORD_DETAILS
		sb->printf (" <i>{<small>%s</small>}</i>", data->d.sb->data());
#endif	/* RECORD_DETAILS */

		if (dc) {
			sb->printf (" <small>decommissioned</small></i>");
			sb->printf ("</font>");
		}

		delete data;
	}
	sb->add ("\n");
}

void
GuiRptLoss::list_damaged_dossier (bool sort)
{
	UHT_LIST_JOB job = { };

	job.what		= "Damaged units, but not abandoned or destroyed";
	job.type		= UHT_LIST_DOSSIER;
	job.in.d.dossier	= d.pdata->data.d;
	job.how.status		= UHT_DAMAGED;
	job.how.allow_decomm	= CFG_full_history();
	job.dext.data		= record_damage_data;
	job.dext.counted	= false;
	job.sort.sorted		= sort;
	if (job.sort.sorted) {
		job.sort.type		= SPWAW_UHT_SORT_USER;
		job.sort.ascending	= true;
		job.sort.cmp		= cmp_damage;
	}
	job.out.visitor		= damage_data_reporter;
	job.out.hdrpre		= "<pre><h3><u>";
	job.out.hdrpst		= "</u></h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.hdr		= d.label_dhdr;
	job.out.lst		= d.label_dlist;

	UHT_list_job (job);
}

void
GuiRptLoss::list_killed (void)
{
	char			buf[32768];
	UtilStrbuf		str(buf, sizeof(buf), true, true);
	int			cnt, idx;
	bool			stop;
	ModelRosterRawData	data[LISTMAX];
	char			tbuf[4096];
	UtilStrbuf		tstr(tbuf, sizeof (tbuf), true, true);
	bool			lostunit, lostcrew;

	str.clear();

	d.model->set_dltsort (false);
	d.model->sort (MDLR_COLUMN_STATUS, Qt::DescendingOrder);

	cnt = idx = 0; stop = false;
	while (1) {
		int dcnt = d.model->rawdata (idx, MDLR_COLUMN_STATUS, data, LISTMAX);
		if (!dcnt) break;

		tstr.clear();
		for (int i=0; i<dcnt; i++) {
			if (SPWDLT_int (data[i].dlt) == SPWAW_UREADY) { stop = true; break; }
			if (!data[i].uir->snap->attr.gen.losses) continue;

			bool dc = data[i].uhte?SPWAW_UHT_is_decommissioned (data[i].uhte):false;

			lostunit = !data[i].uir->snap->data.alive;
			lostcrew = data[i].uir->snap->data.crew
				   && (!data[i].uir->snap->data.aunit.up || !data[i].uir->snap->data.aunit.up->data.alive);

			if (dc) {
				tstr.printf ("<font color=%s>", qPrintable(RES_htmlcolor (RID_GM_DLT_INA)));
				tstr.printf ("<i>");
			}
			if (d.pflag || (d.Vintel_mode != INTEL_MODE_NONE)) {
				tstr.printf ("%3.3s", data[i].uir->snap->strings.uid);
			}
			tstr.printf (" %s, %s %s (%s%s%s)",
				data[i].uir->snap->data.dname,
				data[i].uir->snap->strings.rank, data[i].uir->snap->data.lname,
				lostunit?"unit":"",
				(lostunit && lostcrew) ? " and " : "",
				lostcrew?"crew":"");
			if (dc) {
				tstr.printf (" <small>decommissioned</small></i>");
				tstr.printf ("</font>");
			}
			tstr.printf ("<br>");


			cnt += data[i].uir->snap->attr.gen.losses;
		}
		str.add (tbuf);

		if (stop) break;
		idx += dcnt;
	}

	if (cnt) {
		d.label_klist->setText (buf);

		tstr.clear();
		tstr.printf ("<h3><u>Destroyed units: %d</u></h3>", cnt);
		d.label_khdr->setText (tbuf);
	} else {
		d.label_klist->setText ("-");

		tstr.clear();
		tstr.printf ("<h3><u>Destroyed units: none</u></h3>");
		d.label_khdr->setText (tbuf);
	}
}

void
GuiRptLoss::list_abandoned (void)
{
	char			buf[32768];
	UtilStrbuf		str(buf, sizeof(buf), true, true);
	int			cnt, idx;
	bool			stop;
	ModelRosterRawData	data[LISTMAX];
	char			tbuf[4096];
	UtilStrbuf		tstr(tbuf, sizeof (tbuf), true, true);

	if (!d.pflag && (d.Vintel_mode == INTEL_MODE_NONE)) {
		str.clear();
		str.printf ("<h3><u>Abandoned units, but not destroyed:</u></h3>");
		d.label_ahdr->setText (buf);

		str.clear();
		str.printf ("Not available.");
		d.label_alist->setText (buf);
		return;
	}

	str.clear();

	d.model->set_dltsort (false);
	d.model->sort (MDLR_COLUMN_ABAND, Qt::AscendingOrder);

	cnt = idx = 0; stop = false;
	while (1) {
		int dcnt = d.model->rawdata (idx, MDLR_COLUMN_ABAND, data, LISTMAX);
		if (!dcnt) break;

		tstr.clear();
		for (int i=0; i<dcnt; i++) {
			if (SPWDLT_int (data[i].dlt) == SPWAW_ANONE) { stop = true; break; }
			if (!data[i].uir->snap->data.alive) continue;

			bool dc = data[i].uhte?SPWAW_UHT_is_decommissioned (data[i].uhte):false;

			if (dc) {
				tstr.printf ("<font color=%s>", qPrintable(RES_htmlcolor (RID_GM_DLT_INA)));
				tstr.printf ("<i>");
			}
			tstr.printf ("%3.3s %s, %s %s",
				data[i].uir->snap->strings.uid, data[i].uir->snap->data.dname,
				data[i].uir->snap->strings.rank, data[i].uir->snap->data.lname);
			if (dc) {
				tstr.printf (" <small>decommissioned</small></i>");
				tstr.printf ("</font>");
			}
			tstr.printf ("<br>");

			cnt++;
		}
		str.add (tbuf);

		if (stop) break;
		idx += dcnt;
	}

	if (cnt) {
		if (!d.pflag && (d.Vintel_mode == INTEL_MODE_LMTD)) {
			str.clear();
			str.printf ("Details not available.");
		}
		d.label_alist->setText (buf);

		tstr.clear();
		tstr.printf ("<h3><u>Abandoned units, but not destroyed: %d</u></h3>", cnt);
		d.label_ahdr->setText (tbuf);
	} else {
		d.label_alist->setText ("-");

		tstr.clear();
		tstr.printf ("<h3><u>Abandoned units, but not destroyed: none</u></h3>");
		d.label_ahdr->setText (tbuf);
	}
}

void
GuiRptLoss::list_damaged (void)
{
	char			buf[32768];
	UtilStrbuf		str(buf, sizeof(buf), true, true);
	int			cnt, idx;
	bool			stop;
	ModelRosterRawData	data[LISTMAX];
	char			tbuf[4096];
	UtilStrbuf		tstr(tbuf, sizeof (tbuf), true, true);

	if (!d.pflag && (d.Vintel_mode == INTEL_MODE_NONE)) {
		str.clear();
		str.printf ("<h3><u>Damaged units, but not abandoned or destroyed:</u></h3>");
		d.label_dhdr->setText (buf);

		str.clear();
		str.printf ("Not available.");
		d.label_dlist->setText (buf);
		return;
	}

	str.clear();

	d.model->set_dltsort (true);
	d.model->sort (MDLR_COLUMN_DMG, Qt::AscendingOrder);

	cnt = idx = 0; stop = false;
	while (1) {
		int dcnt = d.model->rawdata (idx, MDLR_COLUMN_DMG, data, LISTMAX);
		if (!dcnt) break;

		tstr.clear();
		for (int i=0; i<dcnt; i++) {
			if (SPWDLT_getint (data[i].dlt) <= 0) { stop = true; break; }
			if (!data[i].uir->snap->data.alive) continue;
			if (data[i].uir->snap->data.aband != SPWAW_ANONE) continue;

			bool dc = data[i].uhte?SPWAW_UHT_is_decommissioned (data[i].uhte):false;

			if (dc) {
				tstr.printf ("<font color=%s>", qPrintable(RES_htmlcolor (RID_GM_DLT_INA)));
				tstr.printf ("<i>");
			}
			tstr.printf ("%3.3s %s, %s %s (%d)",
				data[i].uir->snap->strings.uid, data[i].uir->snap->data.dname,
				data[i].uir->snap->strings.rank, data[i].uir->snap->data.lname,
				SPWDLT_getint (data[i].dlt));
			if (dc) {
				tstr.printf (" <small>decommissioned</small></i>");
				tstr.printf ("</font>");
			}
			tstr.printf ("<br>");

			cnt++;
		}
		str.add (tbuf);

		if (stop) break;
		idx += dcnt;
	}

	if (cnt) {
		if (!d.pflag && (d.Vintel_mode == INTEL_MODE_LMTD)) {
			str.clear();
			str.printf ("Details not available.");
		}
		d.label_dlist->setText (buf);

		tstr.clear();
		tstr.printf ("<h3><u>Damaged units, but not abandoned or destroyed: %d</u></h3>", cnt);
		d.label_dhdr->setText (tbuf);
	} else {
		d.label_dlist->setText ("-");

		tstr.clear();
		tstr.printf ("<h3><u>Damaged units, but not abandoned or destroyed: none</u></h3>");
		d.label_dhdr->setText (tbuf);
	}
}

void
GuiRptLoss::refresh (bool forced)
{
	bool		skip;
	bool		nodata;
	char		buf[256];
	UtilStrbuf	str(buf, sizeof (buf), true, true);
	int		mw[2];

	DBG_TRACE_FENTER;

	skip = update(forced);
	if (skip) goto leave;

	nodata = !d.pdata;
	d.label_nodata->setHidden (!nodata);
	d.label_intel->setHidden (nodata);
	d.label_klist->setHidden (nodata);
	d.label_alist->setHidden (nodata);
	d.label_dlist->setHidden (nodata);
	if (nodata) goto leave;

	intelmode2label (d.Vintel_mode, d.pflag, d.label_intel);

	if (d.ptype == MDLD_TREE_DOSSIER) {
		list_killed_dossier (false);
		list_abandoned_dossier (true);
		list_damaged_dossier (true);
	} else {
		list_killed ();
		list_abandoned ();
		list_damaged ();
	}

	mw[0] = 0;
	mw[1] = d.label_klist->minimumSizeHint().width(); if (mw[1] > mw[0]) mw[0] = mw[1];
	mw[1] = d.label_alist->minimumSizeHint().width(); if (mw[1] > mw[0]) mw[0] = mw[1];
	mw[1] = d.label_dlist->minimumSizeHint().width(); if (mw[1] > mw[0]) mw[0] = mw[1];
	for (int i=0; i<3; i++) d.layout->setColumnMinimumWidth (i, mw[0]);

leave:
	DBG_TRACE_FLEAVE;
}

void
GuiRptLoss::intel_mode_set (INTEL_MODE mode)
{
	d.Vintel_mode = mode;

	refresh (!d.pflag);
}