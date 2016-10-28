/** \file
 * The SPWaW war cabinet - GUI - unit roster report.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_report_roster.h"

GuiRptRst::GuiRptRst (QWidget *P)
	: QScrollArea (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	GUINEW (d.roster, GuiRoster (this), ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "roster");
	GUIERR (d.roster, ERR_GUI_REPORTS_ROSTER_INIT_FAILED);

	setWidget (d.roster);
	setWidgetResizable (true);

	SET_GUICLS_NOERR;
}

GuiRptRst::~GuiRptRst (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
}

void
GuiRptRst::set_parent (GuiRptDsr *parent, bool pflag)
{
	d.roster->set_parent (parent, pflag);
}

void
GuiRptRst::set_parent (GuiRptBtl *parent, bool pflag, bool core)
{
	d.roster->set_parent (parent, pflag, core);
}

void
GuiRptRst::set_parent (GuiRptTrn *parent, bool pflag, bool core)
{
	d.roster->set_parent (parent, pflag, core);
}

void
GuiRptRst::refresh (void)
{
	DBG_TRACE_FENTER;

	d.roster->refresh();

	DBG_TRACE_FLEAVE;
}
