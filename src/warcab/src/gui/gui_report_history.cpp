/** \file
 * The SPWaW war cabinet - GUI - force history report.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_report_history.h"

GuiRptHst::GuiRptHst (QWidget *P)
	: QScrollArea (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	GUINEW (d.history, GuiHistory (this), ERR_GUI_REPORTS_HISTORY_INIT_FAILED, "history");
	GUIERR (d.history, ERR_GUI_REPORTS_HISTORY_INIT_FAILED);

	setWidget (d.history);
	setWidgetResizable (true);

	SET_GUICLS_NOERR;
}

GuiRptHst::~GuiRptHst (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
}

void
GuiRptHst::set_parent (GuiRptDsr *parent, bool pflag)
{
	d.history->set_parent (parent, pflag);
}

void
GuiRptHst::set_parent (GuiRptBtl *parent, bool pflag, bool core)
{
	d.history->set_parent (parent, pflag, core);
}

void
GuiRptHst::refresh (bool forced)
{
	DBG_TRACE_FENTER;

	d.history->refresh(forced);

	DBG_TRACE_FLEAVE;
}
