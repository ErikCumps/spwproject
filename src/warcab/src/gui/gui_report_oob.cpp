/** \file
 * The SPWaW war cabinet - GUI - order of battle report.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_report_oob.h"

GuiRptOob::GuiRptOob (QWidget *P)
	: QScrollArea (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	GUINEW (d.oob, GuiOob (this), ERR_GUI_REPORTS_OOB_INIT_FAILED, "oob");
	GUIERR (d.oob, ERR_GUI_REPORTS_OOB_INIT_FAILED);

	setWidget (d.oob);
	setWidgetResizable (true);

	SET_GUICLS_NOERR;
}

GuiRptOob::~GuiRptOob (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
}

void
GuiRptOob::set_parent (GuiRptDsr *parent, bool pflag)
{
	d.oob->set_parent (parent, pflag);
}

void
GuiRptOob::set_parent (GuiRptBtl *parent, bool pflag, bool core)
{
	d.oob->set_parent (parent, pflag, core);
}

void
GuiRptOob::set_parent (GuiRptTrn *parent, bool pflag, bool core)
{
	d.oob->set_parent (parent, pflag, core);
}

void
GuiRptOob::refresh (void)
{
	DBG_TRACE_FENTER;

	d.oob->refresh();

	DBG_TRACE_FLEAVE;
}
