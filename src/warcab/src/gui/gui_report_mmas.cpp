/** \file
 * The SPWaW war cabinet - GUI - min-max-average-spread report.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_report_mmas.h"

GuiRptMMAS::GuiRptMMAS (QWidget *P)
	: QScrollArea (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	GUINEW (d.mmas, GuiMMAS (this), ERR_GUI_REPORTS_ROSTER_INIT_FAILED, "mmas");
	GUIERR (d.mmas, ERR_GUI_REPORTS_ROSTER_INIT_FAILED);

	setWidget (d.mmas);
	setWidgetResizable (true);

	SET_GUICLS_NOERR;
}

GuiRptMMAS::~GuiRptMMAS (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
}

void
GuiRptMMAS::set_parent (GuiRptDsr *parent, bool pflag)
{
	d.mmas->set_parent (parent, pflag);
}

void
GuiRptMMAS::set_parent (GuiRptBtl *parent, bool pflag, bool core)
{
	d.mmas->set_parent (parent, pflag, core);
}

void
GuiRptMMAS::set_parent (GuiRptTrn *parent, bool pflag)
{
	d.mmas->set_parent (parent, pflag);
}

void
GuiRptMMAS::refresh (void)
{
	DBG_TRACE_FENTER;

	d.mmas->refresh();

	DBG_TRACE_FLEAVE;
}
