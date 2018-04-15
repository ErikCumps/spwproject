/** \file
 * The SPWaW war cabinet - GUI - battle turn report - overview.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
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

	d.layout->addWidget (d.player1,	0, 0, 1, 1);
	d.layout->addWidget (d.mission,	0, 1, 1, 1);
	d.layout->addWidget (d.player2,	0, 2, 1, 1);
	d.layout->addItem   (d.spacer,	0, 3, 1, 1);

	GUINEW (d.label, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label");
	d.label->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label->setWordWrap (true);
	d.label->setFont (*d.font);

	d.layout->addWidget (d.label, 1, 0, 1, 4);

	setWidget(d.frame);
	setWidgetResizable (true);

	if (!connect (this, SIGNAL (cmpcurr(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpcurr(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <cmpcurr> to <dossier:set_cmpcurr>");

	if (!connect (this, SIGNAL (cmpbase(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpbase(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <cmpbase> to <dossier:set_cmpbase>");

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
GuiRptTrnOvr::refresh (void)
{
	MDLD_TREE_ITEM	*item;
	SPWAW_BTURN	*p = NULL;
	char		date[32], buf[4096];
	UtilStrbuf	str(buf, sizeof (buf), true, true);

	DBG_TRACE_FENTER;

	item = (d.parent != NULL) ? d.parent->current() : NULL;
	if (!d.reftrack.changed (item)) goto skip_data_update;

	DBG_TRACE_UPDATE;

	p = (item != NULL) ? item->data.t : NULL;

	if (!p) {
		d.player1->setPixmap (*RES_flag (0));
		d.mission->setPixmap (*RES_pixmap (RID_MSSN_UNKNOWN));
		d.mission->setToolTip (QString());
		d.player2->setPixmap (*RES_flag (0));

		d.label->setText ("No overview available yet.");
	} else {
		d.player1->setPixmap (*RES_flag (p->battle->OOB_p1));
		d.mission->setPixmap (*RES_mission (p->battle->snap->game.battle.data.miss_p1, p->battle->meeting));
		d.mission->setToolTip (QString (p->battle->miss_p1) + QString(" against ") + QString(p->battle->miss_p2));
		d.player2->setPixmap (*RES_flag (p->battle->OOB_p2));

		str.printf ("<pre>");

		SPWAW_date2str (&(p->date), date, sizeof (date));
		str.printf ("<h2>%s, turn %u of %u.</h2>", date, p->turn, p->snap->game.battle.data.turn_max);

		str.printf ("%s start force consists of %u units in %u formations (%u men).\n", SPWAW_oob_people (p->battle->OOB_p1),
			p->info.pbir.ucnt, p->info.pbir.fcnt, p->snap->OOBp1.battle.stats.hcnt);
		str.printf ("%s start force consists of %u units in %u formations (%u men).\n", SPWAW_oob_people (p->battle->OOB_p2),
			p->info.obir.ucnt, p->info.obir.fcnt, p->snap->OOBp2.battle.stats.hcnt);
		str.printf ("\n");

		switch (p->snap->game.battle.data.status){
			case SPWAW_BTDEPLOY:
				str.printf ("Forces are being deployed.\n");
				break;
			case SPWAW_BTSCORE:
				str.printf ("The battle is finished.\n");
				break;
			case SPWAW_BTBUSY:
			default:
				str.printf ("The battle is ongoing.\n");
				break;
		}
		str.printf ("</pre>");

		switch (p->snap->game.battle.data.status){
			case SPWAW_BTDEPLOY:
				break;
			case SPWAW_BTBUSY:
			case SPWAW_BTSCORE:
			default:
				str.printf ("<pre>");
				str.printf ("<h3>%s force:</h3>", SPWAW_oob_people (p->battle->OOB_p1));
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

				str.printf ("<h3>%s force:</h3>", SPWAW_oob_people (p->battle->OOB_p2));
				str.printf ("\toverall readiness is %.0f %%.\n",
					p->snap->OOBp2.battle.attr.gen.ready * 100.0);
				str.printf ("\tachieved %u kills with %u losses.\n",
					p->snap->OOBp2.battle.attr.gen.kills - p->battle->snap->OOBp2.battle.attr.gen.kills,
					p->snap->OOBp2.battle.attr.gen.losses - p->battle->snap->OOBp2.battle.attr.gen.losses);
				str.printf ("\thas %u units left in %u formations. (%u men, %u crews).",
					p->snap->OOBp2.battle.stats.ustats.cnt_left,
					p->snap->OOBp2.battle.stats.fstats.cnt_left,
					p->snap->OOBp2.battle.stats.hcnt_left,
					p->snap->OOBp2.battle.crews.cnt
					);

				str.printf ("</pre>");
				break;
		}

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
