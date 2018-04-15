/** \file
 * The SPWaW war cabinet - GUI - dossier report - overview.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_reports_dossier.h"

// TODO: consider externalizing
#define	LISTMAX	5

GuiRptDsrOvr::GuiRptDsrOvr (QWidget *P)
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
	delete d.model;
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

void
GuiRptDsrOvr::list_promotions (SPWAW_DOSSIER *d, bool reverse, char *buf, unsigned int size, int &icnt)
{
	UtilStrbuf		str(buf, size, true, true);
	SPWAW_BATTLE		*b;
	int			i;
	SPWAW_SNAP_OOB_UEL	*fup, *lup, *up;

	icnt = 0;

	for (i=0; i<d->ucnt; i++) {
		USHORT	idx, nidx;
		bool	skip, report;

		idx = nidx = i; b = d->bfirst; skip = false; report = false;
		up = fup = b->info_sob->pbir.uir[idx].snap;
		while (b->next) {
			nidx = b->ra[idx].dst;
			if (b->ra[idx].rpl || (nidx == SPWAW_BADIDX)) { skip = true; break; }

			lup = b->next->info_sob->pbir.uir[nidx].snap;
			if (up->data.rank != lup->data.rank) report = true;

			idx = nidx; up = lup; b = b->next;
		}
		if (skip || !report) continue;

		if (!reverse) {
			str.printf ("%s %s %s: %s", lup->strings.uid, lup->data.type, lup->data.name, fup->strings.rank);
			idx = nidx = i; b = d->bfirst;
			while (b->next) {
				nidx = b->ra[idx].dst;
				fup = b->info_sob->pbir.uir[idx].snap;
				lup = b->next->info_sob->pbir.uir[nidx].snap;
				if (fup->data.rank != lup->data.rank) {
					str.printf (" -&gt; <small>(%02.2d/%2.2d)</small> %s",
						b->date.month, b->date.year - 1900, lup->strings.rank);
				}
				idx = nidx; b = b->next;
			}
		} else {
			str.printf ("%s %s %s: %s", lup->strings.uid, lup->data.type, lup->data.name, lup->strings.rank);
			idx = nidx = lup->data.uidx; b = d->blast;
			while (b->prev) {
				nidx = b->ra[idx].src;
				fup = b->info_sob->pbir.uir[idx].snap;
				lup = b->prev->info_sob->pbir.uir[nidx].snap;
				if (fup->data.rank != lup->data.rank) {
					str.printf (" <small>(%02.2d/%2.2d)</small> &lt;- %s",
						b->date.month, b->date.year - 1900, lup->strings.rank);
				}
				idx = nidx; b = b->prev;
			}
		}
		str.printf ("\n");

		icnt++;
	}
}

void
GuiRptDsrOvr::list_upgrades (SPWAW_DOSSIER *d, bool reverse, char *buf, unsigned int size, int &icnt)
{
	UtilStrbuf		str(buf, size, true, true);
	int			i;
	SPWAW_BATTLE		*b;
	SPWAW_SNAP_OOB_UEL	*fup, *lup, *up;

	icnt = 0;

	for (i=0; i<d->ucnt; i++) {
		USHORT	idx, nidx;
		bool	skip, report;

		idx = nidx = i; b = d->bfirst; skip = false; report = false;
		up = fup = b->info_sob->pbir.uir[idx].snap;
		while (b->next) {
			nidx = b->ra[idx].dst;
			if (b->ra[idx].rpl || (nidx == SPWAW_BADIDX)) { skip = true; break; }

			lup = b->next->info_sob->pbir.uir[nidx].snap;
			if (strcmp (up->data.type, lup->data.type) != 0) report = true;

			idx = nidx; up = lup; b = b->next;
		}
		if (skip || !report) continue;

		if (!reverse) {
			str.printf ("%s %s: %s", lup->strings.uid, lup->data.name, fup->data.type);
			idx = nidx = i; b = d->bfirst;
			while (b->next) {
				nidx = b->ra[idx].dst;
				fup = b->info_sob->pbir.uir[idx].snap;
				lup = b->next->info_sob->pbir.uir[nidx].snap;
				if (strcmp (fup->data.type, lup->data.type) != 0) {
					str.printf (" -&gt; <small>(%02.2d/%2.2d)</small> %s",
						b->date.month, b->date.year - 1900, lup->data.type);
				}
				idx = nidx; b = b->next;
			}
		} else {
			str.printf ("%s %s: %s", lup->strings.uid, lup->data.name, lup->data.type);
			idx = nidx = lup->data.uidx; b = d->blast;
			while (b->prev) {
				nidx = b->ra[idx].src;
				fup = b->info_sob->pbir.uir[idx].snap;
				lup = b->prev->info_sob->pbir.uir[nidx].snap;
				if (strcmp (fup->data.type, lup->data.type) != 0) {
					str.printf (" <small>(%02.2d/%2.2d)</small> &lt;- %s",
						b->date.month, b->date.year - 1900, lup->data.type);
				}
				idx = nidx; b = b->prev;
			}
		}
		str.printf ("\n");

		icnt++;
	}
}

void
GuiRptDsrOvr::refresh (bool forced)
{
	MDLD_TREE_ITEM	*item;
	SPWAW_DOSSIER	*p = NULL;
	char		buf[8*4096], buf2[4096];
	UtilStrbuf	str(buf, sizeof (buf), true, true);
	int		i, ccnt;
	SPWAW_PERIOD	span;
	SPWAW_BATTLE	*b = NULL;

	DBG_TRACE_FENTER;

	item = (d.parent != NULL) ? d.parent->current() : NULL;
	if (!d.reftrack.changed (item) && !forced) goto skip_data_update;

	DBG_TRACE_UPDATE;

	p = (item != NULL) ? item->data.d : NULL;

	if (!p) {
		d.flag->setPixmap (*RES_flag (0));
		d.overview->setText ("No overview available yet.");
		d.losses.plr->clear();
		d.losses.opp->clear();
		d.changes->clear();
	} else {
		if (p->type == SPWAW_CAMPAIGN_DOSSIER) {
			d.flag->setPixmap (*RES_flag (p->OOB));
			d.flag->setHidden(false);
		} else {
			d.flag->setHidden(true);
		}

		str.printf ("<pre>");

		str.printf ("<h1>%s</h1>", p->name);
		str.printf ("<h2>%s</h2>", p->comment);
		if (p->comment[strlen (p->comment) - 1] == '\n') str.del (1);

		if (p->bcnt <= 1) {
			if (p->bcnt == 0) {
				str.printf ("No battles recorded yet.\n");
			} else {
				str.printf ("One battle recorded.\n");
			}
		} else {
			SPWAW_date_delta (&(p->bfirst->date), &(p->blast->date), &span);
			str.printf ("%u battles recorded, spanning ", p->bcnt);
			UTIL_fmt_longspan (&span, &str);
			str.printf (".\n");
		}
		if (p->bcnt >= 1) {
			ccnt = 0;
			for (i=0; i<p->bcnt; i++) {
				b = p->blist[i];
				if (b->tcnt == 0) continue;

				switch (b->tlist[b->tcnt-1]->snap->game.battle.data.status) {
					case SPWAW_BTSCORE:
						ccnt++;
						break;
					case SPWAW_BTDEPLOY:
					case SPWAW_BTBUSY:
					default:
						break;
				}
			}

			if (ccnt == 1) {
				str.printf ("Of these, records show 1 concluded battle.\n");
			} else {
				str.printf ("Of these, records show %u concluded battles.\n", ccnt);
			}
		}
		str.printf ("\n");

		if (p->type == SPWAW_CAMPAIGN_DOSSIER) {
			str.printf ("%s core force consists of %u units in %u formations.", SPWAW_oob_people (p->OOB), p->ucnt, p->fcnt);
		} else {
			str.printf ("Campaign tracking is not available for this dossier.");
		}

		d.overview->setText (buf);
		str.clear();

		if (p->type == SPWAW_CAMPAIGN_DOSSIER) {
			if (p->bcnt) {
				str.printf ("<pre>");
				str.printf ("<h3>%s campaign losses:</h3>", SPWAW_oob_people (p->OOB));
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
				int	cnt = 0;

				list_promotions (p, false, buf2, sizeof (buf2), cnt);
				if (cnt) {
					str.printf ("<pre><h3>Promotions:</h3>");
					str.add (buf2);
					str.printf ("</pre>");
				}

				list_upgrades (p, true, buf2, sizeof (buf2), cnt);
				if (cnt) {
					str.printf ("<pre><h3>Upgrades:</h3>");
					str.add (buf2);
					str.printf ("</pre>");
				}
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
