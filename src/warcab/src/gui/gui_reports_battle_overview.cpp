/** \file
 * The SPWaW war cabinet - GUI - battle report - overview.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
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

	GUINEW (d.model, ModelRoster (), ERR_GUI_REPORTS_INIT_FAILED, "data model");

	GUINEW (d.font, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_REPORTS_INIT_FAILED, "font");

	GUINEW (d.frame, QFrame (this), ERR_GUI_REPORTS_INIT_FAILED, "frame");

	GUINEW (d.layout, QGridLayout (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "layout");
	d.layout->setVerticalSpacing (20);

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

	d.layout->addWidget (d.player1,	0, 0, 1, 1);
	d.layout->addWidget (d.mission,	0, 1, 1, 1);
	d.layout->addWidget (d.player2,	0, 2, 1, 1);
	d.layout->addItem   (d.rspacer,	0, 3, 1, 1);

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

	d.layout->addWidget (d.overview,	1, 0, 1, 4);
	d.layout->addLayout (d.losses.layout,	2, 0, 1, 4);
	d.layout->addWidget (d.changes,		3, 0, 1, 4);
	d.layout->addItem   (d.bspacer,		4, 0, 1, 4);

	setWidget(d.frame);
	setWidgetResizable (true);

	if (!connect (this, SIGNAL (cmpcurr(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpcurr(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <cmpcurr> to <dossier:set_cmpcurr>");

	if (!connect (this, SIGNAL (cmpbase(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpbase(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <cmpbase> to <dossier:set_cmpbase>");

	SET_GUICLS_NOERR;
}

GuiRptBtlOvr::~GuiRptBtlOvr (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
	delete d.model;
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

void
GuiRptBtlOvr::list_replacements (SPWAW_BATTLE *b, char *buf, unsigned int size, int &icnt)
{
	UtilStrbuf	str(buf, size, true, true);
	SPWAW_BATTLE	*nb;

	nb = b->next ? b->next : b;

	icnt = 0;
	for (int i=0; i<b->dossier->ucnt; i++) {
		SPWAW_SNAP_OOB_UEL *up = &(b->snap->OOBp1.core.units.list[i]);
		if (b->ra[i].rpl) {
			SPWAW_SNAP_OOB_UEL *nup = &(nb->snap->OOBp1.core.units.list[b->ra[i].dst]);
			str.printf ("\t%s %s: %s %s -> %s %s\n",
				up->strings.uid, up->data.type,
				up->strings.rank, up->data.name,
				nup->strings.rank, nup->data.name);
			icnt++;
		}
	}
}

void
GuiRptBtlOvr::list_reassignments (SPWAW_BATTLE *b, char *buf, unsigned int size, int &icnt)
{
	UtilStrbuf	str(buf, size, true, true);
	SPWAW_BATTLE	*nb;

	nb = b->next ? b->next : b;

	icnt = 0;
	for (int i=0; i<b->dossier->ucnt; i++) {
		SPWAW_SNAP_OOB_UEL *up = &(b->snap->OOBp1.core.units.list[i]);
		if ((b->ra[i].dst != i) && !b->ra[i].rpl &&(b->ra[i].src != SPWAW_BADIDX) && (b->ra[i].dst != SPWAW_BADIDX)) {
			SPWAW_SNAP_OOB_UEL *nup = &(nb->snap->OOBp1.core.units.list[b->ra[i].dst]);
			str.printf ("\t%s %s: %s -> %s\n",
				up->data.type, up->data.name,
				up->strings.uid,
				nup->strings.uid);
			icnt++;
		}
	}
}

void
GuiRptBtlOvr::list_promotions (SPWAW_BATTLE *b, bool promo, char *buf, unsigned int size, int &icnt)
{
	UtilStrbuf		str(buf, size, true, true);
	char			tbuf[4096];
	UtilStrbuf		tstr(tbuf, sizeof (tbuf), true, true);
	SPWAW_BATTLE		*nb;
	SPWAW_SNAP_OOB_UEL	*up, *nup;

	nb = b->next ? b->next : b;

	icnt = 0;

	for (int i=0; i<b->dossier->ucnt; i++) {
		if (b->ra[i].rpl || (b->ra[i].dst == SPWAW_BADIDX)) continue;

		up = b->info_sob->pbir.uir[i].snap;
		nup = nb->info_sob->pbir.uir[b->ra[i].dst].snap;

		if (up->data.rank == nup->data.rank) continue;

		if (promo) {
			if (up->data.rank > nup->data.rank) continue;
		} else {
			if (up->data.rank < nup->data.rank) continue;
		}

		str.printf ("\t%s %s %s: %s -> %s\n",
			up->strings.uid, up->data.type, up->data.name, up->strings.rank, nup->strings.rank);
		icnt++;
	}
}

void
GuiRptBtlOvr::list_upgrades (SPWAW_BATTLE *b, char *buf, unsigned int size, int &icnt)
{
	UtilStrbuf		str(buf, size, true, true);
	char			tbuf[4096];
	UtilStrbuf		tstr(tbuf, sizeof (tbuf), true, true);
	ModelRosterRawData	data[LISTMAX];
	int			idx;
	bool			stop;
	SPWAW_BATTLE		*nb;
	SPWAW_SNAP_OOB_UEL	*up, *nup;

	nb = b->next ? b->next : b;

	icnt = 0;

	d.model->set_dltsort (true);
	d.model->sort (MDLR_COLUMN_UNIT, Qt::DescendingOrder);

	idx = 0; stop = false;
	while (1) {
		int cnt = d.model->rawdata (idx, MDLR_COLUMN_UNIT, data, LISTMAX);
		if (!cnt) break;

		tstr.clear();
		for (int i=0; i<cnt; i++) {
			if (!SPWDLT_check(data[i].dlt)) { stop = true; break; }

			up = &(b->snap->OOBp1.core.units.list[data[i].idx]);
			nup = &(nb->snap->OOBp1.core.units.list[b->ra[data[i].idx].dst]);
			tstr.printf ("\t%s %s %s: %s -> %s\n",
				data[i].uir->snap->strings.uid, data[i].uir->snap->strings.rank, data[i].uir->snap->data.name,
				up->data.type,
				nup->data.type);
			icnt++;
		}
		str.add (tbuf);

		if (stop) break;
		idx += cnt;
	}
}

void
GuiRptBtlOvr::refresh (void)
{
	MDLD_TREE_ITEM		*item;
	SPWAW_BATTLE		*p = NULL;
	char			date[32], buf[8*4096], buf2[4096];
	UtilStrbuf		str(buf, sizeof (buf), true, true);
	UtilStrbuf		str2(buf2, sizeof (buf2), true, true);
	SPWAW_PERIOD		span;
	int			cnt;

	DBG_TRACE_FENTER;

	item = (d.parent != NULL) ? d.parent->current() : NULL;
	if (!d.reftrack.changed (item)) goto skip_data_update;

	DBG_TRACE_UPDATE;

	p = (item != NULL) ? item->data.b : NULL;

	if (!p) {
		d.player1->setPixmap (*RES_flag (0));
		d.mission->setPixmap (*RES_pixmap (RID_MSSN_UNKNOWN));
		d.mission->setToolTip (QString());
		d.player2->setPixmap (*RES_flag (0));

		d.overview->setText ("No overview available yet.");
		d.losses.plr->clear();
		d.losses.opp->clear();
		d.changes->clear ();
	} else {
		DEVASSERT (p->tcnt > 0);

		d.model->load (p->next ? p->next : p, p, true, true);

		d.player1->setPixmap (*RES_flag (p->dossier->OOB));
		d.mission->setPixmap (*RES_mission (p->snap->game.battle.data.miss_p1, p->meeting));
		d.mission->setToolTip (QString (p->miss_p1) + QString(" against ") + QString(p->miss_p2));
		d.player2->setPixmap (*RES_flag (p->OOB));

		str.printf ("<pre>");
		str.printf ("<h2>Battle at %s,\n%s %s against %s %s.</h2>", p->location,
			SPWAW_oob_people (p->dossier->OOB), p->miss_p1,
			SPWAW_oob_people (p->OOB), p->miss_p2);

		SPWAW_date2str (&(p->date), date, sizeof (date));
		str.printf ("%s, %u turns.\n", date, p->snap->game.battle.data.turn_max);

		str.printf ("%.3f x %.3f km %s terrain, %s with %u meters visibility.\n",
			SPWAW_HEX2K (p->tfirst->snap->game.map.width),
			SPWAW_HEX2K (p->tfirst->snap->game.map.height),
			p->tfirst->snap->game.battle.strings.terrain,
			p->tfirst->snap->game.battle.strings.weather,
			SPWAW_HEX2M (p->tfirst->snap->game.battle.data.visibility));
		str.printf ("\n");

		str.printf ("%s start force consists of %u units in %u formations (%u men).\n", SPWAW_oob_people (p->dossier->OOB),
			p->info_sob->pbir.ucnt, p->info_sob->pbir.fcnt, p->snap->OOBp1.battle.stats.hcnt);
		str.printf ("%s start force consists of %u units in %u formations (%u men).\n", SPWAW_oob_people (p->OOB),
			p->info_sob->obir.ucnt, p->info_sob->obir.fcnt, p->snap->OOBp2.battle.stats.hcnt);
		str.printf ("\n");

		if (p->tcnt > 1) {
			SPWAW_date_delta (&(p->tfirst->date), &(p->tlast->date), &span);
			str.printf ("Recorded %u battle turns spanning ", p->tcnt);
			UTIL_fmt_shortspan (&span, &str);
			str.printf (".\n");
		} else {
			str.printf ("Recorded 1 battle turn.\n");
		}
		str.printf ("\n");

		switch (p->tlast->snap->game.battle.data.status){
			case SPWAW_BTDEPLOY:
				str.printf ("This battle is not yet started.");
				break;
			case SPWAW_BTSCORE:
				str.printf ("This battle is finished.");
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
				str.printf ("<h3>%s force:</h3>", SPWAW_oob_people (p->dossier->OOB));
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

				str.printf ("<h3>%s force:</h3>", SPWAW_oob_people (p->OOB));
				str.printf ("\toverall readiness is %.0f %%.\n",
					p->tlast->snap->OOBp2.battle.attr.gen.ready * 100.0);
				str.printf ("\tachieved %u kills with %u losses.\n",
					p->tlast->snap->OOBp2.battle.attr.gen.kills - p->tlast->battle->snap->OOBp2.battle.attr.gen.kills,
					p->tlast->snap->OOBp2.battle.attr.gen.losses - p->tlast->battle->snap->OOBp2.battle.attr.gen.losses);
				str.printf ("\thas %u units left in %u formations. (%u men, %u crews).",
					p->tlast->snap->OOBp2.battle.stats.ustats.cnt_left,
					p->tlast->snap->OOBp2.battle.stats.fstats.cnt_left,
					p->tlast->snap->OOBp2.battle.stats.hcnt_left,
					p->tlast->snap->OOBp2.battle.crews.cnt
					);

				str.printf ("</pre>");
				break;
			case SPWAW_BTDEPLOY:
			default:
				break;
		}

		d.overview->setText (buf);
		str.clear();

		str.printf ("<pre>");
		str.printf ("<h3>%s battle losses:</h3>", SPWAW_oob_people (p->dossier->OOB));
		if (p->tlist[p->tcnt-1]->snap->game.battle.data.status != SPWAW_BTDEPLOY) {
			str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P1BL.men, "men");
			str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P1BL.art, "artillery");
			str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P1BL.soft, "soft vehicles");
			str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P1BL.apc, "armoured personnel carriers");
			str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P1BL.afv, "armoured fighting vehicles");
			str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P1BL.gliders, "gliders");
			str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P1BL.air, "aircraft");
		} else {
			str.printf ("  none yet\n");
		}
		str.printf ("</pre>");
		d.losses.plr->setText (buf);
		str.clear();

		str.printf ("<pre>");
		str.printf ("<h3>%s battle losses:</h3>", SPWAW_oob_people (p->OOB));
		if (p->tlist[p->tcnt-1]->snap->game.battle.data.status != SPWAW_BTDEPLOY) {
			str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P2BL.men, "men");
			str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P2BL.art, "artillery");
			str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P2BL.soft, "soft vehicles");
			str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P2BL.apc, "armoured personnel carriers");
			str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P2BL.afv, "armoured fighting vehicles");
			str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P2BL.gliders, "gliders");
			str.printf ("  %6u %s\n", p->tlast->snap->game.campaign.data.P2BL.air, "aircraft");
		} else {
			str.printf ("  none yet\n");
		}
		str.printf ("</pre>");
		d.losses.opp->setText (buf);
		str.clear();

		list_replacements (p, buf2, sizeof (buf2), cnt);
		if (cnt) {
			str.printf ("<pre>");
			str.printf ("<h3>Replacements:</h3>");
			str.add (buf2);
			str.printf ("</pre>");
		}

		list_reassignments (p, buf2, sizeof (buf2), cnt);
		if (cnt) {
			str.printf ("<pre>");
			str.printf ("<h3>Reassignments:</h3>");
			str.add (buf2);
			str.printf ("</pre>");
		}

		list_promotions (p, true, buf2, sizeof (buf2), cnt);
		if (cnt) {
			str.printf ("<pre>");
			str.printf ("<h3>Promotions:</h3>");
			str.add (buf2);
			str.printf ("</pre>");
		}

		list_promotions (p, false, buf2, sizeof (buf2), cnt);
		if (cnt) {
			str.printf ("<pre>");
			str.printf ("<h3>Demotions:</h3>");
			str.add (buf2);
			str.printf ("</pre>");
		}

		list_upgrades (p, buf2, sizeof (buf2), cnt);
		if (cnt) {
			str.printf ("<pre>");
			str.printf ("<h3>Upgrades:</h3>");
			str.add (buf2);
			str.printf ("</pre>");
		}

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
