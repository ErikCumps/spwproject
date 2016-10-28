/** \file
 * The SPWaW war cabinet - GUI - battle turn report - force.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_reports_bturn.h"

GuiRptTrnFrc::GuiRptTrnFrc (QWidget *P)
	: QTabWidget (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Fix the QTabWidget background color */
	setPalette (GUI_APP->palette());
	setAutoFillBackground (true);

	GUINEW (d.disabled_label, QLabel ("There is no support force."), ERR_GUI_REPORTS_INIT_FAILED, "disabled_label");
	d.disabled_label->setAlignment (Qt::AlignHCenter|Qt::AlignVCenter);

	GUINEW (d.kill, GuiRptKill (), ERR_GUI_REPORTS_INIT_FAILED, "kill");
	GUIERR (d.kill, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.loss, GuiRptLoss (), ERR_GUI_REPORTS_INIT_FAILED, "loss");
	GUIERR (d.loss, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.oob, GuiRptOob (), ERR_GUI_REPORTS_INIT_FAILED, "oob");
	GUIERR (d.oob, ERR_GUI_REPORTS_INIT_FAILED);

	GUINEW (d.rst, GuiRptRst (), ERR_GUI_REPORTS_INIT_FAILED, "rst");
	GUIERR (d.rst, ERR_GUI_REPORTS_INIT_FAILED);

	//addTab (d.kill, "Kills");
	//addTab (d.loss, "Losses");
	//addTab (d.oob, "OOB");
	//addTab (d.rst, "Roster");

	if (!connect (this, SIGNAL (currentChanged(int)), SLOT (selectedCurrentIndex(int))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <currentChanged> to <selectedCurrentIndex>");

	set_enabled (false);

	SET_GUICLS_NOERR;
}

GuiRptTrnFrc::~GuiRptTrnFrc (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.disabled_label;
	delete d.kill;
	delete d.loss;
	delete d.oob;
	delete d.rst;
}

void
GuiRptTrnFrc::set_enabled (bool flag)
{
	bool	hidden = isHidden();

	if (d.enabled == flag) return;

	d.enabled = flag;

	if (!hidden) hide();
	if (flag) {
		removeTab (indexOf (d.disabled_label));
		addTab (d.kill, "Kills");
		addTab (d.loss, "Losses");
		addTab (d.oob, "OOB");
		addTab (d.rst, "Roster");

		setCurrentIndex (d.last_index);
		tabBar()->setTabTextColor (currentIndex(), *RES_color (RID_TAB_FG_SEL));
	} else {
		removeTab (indexOf (d.kill));
		removeTab (indexOf (d.loss));
		removeTab (indexOf (d.oob));
		removeTab (indexOf (d.rst));
		addTab (d.disabled_label, "No data available");
	}
	if (!hidden) show();
}

void
GuiRptTrnFrc::set_parent (GuiRptTrn *parent, bool pflag, bool cflag)
{
	d.kill->set_parent  (parent, pflag, cflag);
	d.loss->set_parent  (parent, pflag, cflag);
	d.oob->set_parent (parent, pflag, cflag);
	d.rst->set_parent (parent, pflag, cflag);

	d.parent = parent;
	d.pflag = pflag;
	d.cflag = cflag;
}

void
GuiRptTrnFrc::refresh (void)
{
	SPWAW_BTURN	*p;
	bool		enable = true;

	DBG_TRACE_FENTER;

	if (d.pflag && !d.cflag && d.parent && d.parent->current() && (p = d.parent->current()->data.t))
	{
		enable = (p->snap->OOBp1.support.formations.cnt != 0);
	}

	set_enabled (enable);

	if (enable) {
		d.kill->refresh();
		d.loss->refresh();
		d.oob->refresh();
		d.rst->refresh();
	}

	DBG_TRACE_FLEAVE;
}

void
GuiRptTrnFrc::selectedCurrentIndex (int index)
{
	int	i;

	for (i=0; i<count(); i++)
		tabBar()->setTabTextColor (i, *((i!=index) ? RES_color (RID_TAB_FG_DEFAULT): RES_color (RID_TAB_FG_SEL)));

	d.last_index = index;

	if (!isVisible()) return;

	refresh();
}
