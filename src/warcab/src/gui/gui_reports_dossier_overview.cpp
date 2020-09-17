/** \file
 * The SPWaW war cabinet - GUI - dossier report - overview.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_reports_dossier.h"
#include <spwawlib_uht_job.h>

// TODO: consider externalizing
#define	LISTMAX	5

GuiRptDsrOvr::GuiRptDsrOvr (QWidget *P)
	: QScrollArea (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	GUINEW (d.font, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_REPORTS_INIT_FAILED, "font");

	GUINEW (d.frame, QFrame (this), ERR_GUI_REPORTS_INIT_FAILED, "frame");
	GUINEW (d.layout, QGridLayout (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "layout");

	d.layout->setVerticalSpacing (20);

	GUINEW (d.flag, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "flag");
	d.flag->setAlignment (Qt::AlignLeft);
	d.flag->setSizePolicy (QSizePolicy::Minimum, QSizePolicy::Fixed);

	GUINEW (d.overview, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "overview");
	d.overview->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.overview->setWordWrap (true);
	d.overview->setFont (*d.font);

	GUINEW (d.losses.layout, QGridLayout (), ERR_GUI_REPORTS_INIT_FAILED, "losses layout");

	GUINEW (d.losses.plr, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "player losses");
	d.losses.plr->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.losses.plr->setWordWrap (true);
	d.losses.plr->setFont (*d.font);

	GUINEW (d.losses.opp, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "opponent losses");
	d.losses.opp->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.losses.opp->setWordWrap (true);
	d.losses.opp->setFont (*d.font);

	GUINEW (d.losses.spacer, QSpacerItem (1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum), ERR_GUI_REPORTS_INIT_FAILED, "losses spacer");

	GUINEW (d.changes, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "changes");
	d.changes->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.changes->setWordWrap (true);
	d.changes->setFont (*d.font);

	GUINEW (d.spacer, QSpacerItem (0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding), ERR_GUI_REPORTS_INIT_FAILED, "spacer");

	d.losses.layout->addWidget (d.losses.plr,	0, 0, 1, 1);
	d.losses.layout->addWidget (d.losses.opp,	0, 1, 1, 1);
	d.losses.layout->addItem   (d.losses.spacer,	0, 2, 1, 1);

	d.layout->addWidget (d.flag,		0, 0, 1, 1);
	d.layout->addWidget (d.overview,	1, 0, 1, 1);
	d.layout->addLayout (d.losses.layout,	2, 0, 1, 1);
	d.layout->addWidget (d.changes,		3, 0, 1, 1);
	d.layout->addItem   (d.spacer,		4, 0, 1, 1);

	setWidget(d.frame);
	setWidgetResizable (true);

	if (!connect (this, SIGNAL (cmpcurr(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpcurr(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <cmpcurr> to <dossier:set_cmpcurr>");

	if (!connect (this, SIGNAL (cmpbase(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpbase(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <cmpbase> to <dossier:set_cmpbase>");

	SET_GUICLS_NOERR;
}

GuiRptDsrOvr::~GuiRptDsrOvr (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
}

void
GuiRptDsrOvr::resizeEvent (QResizeEvent * /*event*/)
{
	refresh(false);
}

void
GuiRptDsrOvr::set_parent (GuiRptDsr *p)
{
	d.parent = p;
}

typedef char *(* extra_cb)(SPWAW_DOSSIER_UIR *uir);

static void
report_decommissioned (SPWAW_UHT_LIST_CBCTX &context, UtilStrbuf *sb)
{
	if (context.last) {
		if (context.decommisioned) {
			SPWAW_UHTE *dcu = SPWAW_UHT_last(context.uhte, UHT_DECOMMISSIONED);
			SPWAW_BDATE (dcu->LBD, bd, false);
			sb->printf (" <i><small>(decommisioned after %s)</small></i>", bd);
		}
	}
}

static void
record_battle_date_info_inorder (SPWAW_UHT_LIST_CBCTX &context)
{
	UtilStrbuf	*sb;

	if (!context.extra) return;

	if (!*context.data) *context.data = new UtilStrbuf (true);
	sb = (UtilStrbuf *)(*context.data);

	SPWAW_BDATE (*context.bdate, bd, false);

	if (context.first) {
		sb->printf ("%s %s %s: ", context.uir->snap->strings.uid, context.uir->snap->data.dname, context.uir->snap->data.lname);
		sb->printf ("%s", ((extra_cb)(context.extra))(context.uir));
	} else {
		sb->printf (" -&gt; <small>(%s)</small> %s", bd, ((extra_cb)context.extra)(context.uir));
	}
	report_decommissioned (context, sb);
}

static void
record_battle_date_info_reverse (SPWAW_UHT_LIST_CBCTX &context)
{
	UtilStrbuf	*sb;

	if (!context.extra) return;

	if (!*context.data) *context.data = new UtilStrbuf (true);
	sb = (UtilStrbuf *)(*context.data);

	SPWAW_BDATE (*context.bdate, bd, false);

	if (context.first) {
		sb->printf ("%s %s %s: ", context.uir->snap->strings.uid, context.uir->snap->data.dname, context.uir->snap->data.lname);
	}
	if (!context.last) {
		sb->printf ("%s <small>(%s)</small> &lt;- ", ((extra_cb)context.extra)(context.uir), bd);

	} else {
		sb->printf ("%s", ((extra_cb)context.extra)(context.uir));
	}
	report_decommissioned (context, sb);
}

static char *
uir_data_lname (SPWAW_DOSSIER_UIR *uir)
{
	return (uir->snap->data.lname);
}

void
GuiRptDsrOvr::list_replacements (SPWAW_DOSSIER *d, bool reverse, UtilStrbuf &strbuf)
{
	UHT_LIST_JOB job = { };

	job.what		= "Replaced units";
	job.type		= UHT_LIST_DOSSIER;
	job.in.d.dossier	= d;
	job.how.status		= UHT_REPLACED;
	job.how.reversed	= reverse;
	job.how.allow_decomm	= CFG_full_history();
	job.how.terminal	= true;
	job.dext.data		= reverse?record_battle_date_info_reverse:record_battle_date_info_inorder;
	job.dext.extra		= uir_data_lname;
	job.out.skip_if_empty	= true;
	job.out.hdrpre		= "<pre><h3>";
	job.out.hdrpst		= "</h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.strbuf		= &strbuf;

	UHT_list_job (job);
}

static char *
uir_strings_uid (SPWAW_DOSSIER_UIR *uir)
{
	return (uir->snap->strings.uid);
}

void
GuiRptDsrOvr::list_reassignments (SPWAW_DOSSIER *d, bool reverse, UtilStrbuf &strbuf)
{
	UHT_LIST_JOB job = { };

	job.what		= "Reassigned units";
	job.type		= UHT_LIST_DOSSIER;
	job.in.d.dossier	= d;
	job.how.status		= UHT_REASSIGNED;
	job.how.reversed	= reverse;
	job.how.allow_decomm	= CFG_full_history();
	job.how.terminal	= true;
	job.dext.data		= reverse?record_battle_date_info_reverse:record_battle_date_info_inorder;
	job.dext.extra		= uir_strings_uid;
	job.out.skip_if_empty	= true;
	job.out.hdrpre		= "<pre><h3>";
	job.out.hdrpst		= "</h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.strbuf		= &strbuf;

	UHT_list_job (job);
}

static char *
uir_data_uname (SPWAW_DOSSIER_UIR *uir)
{
	return (uir->snap->data.uname);
}

void
GuiRptDsrOvr::list_upgrades (SPWAW_DOSSIER *d, bool reverse, UtilStrbuf &strbuf)
{
	UHT_LIST_JOB job = { };

	job.what		= "Upgraded units";
	job.type		= UHT_LIST_DOSSIER;
	job.in.d.dossier	= d;
	job.how.status		= UHT_UPGRADED;
	job.how.reversed	= reverse;
	job.how.allow_decomm	= CFG_full_history();
	job.how.terminal	= true;
	job.dext.data		= reverse?record_battle_date_info_reverse:record_battle_date_info_inorder;
	job.dext.extra		= uir_data_uname;
	job.out.skip_if_empty	= true;
	job.out.hdrpre		= "<pre><h3>";
	job.out.hdrpst		= "</h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.strbuf		= &strbuf;

	UHT_list_job (job);
}

static char *
uir_strings_rank (SPWAW_DOSSIER_UIR *uir)
{
	return (uir->snap->strings.rank);
}

void
GuiRptDsrOvr::list_promotions (SPWAW_DOSSIER *d, bool reverse, UtilStrbuf &strbuf)
{
	UHT_LIST_JOB job = { };

	job.what		= "Promoted units";
	job.type		= UHT_LIST_DOSSIER;
	job.in.d.dossier	= d;
	job.how.status		= UHT_PROMOTED;
	job.how.reversed	= reverse;
	job.how.allow_decomm	= CFG_full_history();
	job.how.terminal	= true;
	job.dext.data		= reverse?record_battle_date_info_reverse:record_battle_date_info_inorder;
	job.dext.extra		= uir_strings_rank;
	job.out.skip_if_empty	= true;
	job.out.hdrpre		= "<pre><h3>";
	job.out.hdrpst		= "</h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.strbuf		= &strbuf;

	UHT_list_job (job);
}

void
GuiRptDsrOvr::list_demotions (SPWAW_DOSSIER *d, bool reverse, UtilStrbuf &strbuf)
{
	UHT_LIST_JOB job = { };

	job.what		= "Demoted units";
	job.type		= UHT_LIST_DOSSIER;
	job.in.d.dossier	= d;
	job.how.status		= UHT_DEMOTED;
	job.how.reversed	= reverse;
	job.how.allow_decomm	= CFG_full_history();
	job.how.terminal	= true;
	job.dext.data		= reverse?record_battle_date_info_reverse:record_battle_date_info_inorder;
	job.dext.extra		= uir_strings_rank;
	job.out.skip_if_empty	= true;
	job.out.hdrpre		= "<pre><h3>";
	job.out.hdrpst		= "</h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.strbuf		= &strbuf;

	UHT_list_job (job);
}

static void
record_battle_date_info_for_commissions (SPWAW_UHT_LIST_CBCTX &context)
{
	UtilStrbuf	*sb;

	if (!*context.data) *context.data = new UtilStrbuf (true);
	sb = (UtilStrbuf *)(*context.data);

	SPWAW_BDATE (*context.bdate, bd, false);

	if (context.first) {
		sb->printf ("%s %s %s <small>(%s)</small>",
			context.uir->snap->strings.uid, context.uir->snap->data.dname, context.uir->snap->data.lname, bd);
	}
	report_decommissioned (context, sb);
}

void
GuiRptDsrOvr::list_commissions (SPWAW_DOSSIER *d, UtilStrbuf &strbuf)
{
	if (!CFG_full_history()) return;

	UHT_LIST_JOB job = { };

	job.what		= "Additional commissioned units";
	job.type		= UHT_LIST_DOSSIER;
	job.in.d.dossier	= d;
	job.how.status		= UHT_COMMISSIONED;
	job.how.allow_decomm	= true;
	job.how.terminal	= true;
	job.dext.data		= record_battle_date_info_for_commissions;
	job.out.skip_if_empty	= true;
	job.out.hdrpre		= "<pre><h3>";
	job.out.hdrpst		= "</h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.strbuf		= &strbuf;

	UHT_list_job (job);
}

static void
record_battle_date_info_for_decommissions (SPWAW_UHT_LIST_CBCTX &context)
{
	UtilStrbuf	*sb;

	if (!*context.data) *context.data = new UtilStrbuf (true);
	sb = (UtilStrbuf *)(*context.data);

	SPWAW_BDATE (context.uhte->LBD, bd, false);

	if (context.last) {
		sb->printf ("%s %s %s <small>(after %s)</small>",
			context.uir->snap->strings.uid, context.uir->snap->data.dname, context.uir->snap->data.lname, bd);
	}
}

void
GuiRptDsrOvr::list_decommissions (SPWAW_DOSSIER *d, UtilStrbuf &strbuf)
{
	if (!CFG_full_history()) return;

	UHT_LIST_JOB job = { };

	job.what		= "Decommissioned units";
	job.type		= UHT_LIST_DOSSIER;
	job.in.d.dossier	= d;
	job.how.status		= UHT_DECOMMISSIONED;
	job.how.allow_decomm	= true;
	job.how.terminal	= true;
	job.dext.data		= record_battle_date_info_for_decommissions;
	job.out.skip_if_empty	= true;
	job.out.hdrpre		= "<pre><h3>";
	job.out.hdrpst		= "</h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.strbuf		= &strbuf;

	UHT_list_job (job);
}

void
GuiRptDsrOvr::refresh (bool forced)
{
	MDLD_TREE_ITEM	*item;
	SPWAW_DOSSIER	*p = NULL;
	char		buf[65536];
	UtilStrbuf	str(buf, sizeof (buf), true, true);
	SPWAW_PERIOD	span;

	DBG_TRACE_FENTER;

	item = (d.parent != NULL) ? d.parent->current() : NULL;
	if (!d.reftrack.changed (item) && !forced) goto skip_data_update;

	DBG_TRACE_UPDATE;

	p = (item != NULL) ? item->data.d : NULL;

	if (!p) {
		d.flag->setPixmap (*RES_flag (SPWAW_GAME_TYPE_SPWAW, SPWOOB_UNKNOWN));
		d.overview->setText ("No overview available yet.");
		d.losses.plr->clear();
		d.losses.opp->clear();
		d.changes->clear();
	} else {
		if (p->type != SPWAW_STDALONE_DOSSIER) {
			d.flag->setPixmap (*RES_flag (p->gametype, p->props.OOB));
			d.flag->setHidden(false);
		} else {
			d.flag->setHidden(true);
		}

		str.printf ("<pre>");

		str.printf ("<h1>%s</h1>", p->name);
		str.printf ("<h2>%s", p->comment);
		if (p->comment[strlen (p->comment) - 1] == '\n') str.del (1);
		str.printf ("</h2>");

		switch (p->bcnt) {
			case 0:
				str.printf ("No battles recorded yet.\n");
				break;
			case 1:
				str.printf ("One battle recorded.\n");
				break;
			default:
				SPWAW_date_delta (&(p->bfirst->bdate.date), &(p->blast->bdate.date), &span);
				str.printf ("%u battles recorded", p->bcnt);
				if ((p->type != SPWAW_STDALONE_DOSSIER) && span.stamp) {
					str.printf (", spanning ");
					UTIL_fmt_longspan (&span, &str);
				}
				str.printf (".\n");
				break;
		}

		if (p->stats.concluded > 0) {
			switch (p->stats.concluded) {
				case 1:
					str.printf ("Of these, records show 1 concluded battle.");
					break;
				default:
					str.printf ("Of these, records show %u concluded battles.", p->stats.concluded);
					break;
			}

			str.printf ("<h3>Battle results:</h3>\n\n");
			for (int i=0; i<ARRAYCOUNT(p->stats.results)-1; i++) {
				str.printf ("  <b>%s</b>:\t%u\n", SPWAW_bresult2str((SPWAW_BRESULT)i), p->stats.results[i]);
			}
			//str.printf ("\n");
		}

		str.printf ("<h3>Core force:</h3>");
		if (p->type != SPWAW_STDALONE_DOSSIER) {
			if ((p->props.iucnt != p->props.cucnt) || (p->props.ifcnt != p->props.cfcnt)) {
				str.printf ("Initial %s core force consists of %u units in %u formations.\n",
					SPWAW_oob_people (p->gametype, p->props.OOB), p->props.iucnt, p->props.ifcnt);
				str.printf ("Current core force consists of %u units in %u formations.",
					p->props.cucnt, p->props.cfcnt);
			} else {
				str.printf ("%s core force consists of %u units in %u formations.",
					SPWAW_oob_people (p->gametype, p->props.OOB), p->props.iucnt, p->props.ifcnt);

			}
		} else {
			str.printf ("Campaign tracking is not available for this dossier.");
		}
		str.printf ("</pre>");

		d.overview->setText (buf);
		str.clear();

		if (p->type != SPWAW_STDALONE_DOSSIER) {
			if (p->bcnt) {
				str.printf ("<pre>");
				str.printf ("<h3>%s campaign losses:</h3>", SPWAW_oob_people (p->gametype, p->props.OOB));
				str.printf ("  %6u %s\n", p->blast->tlast->snap->game.campaign.data.P1TL.men, "men");
				str.printf ("  %6u %s\n", p->blast->tlast->snap->game.campaign.data.P1TL.art, "artillery");
				str.printf ("  %6u %s\n", p->blast->tlast->snap->game.campaign.data.P1TL.soft, "soft vehicles");
				str.printf ("  %6u %s\n", p->blast->tlast->snap->game.campaign.data.P1TL.apc, "armoured personnel carriers");
				str.printf ("  %6u %s\n", p->blast->tlast->snap->game.campaign.data.P1TL.afv, "armoured fighting vehicles");
				str.printf ("  %6u %s\n", p->blast->tlast->snap->game.campaign.data.P1TL.gliders, "gliders");
				str.printf ("  %6u %s\n", p->blast->tlast->snap->game.campaign.data.P1TL.air, "aircraft");
				str.printf ("</pre>");
				d.losses.plr->setText (buf);
				str.clear();

				str.printf ("<pre>");
				str.printf ("<h3>Opponent campaign losses:</h3>");
				str.printf ("  %6u %s\n", p->blast->tlast->snap->game.campaign.data.P2TL.men, "men");
				str.printf ("  %6u %s\n", p->blast->tlast->snap->game.campaign.data.P2TL.art, "artillery");
				str.printf ("  %6u %s\n", p->blast->tlast->snap->game.campaign.data.P2TL.soft, "soft vehicles");
				str.printf ("  %6u %s\n", p->blast->tlast->snap->game.campaign.data.P2TL.apc, "armoured personnel carriers");
				str.printf ("  %6u %s\n", p->blast->tlast->snap->game.campaign.data.P2TL.afv, "armoured fighting vehicles");
				str.printf ("  %6u %s\n", p->blast->tlast->snap->game.campaign.data.P2TL.gliders, "gliders");
				str.printf ("  %6u %s\n", p->blast->tlast->snap->game.campaign.data.P2TL.air, "aircraft");
				str.printf ("</pre>");
				d.losses.opp->setText (buf);
				str.clear();
			}

			if (p->bcnt) {
				bool reverse = true;
				list_replacements (p, reverse, str);
				list_reassignments (p, reverse, str);
				list_upgrades (p, reverse, str);
				list_promotions (p, reverse, str);
				list_demotions (p, reverse, str);
				list_commissions (p, str);
				list_decommissions (p, str);
			}

			d.changes->setText (buf);
		}
	}

skip_data_update:
	if (isVisible()) {
		emit cmpcurr (NULL);
		emit cmpbase (NULL);
	}

	/* Make sure layout is updated if necessary */
	d.flag->updateGeometry();

	DBG_TRACE_FLEAVE;
}
