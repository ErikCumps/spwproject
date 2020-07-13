/** \file
 * The SPWaW war cabinet - GUI - battle report - overview.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_reports_battle.h"

// TODO: consider externalizing
#define	LISTMAX	5

GuiRptBtlOvr::GuiRptBtlOvr (QWidget *P)
	: QScrollArea (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	GUINEW (d.font, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_REPORTS_INIT_FAILED, "font");

	GUINEW (d.frame, QFrame (this), ERR_GUI_REPORTS_INIT_FAILED, "frame");

	GUINEW (d.layout, QGridLayout (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "layout");
	d.layout->setVerticalSpacing (20);

	GUINEW (d.name, QLabel(d.frame), ERR_GUI_REPORTS_INIT_FAILED, "battle name label");

	d.layout->addWidget (d.name, 0, 0, 1, 4);

	GUINEW (d.intel, QLabel(d.frame), ERR_GUI_REPORTS_INIT_FAILED, "intel mode label");

	d.layout->addWidget (d.intel, 1, 0, 1, 4);

	GUINEW (d.player1, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "player1 label");
	GUINEW (d.mission, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "mission label");
	GUINEW (d.player2, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "player2 label");
	GUINEW (d.rspacer, QSpacerItem (1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum), ERR_GUI_REPORTS_INIT_FAILED, "rspacer");

	d.player1->setAlignment (Qt::AlignRight);
	d.player1->setSizePolicy (QSizePolicy::Minimum, QSizePolicy::Fixed);

	d.mission->setAlignment (Qt::AlignHCenter);
	d.mission->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
	d.mission->setMouseTracking (true);

	d.player2->setAlignment (Qt::AlignLeft);
	d.player2->setSizePolicy (QSizePolicy::Minimum, QSizePolicy::Fixed);

	d.layout->addWidget (d.player1,	2, 0, 1, 1);
	d.layout->addWidget (d.mission,	2, 1, 1, 1);
	d.layout->addWidget (d.player2,	2, 2, 1, 1);
	d.layout->addItem   (d.rspacer,	2, 3, 1, 1);

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

	GUINEW (d.bspacer, QSpacerItem (0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding), ERR_GUI_REPORTS_INIT_FAILED, "bspacer");

	d.losses.layout->addWidget (d.losses.plr,	0, 0, 1, 1);
	d.losses.layout->addWidget (d.losses.opp,	0, 1, 1, 1);
	d.losses.layout->addItem   (d.losses.spacer,	0, 2, 1, 1);

	d.layout->addWidget (d.overview,	3, 0, 1, 4);
	d.layout->addLayout (d.losses.layout,	4, 0, 1, 4);
	d.layout->addWidget (d.changes,		5, 0, 1, 4);
	d.layout->addItem   (d.bspacer,		6, 0, 1, 4);

	setWidget(d.frame);
	setWidgetResizable (true);

	if (!connect (this, SIGNAL (cmpcurr(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpcurr(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <cmpcurr> to <dossier:set_cmpcurr>");

	if (!connect (this, SIGNAL (cmpbase(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpbase(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <cmpbase> to <dossier:set_cmpbase>");

	if (!connect (GUI_WIN, SIGNAL (selected_intel_mode(INTEL_MODE)), this, SLOT (intel_mode_set(INTEL_MODE))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <mainwindow:selected_intel_mode> to <intel_mode_set>");

	SET_GUICLS_NOERR;
}

GuiRptBtlOvr::~GuiRptBtlOvr (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
}

void
GuiRptBtlOvr::resizeEvent (QResizeEvent * /*event*/)
{
	refresh();
}

void
GuiRptBtlOvr::set_parent (GuiRptBtl *p)
{
	d.parent = p;
}

static void
record_replacement (SPWAW_UHT_LIST_CBCTX &context)
{
	UtilStrbuf	*sb;

	if (!*context.data) *context.data = new UtilStrbuf (true);
	sb = (UtilStrbuf *)(*context.data);

	sb->printf ("%s %s: %s %s -> %s %s",
		context.uir->snap->strings.uid, context.uir->snap->data.dname,
		context.puir->snap->strings.rank, context.puir->snap->data.lname,
		context.uir->snap->strings.rank, context.uir->snap->data.lname);
}

void
GuiRptBtlOvr::list_replacements (SPWAW_BATTLE *b, UtilStrbuf &strbuf)
{
	UHT_LIST_JOB job = { };

	job.what		= "Replacements";
	job.type		= UHT_LIST_BATTLE;
	job.in.b.battle		= b;
	job.how.status		= UHT_REPLACED;
	job.how.allow_decomm	= true;
	job.dext.data		= record_replacement;
	job.out.skip_if_empty	= true;
	job.out.hdrpre		= "<pre><h3>";
	job.out.hdrpst		= "</h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.strbuf		= &strbuf;

	UHT_list_job (job);
}

static void
record_reassignment (SPWAW_UHT_LIST_CBCTX &context)
{
	UtilStrbuf	*sb;

	if (!*context.data) *context.data = new UtilStrbuf (true);
	sb = (UtilStrbuf *)(*context.data);

	sb->printf ("%s %s: %s -> %s",
		context.uir->snap->data.dname, context.uir->snap->data.lname,
		context.puir->snap->strings.uid,
		context.uir->snap->strings.uid);
}

void
GuiRptBtlOvr::list_reassignments (SPWAW_BATTLE *b, UtilStrbuf &strbuf)
{
	UHT_LIST_JOB job = { };

	job.what		= "Reassignments";
	job.type		= UHT_LIST_BATTLE;
	job.in.b.battle		= b;
	job.how.status		= UHT_REASSIGNED;
	job.how.allow_decomm	= true;
	job.dext.data		= record_reassignment;
	job.out.skip_if_empty	= true;
	job.out.hdrpre		= "<pre><h3>";
	job.out.hdrpst		= "</h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.strbuf		= &strbuf;

	UHT_list_job (job);
}

static void
record_upgrade (SPWAW_UHT_LIST_CBCTX &context)
{
	UtilStrbuf	*sb;

	if (!*context.data) *context.data = new UtilStrbuf (true);
	sb = (UtilStrbuf *)(*context.data);

	sb->printf ("%s %s %s: %s -> %s",
		context.uir->snap->strings.uid, context.uir->snap->strings.rank, context.uir->snap->data.lname,
		context.puir->snap->data.dname,
		context.uir->snap->data.dname);
}

void
GuiRptBtlOvr::list_upgrades (SPWAW_BATTLE *b, UtilStrbuf &strbuf)
{
	UHT_LIST_JOB job = { };

	job.what		= "Upgrades";
	job.type		= UHT_LIST_BATTLE;
	job.in.b.battle		= b;
	job.how.status		= UHT_UPGRADED;
	job.how.allow_decomm	= true;
	job.dext.data		= record_upgrade;
	job.out.skip_if_empty	= true;
	job.out.hdrpre		= "<pre><h3>";
	job.out.hdrpst		= "</h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.strbuf		= &strbuf;

	UHT_list_job (job);
}

static void
record_rerank (SPWAW_UHT_LIST_CBCTX &context)
{
	UtilStrbuf	*sb;

	if (!*context.data) *context.data = new UtilStrbuf (true);
	sb = (UtilStrbuf *)(*context.data);

	sb->printf ("%s %s %s: %s -> %s",
		context.uir->snap->strings.uid, context.uir->snap->data.dname, context.uir->snap->data.lname,
		context.puir->snap->strings.rank,
		context.uir->snap->strings.rank);
}

void
GuiRptBtlOvr::list_promotions (SPWAW_BATTLE *b, UtilStrbuf &strbuf)
{
	UHT_LIST_JOB job = { };

	job.what		= "Promotions";
	job.type		= UHT_LIST_BATTLE;
	job.in.b.battle		= b;
	job.how.status		= UHT_PROMOTED;
	job.how.allow_decomm	= true;
	job.dext.data		= record_rerank;
	job.out.skip_if_empty	= true;
	job.out.hdrpre		= "<pre><h3>";
	job.out.hdrpst		= "</h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.strbuf		= &strbuf;

	UHT_list_job (job);
}

void
GuiRptBtlOvr::list_demotions (SPWAW_BATTLE *b, UtilStrbuf &strbuf)
{
	UHT_LIST_JOB job = { };

	job.what		= "Demotions";
	job.type		= UHT_LIST_BATTLE;
	job.in.b.battle		= b;
	job.how.status		= UHT_DEMOTED;
	job.how.allow_decomm	= true;
	job.dext.data		= record_rerank;
	job.out.skip_if_empty	= true;
	job.out.hdrpre		= "<pre><h3>";
	job.out.hdrpst		= "</h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.strbuf		= &strbuf;

	UHT_list_job (job);
}

static void
record_commission (SPWAW_UHT_LIST_CBCTX &context)
{
	UtilStrbuf	*sb;

	if (!*context.data) *context.data = new UtilStrbuf (true);
	sb = (UtilStrbuf *)(*context.data);

	sb->printf ("%s %s %s",
		context.uir->snap->strings.uid, context.uir->snap->data.dname, context.uir->snap->data.lname);
}

void
GuiRptBtlOvr::list_commissions (SPWAW_BATTLE *b, UtilStrbuf &strbuf)
{
	UHT_LIST_JOB job = { };

	job.what		= "Commissions";
	job.type		= UHT_LIST_BATTLE;
	job.in.b.battle		= b;
	job.how.status		= UHT_COMMISSIONED;
	job.how.allow_decomm	= true;
	job.dext.data		= record_commission;
	job.out.skip_if_empty	= true;
	job.out.hdrpre		= "<pre><h3>";
	job.out.hdrpst		= "</h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.strbuf		= &strbuf;

	UHT_list_job (job);
}

static void
record_decommission (SPWAW_UHT_LIST_CBCTX &context)
{
	UtilStrbuf	*sb;

	if (!*context.data) *context.data = new UtilStrbuf (true);
	sb = (UtilStrbuf *)(*context.data);

	sb->printf ("%s %s %s",
		context.uir->snap->strings.uid, context.uir->snap->data.dname, context.uir->snap->data.lname);
}

void
GuiRptBtlOvr::list_decommissions (SPWAW_BATTLE *b, UtilStrbuf &strbuf)
{
	UHT_LIST_JOB job = { };

	job.what		= "Decommissions";
	job.type		= UHT_LIST_BATTLE;
	job.in.b.battle		= b;
	job.how.status		= UHT_DECOMMISSIONED;
	job.how.allow_decomm	= true;
	job.dext.data		= record_decommission;
	job.out.skip_if_empty	= true;
	job.out.hdrpre		= "<pre><h3>";
	job.out.hdrpst		= "</h3></pre>";
	job.out.lstpre		= "<pre>";
	job.out.lstpst		= "</pre>";
	job.out.strbuf		= &strbuf;

	UHT_list_job (job);
}


void
GuiRptBtlOvr::refresh (bool forced)
{
	MDLD_TREE_ITEM		*item;
	bool			skip_data;
	SPWAW_BATTLE		*p = NULL;
	char			date[32], buf[65536];
	UtilStrbuf		str(buf, sizeof (buf), true, true);
	SPWAW_PERIOD		span;

	DBG_TRACE_FENTER;

	item = (d.parent != NULL) ? d.parent->current() : NULL;

	skip_data  = !d.reftrack.changed (item);
	skip_data &= !GUIVALCHANGED (Vintel_mode);
	skip_data &= !forced;
	if (skip_data) goto skip_data_update;

	DBG_TRACE_UPDATE;

	p = (item != NULL) ? item->data.b : NULL;

	if (!p) {
		d.name->clear(); d.name->hide();
		d.intel->clear(); d.intel->hide();
		d.player1->setPixmap (*RES_flag (SPWAW_GAME_TYPE_SPWAW, SPWOOB_UNKNOWN));
		d.mission->setPixmap (*RES_pixmap (RID_MSSN_UNKNOWN));
		d.mission->setToolTip (QString());
		d.player2->setPixmap (*RES_flag (SPWAW_GAME_TYPE_SPWAW, SPWOOB_UNKNOWN));

		d.overview->setText ("No overview available yet.");
		d.losses.plr->clear();
		d.losses.opp->clear();
		d.changes->clear ();
	} else {
		DEVASSERT (p->tcnt > 0);

		if (p->name) {
			str.printf ("<h1>%s</h1>", p->name);
			d.name->setText (buf); d.name->show();
			str.clear();
		} else {
			d.name->clear(); d.name->hide();
		}

		intelmode2label (d.Vintel_mode, d.intel);

		d.player1->setPixmap (*RES_flagbyid (p->snap->game.battle.strings.flagid_p1));
		d.mission->setPixmap (*RES_mission (p->snap->game.battle.data.miss_p1, p->meeting));
		d.mission->setToolTip (QString (p->miss_p1) + QString(" against ") + QString(p->miss_p2));
		d.player2->setPixmap (*RES_flagbyid (p->snap->game.battle.strings.flagid_p2));

		str.printf ("<pre>");
		str.printf ("<h2>Battle at %s,\n%s %s against %s %s.</h2>", p->location,
			p->snap->game.battle.strings.people_p1,	p->miss_p1,
			p->snap->game.battle.strings.people_p2, p->miss_p2);

		SPWAW_date2str (&(p->bdate.date), date, sizeof (date));
		str.printf ("%s, %u turns.\n", date, p->snap->game.battle.data.turn_max);

		str.printf ("%.3f x %.3f km %s terrain, %s with %u meters visibility.\n",
			SPWAW_HEX2K (p->tfirst->snap->game.map.width),
			SPWAW_HEX2K (p->tfirst->snap->game.map.height),
			p->tfirst->snap->game.battle.strings.terrain,
			p->tfirst->snap->game.battle.strings.weather,
			SPWAW_HEX2M (p->tfirst->snap->game.battle.data.visibility));
		str.printf ("\n");

		str.printf ("%s force consists of %u units in %u formations (%u men).\n",
			p->snap->game.battle.strings.people_p1,
			p->props.pb_ucnt, p->props.pb_fcnt,
			p->snap->OOBp1.battle.stats.hcnt);
		if (p->props.ps_ucnt != 0) {
			str.printf ("\tCore force   : %u units in %u formations\n", p->props.pc_ucnt, p->props.pc_fcnt);
			str.printf ("\tSupport force: %u units in %u formations\n", p->props.ps_ucnt, p->props.ps_fcnt);
		}
		str.printf ("\n");

		str.printf ("%s force", p->snap->game.battle.strings.people_p2);
		switch (d.Vintel_mode) {
			case INTEL_MODE_FULL:
			case INTEL_MODE_LMTD:
			default:
				str.printf (" consists of %u units in %u formations (%u men).\n",
					p->props.ob_ucnt, p->props.ob_fcnt,
					p->snap->OOBp2.battle.stats.hcnt);
				break;
			case INTEL_MODE_NONE:
				str.printf (": number of units and formations unknown.\n");
				break;
		}
		str.printf ("\n");

		if (p->tcnt > 1) {
			SPWAW_date_delta (&(p->tfirst->tdate.date), &(p->tlast->tdate.date), &span);
			str.printf ("Recorded %u battle turns", p->tcnt);
			if (span.stamp) {
				str.printf (", spanning ");
				UTIL_fmt_fullspan (&span, &str);
			}
			str.printf (".\n");
		} else {
			str.printf ("Recorded 1 battle turn.\n");
		}
		str.printf ("</pre>");

		str.printf ("<pre>");
		switch (p->tlast->snap->game.battle.data.status){
			case SPWAW_BTDEPLOY:
				str.printf ("This battle is not yet started.");
				break;
			case SPWAW_BTSCORE:
				if (p->tlast->snap->game.campaign.data.P1result != SPWAW_BRUNKNOWN) {
					str.printf ("<h3>A %s:</h3>", SPWAW_bresult2str (p->tlast->snap->game.campaign.data.P1result));
				} else {
					str.printf ("<h3>Unfortunately, the battle result is not available.</h3>");
				}
				str.printf ("\t%s score:\t%u\n",
					p->snap->game.battle.strings.people_p1, p->tlast->snap->game.campaign.data.P1score);
				str.printf ("\t%s score:\t%u\n",
					p->snap->game.battle.strings.people_p2, p->tlast->snap->game.campaign.data.P2score);
				break;
			case SPWAW_BTBUSY:
			default:
				str.printf ("This battle is ongoing.");
				break;
		}
		str.printf ("</pre>");

		switch (p->tlast->snap->game.battle.data.status){
			case SPWAW_BTSCORE:
			case SPWAW_BTBUSY:
				str.printf ("<pre>");
				str.printf ("<h3>%s force:</h3>", p->snap->game.battle.strings.people_p1);
				str.printf ("\toverall readiness is %.0f %%.\n",
					p->tlast->snap->OOBp1.battle.attr.gen.ready * 100.0);
				str.printf ("\tachieved %u kills with %u losses.\n",
					p->tlast->snap->OOBp1.battle.attr.gen.kills - p->tlast->battle->snap->OOBp1.battle.attr.gen.kills,
					p->tlast->snap->OOBp1.battle.attr.gen.losses - p->tlast->battle->snap->OOBp1.battle.attr.gen.losses);
				str.printf ("\thas %u units left in %u formations. (%u men, %u crews).",
					p->tlast->snap->OOBp1.battle.stats.ustats.cnt_left,
					p->tlast->snap->OOBp1.battle.stats.fstats.cnt_left,
					p->tlast->snap->OOBp1.battle.stats.hcnt_left,
					p->tlast->snap->OOBp1.battle.crews.cnt
					);

				str.printf ("<h3>%s force:</h3>", p->snap->game.battle.strings.people_p2);
				switch (d.Vintel_mode) {
					case INTEL_MODE_FULL:
					default:
						str.printf ("\toverall readiness is %.0f %%.\n",
							p->tlast->snap->OOBp2.battle.attr.gen.ready * 100.0);
						str.printf ("\tachieved %u kills with %u losses.\n",
							p->tlast->snap->OOBp2.battle.attr.gen.kills - p->tlast->battle->snap->OOBp2.battle.attr.gen.kills,
							p->tlast->snap->OOBp2.battle.attr.gen.losses - p->tlast->battle->snap->OOBp2.battle.attr.gen.losses
							);
						str.printf ("\thas %u units left in %u formations. (%u men, %u crews)",
							p->tlast->snap->OOBp2.battle.stats.ustats.cnt_left,
							p->tlast->snap->OOBp2.battle.stats.fstats.cnt_left,
							p->tlast->snap->OOBp2.battle.stats.hcnt_left,
							p->tlast->snap->OOBp2.battle.crews.cnt
							);
						break;
					case INTEL_MODE_LMTD:
						str.printf ("\tachieved %u kills with %u losses.\n",
							p->tlast->snap->OOBp2.battle.attr.gen.kills - p->tlast->battle->snap->OOBp2.battle.attr.gen.kills,
							p->tlast->snap->OOBp2.battle.attr.gen.losses - p->tlast->battle->snap->OOBp2.battle.attr.gen.losses
							);
						str.printf ("\thas %u units left in %u formations.",
							p->tlast->snap->OOBp2.battle.stats.ustats.cnt_left,
							p->tlast->snap->OOBp2.battle.stats.fstats.cnt_left
							);
						break;
					case INTEL_MODE_NONE:
						str.printf ("\tachieved %u kills with %u losses.",
							p->tlast->snap->OOBp2.battle.attr.gen.kills - p->tlast->battle->snap->OOBp2.battle.attr.gen.kills,
							p->tlast->snap->OOBp2.battle.attr.gen.losses - p->tlast->battle->snap->OOBp2.battle.attr.gen.losses);
						break;
				}

				str.printf ("</pre>");
				break;
			case SPWAW_BTDEPLOY:
			default:
				break;
		}

		str.printf ("<pre>");
		str.printf ("<h3>Victory hex occupation:</h3>");
		str.printf ("\t%s force:\t%2u occupied (worth %u points)\n",
			p->snap->game.battle.strings.people_p1,
			p->tlast->snap->game.battle.stats.vhex_stats[SPWAW_VHP1].count,
			p->tlast->snap->game.battle.stats.vhex_stats[SPWAW_VHP1].value);
		switch (d.Vintel_mode) {
			case INTEL_MODE_FULL:
			case INTEL_MODE_LMTD:
			default:
				str.printf ("\t%s force:\t%2u occupied (worth %u points)\n",
					p->snap->game.battle.strings.people_p2,
					p->tlast->snap->game.battle.stats.vhex_stats[SPWAW_VHP2].count,
					p->tlast->snap->game.battle.stats.vhex_stats[SPWAW_VHP2].value);
				str.printf ("\tNeutral:\t\t%2u unoccupied (worth %u points)\n",
					p->tlast->snap->game.battle.stats.vhex_stats[SPWAW_VHN].count,
					p->tlast->snap->game.battle.stats.vhex_stats[SPWAW_VHN].value);
				break;
			case INTEL_MODE_NONE:
				str.printf ("\tOther:\t\t%2u contested (worth %u points)\n",
					p->tlast->snap->game.battle.stats.vhex_stats[SPWAW_VHP2].count +
					p->tlast->snap->game.battle.stats.vhex_stats[SPWAW_VHN].count,
					p->tlast->snap->game.battle.stats.vhex_stats[SPWAW_VHP2].value +
					p->tlast->snap->game.battle.stats.vhex_stats[SPWAW_VHN].value);

				break;
		}
		str.printf ("</pre>");

		d.overview->setText (buf);
		str.clear();

		switch (p->tlast->snap->game.battle.data.status){
			case SPWAW_BTSCORE:
				str.printf ("<pre>");
				str.printf ("<h3>%s battle losses:</h3>", p->snap->game.battle.strings.people_p1);
				str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P1BL.men, "men");
				str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P1BL.art, "artillery");
				str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P1BL.soft, "soft vehicles");
				str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P1BL.apc, "armoured personnel carriers");
				str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P1BL.afv, "armoured fighting vehicles");
				str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P1BL.gliders, "gliders");
				str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P1BL.air, "aircraft");
				str.printf ("</pre>");
				d.losses.plr->setText (buf);
				str.clear();

				str.printf ("<pre>");
				str.printf ("<h3>%s battle losses:</h3>", p->snap->game.battle.strings.people_p2);
				str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P2BL.men, "men");
				str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P2BL.art, "artillery");
				str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P2BL.soft, "soft vehicles");
				str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P2BL.apc, "armoured personnel carriers");
				str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P2BL.afv, "armoured fighting vehicles");
				str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P2BL.gliders, "gliders");
				str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P2BL.air, "aircraft");
				str.printf ("</pre>");
				d.losses.opp->setText (buf);
				str.clear();

				break;
			case SPWAW_BTBUSY:
			case SPWAW_BTDEPLOY:
			default:
				str.printf ("<pre><h3>Battle losses:</h3>\tnone recorded yet\n</pre>");
				d.losses.plr->setText (buf);
				str.clear();

				d.losses.opp->clear();
				break;
		}

		list_replacements (p, str);
		list_reassignments (p, str);
		list_upgrades (p, str);
		list_promotions (p, str);
		list_demotions (p, str);
		list_commissions (p, str);
		list_decommissions (p, str);

		d.changes->setText (buf);
	}

skip_data_update:
	if (isVisible()) {
		emit cmpcurr (NULL);
		emit cmpbase (NULL);
	}

	/* Make sure layout is updated if necessary */
	d.player1->updateGeometry();

	DBG_TRACE_FLEAVE;
}

void
GuiRptBtlOvr::intel_mode_set (INTEL_MODE mode)
{
	d.Vintel_mode = mode;

	refresh();
}