/** \file
 * The SPWaW war cabinet - GUI - battle turn report - overview.
 *
 * Copyright (C) 2005-2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_reports_bturn.h"

GuiRptTrnOvr::GuiRptTrnOvr (QWidget *P)
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
	GUINEW (d.spacer, QSpacerItem (1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum), ERR_GUI_REPORTS_INIT_FAILED, "spacer");

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
	d.layout->addItem   (d.spacer,	2, 3, 1, 1);

	GUINEW (d.label, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label");
	d.label->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label->setWordWrap (true);
	d.label->setFont (*d.font);

	GUINEW (d.bspacer, QSpacerItem (0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding), ERR_GUI_REPORTS_INIT_FAILED, "bspacer");

	d.layout->addWidget (d.label,	3, 0, 1, 4);
	d.layout->addItem   (d.bspacer,	4, 0, 1, 4);

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

GuiRptTrnOvr::~GuiRptTrnOvr (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
}

void
GuiRptTrnOvr::resizeEvent (QResizeEvent * /*event*/)
{
	refresh();
}

void
GuiRptTrnOvr::set_parent (GuiRptTrn *p)
{
	d.parent = p;
}

void
GuiRptTrnOvr::refresh (bool forced)
{
	MDLD_TREE_ITEM	*item;
	bool		skip_data;
	SPWAW_BTURN	*p = NULL;
	char		date[32], buf[4096], ptmp[32], otmp[32];
	UtilStrbuf	str(buf, sizeof (buf), true, true);
	UtilStrbuf		pstr(ptmp, sizeof (ptmp), true, true);
	UtilStrbuf		ostr(otmp, sizeof (otmp), true, true);

	DBG_TRACE_FENTER;

	item = (d.parent != NULL) ? d.parent->current() : NULL;

	skip_data  = !d.reftrack.changed (item);
	skip_data &= !GUIVALCHANGED (Vintel_mode);
	skip_data &= !forced;
	if (skip_data) goto skip_data_update;

	DBG_TRACE_UPDATE;

	p = (item != NULL) ? item->data.t : NULL;

	if (!p) {
		d.name->clear(); d.name->hide();
		d.intel->clear(); d.intel->hide();
		d.player1->setPixmap (*RES_flag (SPWAW_GAME_TYPE_SPWAW, SPWOOB_UNKNOWN));
		d.mission->setPixmap (*RES_pixmap (RID_MSSN_UNKNOWN));
		d.mission->setToolTip (QString());
		d.player2->setPixmap (*RES_flag (SPWAW_GAME_TYPE_SPWAW, SPWOOB_UNKNOWN));

		d.label->setText ("No overview available yet.");
	} else {
		if (p->battle->name) {
			str.printf ("<h1>%s</h1>", p->battle->name);
			d.name->setText (buf); d.name->show();
			str.clear();
		} else {
			d.name->clear(); d.name->hide();
		}

		intelmode2label (d.Vintel_mode, d.intel);

		d.player1->setPixmap (*RES_flagbyid (p->battle->snap->game.battle.strings.flagid_p1));
		d.mission->setPixmap (*RES_mission (p->battle->snap->game.battle.data.miss_p1, p->battle->meeting));
		d.mission->setToolTip (QString (p->battle->miss_p1) + QString(" against ") + QString(p->battle->miss_p2));
		d.player2->setPixmap (*RES_flagbyid (p->battle->snap->game.battle.strings.flagid_p2));

		str.printf ("<pre>");

		SPWAW_date2str (&(p->tdate.date), date, sizeof (date));
		str.printf ("<h2>%s, turn %u of %u.</h2>", date, p->tdate.turn, p->snap->game.battle.data.turn_max);

		str.printf ("%s start force consists of %u units in %u formations (%u men).\n",
			p->battle->snap->game.battle.strings.people_p1,
			p->info.pbir_battle.ucnt, p->info.pbir_battle.fcnt,
			p->snap->OOBp1.battle.stats.hcnt);
		if (p->info.pbir_support.ucnt != 0) {
			str.printf ("\tCore force   : %u units in %u formations\n", p->info.pbir_core.ucnt, p->info.pbir_core.fcnt);
			str.printf ("\tSupport force: %u units in %u formations\n", p->info.pbir_support.ucnt, p->info.pbir_support.fcnt);
		}
		str.printf ("\n");

		str.printf ("%s start force", p->battle->snap->game.battle.strings.people_p2);
		switch (d.Vintel_mode) {
			case INTEL_MODE_FULL:
			case INTEL_MODE_LMTD:
			default:
				str.printf (" consists of %u units in %u formations (%u men).",
					p->info.obir_battle.ucnt, p->info.obir_battle.fcnt,
					p->snap->OOBp2.battle.stats.hcnt);
				break;
			case INTEL_MODE_NONE:
				str.printf (": number of units and formations unknown.");
				break;
		}
		str.printf ("\n");
		str.printf ("</pre>");

		str.printf ("<pre>");
		switch (p->snap->game.battle.data.status){
			case SPWAW_BTDEPLOY:
				str.printf ("Forces are being deployed.\n");
				break;
			case SPWAW_BTSCORE:
				if (p->snap->game.campaign.data.P1result != SPWAW_BRUNKNOWN) {
					str.printf ("<h3>A %s:</h3>", SPWAW_bresult2str (p->snap->game.campaign.data.P1result));
				} else {
					str.printf ("<h3>Unfortunately, the battle result is not available.</h3>");
				}
				pstr.printf ("%s score:", p->battle->snap->game.battle.strings.people_p1);
				ostr.printf ("%s score:", p->battle->snap->game.battle.strings.people_p2);
				str.printf ("\t%-30s%4u\n",
					pstr.data(), p->snap->game.campaign.data.P1score);
				str.printf ("\t%-30s%4u\n",
					ostr.data(), p->snap->game.campaign.data.P2score);
				pstr.clear(); ostr.clear();
				break;
			case SPWAW_BTBUSY:
			default:
				str.printf ("The battle is ongoing.\n");
				break;
		}
		str.printf ("</pre>");

		switch (p->snap->game.battle.data.status){
			case SPWAW_BTSCORE:
			case SPWAW_BTBUSY:
				str.printf ("<pre>");
				str.printf ("<h3>%s force:</h3>", p->battle->snap->game.battle.strings.people_p1);
				str.printf ("\toverall readiness is %.0f %%.\n",
					p->snap->OOBp1.battle.attr.gen.ready * 100.0);
				str.printf ("\tachieved %u kills with %u losses.\n",
					p->snap->OOBp1.battle.attr.gen.kills - p->battle->snap->OOBp1.battle.attr.gen.kills,
					p->snap->OOBp1.battle.attr.gen.losses - p->battle->snap->OOBp1.battle.attr.gen.losses);
				str.printf ("\thas %u units left in %u formations. (%u men, %u crews).",
					p->snap->OOBp1.battle.stats.ustats.cnt_left,
					p->snap->OOBp1.battle.stats.fstats.cnt_left,
					p->snap->OOBp1.battle.stats.hcnt_left,
					p->snap->OOBp1.battle.crews.cnt
					);

				str.printf ("<h3>%s force:</h3>", p->battle->snap->game.battle.strings.people_p2);
				switch (d.Vintel_mode) {
					case INTEL_MODE_FULL:
					default:
						str.printf ("\toverall readiness is %.0f %%.\n",
							p->snap->OOBp2.battle.attr.gen.ready * 100.0);
						str.printf ("\tachieved %u kills with %u losses.\n",
							p->snap->OOBp2.battle.attr.gen.kills - p->battle->snap->OOBp2.battle.attr.gen.kills,
							p->snap->OOBp2.battle.attr.gen.losses - p->battle->snap->OOBp2.battle.attr.gen.losses
							);
						str.printf ("\thas %u units left in %u formations. (%u men, %u crews)",
							p->snap->OOBp2.battle.stats.ustats.cnt_left,
							p->snap->OOBp2.battle.stats.fstats.cnt_left,
							p->snap->OOBp2.battle.stats.hcnt_left,
							p->snap->OOBp2.battle.crews.cnt
							);
						break;
					case INTEL_MODE_LMTD:
						str.printf ("\tachieved %u kills with %u losses.\n",
							p->snap->OOBp2.battle.attr.gen.kills - p->battle->snap->OOBp2.battle.attr.gen.kills,
							p->snap->OOBp2.battle.attr.gen.losses - p->battle->snap->OOBp2.battle.attr.gen.losses
							);
						str.printf ("\thas %u units left in %u formations.",
							p->snap->OOBp2.battle.stats.ustats.cnt_left,
							p->snap->OOBp2.battle.stats.fstats.cnt_left
							);
						break;
					case INTEL_MODE_NONE:
						str.printf ("\tachieved %u kills with %u losses.",
							p->snap->OOBp2.battle.attr.gen.kills - p->battle->snap->OOBp2.battle.attr.gen.kills,
							p->snap->OOBp2.battle.attr.gen.losses - p->battle->snap->OOBp2.battle.attr.gen.losses);
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
		pstr.printf ("%s force:", p->snap->game.battle.strings.people_p1);
		ostr.printf ("%s force:", p->snap->game.battle.strings.people_p2);
		str.printf ("\t%-30s%4u occupied (worth %u points)\n",
			pstr.data(),
			p->snap->game.battle.stats.vhex_stats[SPWAW_VHP1].count,
			p->snap->game.battle.stats.vhex_stats[SPWAW_VHP1].value);

		switch (d.Vintel_mode) {
			case INTEL_MODE_FULL:
			case INTEL_MODE_LMTD:
			default:
				str.printf ("\t%-30s%4u occupied (worth %u points)\n",
					ostr.data(),
					p->snap->game.battle.stats.vhex_stats[SPWAW_VHP2].count,
					p->snap->game.battle.stats.vhex_stats[SPWAW_VHP2].value);
				str.printf ("\t%-30s%4u occupied (worth %u points)\n",
					"Neutral:",
					p->snap->game.battle.stats.vhex_stats[SPWAW_VHN].count,
					p->snap->game.battle.stats.vhex_stats[SPWAW_VHN].value);
				break;
			case INTEL_MODE_NONE:
				str.printf ("\t%-30s%4u contested (worth %u points)\n",
					"Other:",
					p->snap->game.battle.stats.vhex_stats[SPWAW_VHP2].count +
					p->snap->game.battle.stats.vhex_stats[SPWAW_VHN].count,
					p->snap->game.battle.stats.vhex_stats[SPWAW_VHP2].value +
					p->snap->game.battle.stats.vhex_stats[SPWAW_VHN].value);
				break;
		}
		pstr.clear(); ostr.clear();
		str.printf ("</pre>");

		d.label->setText (buf);
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
GuiRptTrnOvr::intel_mode_set (INTEL_MODE mode)
{
	d.Vintel_mode = mode;

	refresh();
}