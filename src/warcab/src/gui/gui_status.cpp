/** \file
 * The SPWaW war cabinet - GUI - status bar.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_status.h"

GuiStatus::GuiStatus (QFont &font)
	: QObject()
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Create status bar */
	d.bar = GUI_WIN->statusBar(); DEVASSERT (d.bar != NULL);
	d.bar->setFont (font);
	d.bar->setSizeGripEnabled (FALSE);

	GUINEW (d.infoFrame, QFrame (d.bar), ERR_GUI_STATUS_INIT_FAILED, "info frame");

	GUINEW (d.info, QLabel ("", d.infoFrame), ERR_GUI_STATUS_INIT_FAILED, "info label");
	d.info->adjustSize();
	d.bar->addPermanentWidget (d.infoFrame, 1);

	/* Create info message timer */
	GUINEW (d.msgtimer, QTimer (this), ERR_GUI_STATUS_INIT_FAILED, "message timer");

	if (!connect (d.msgtimer, SIGNAL (timeout()), this, SLOT (msgdone())))
		SET_GUICLS_ERROR (ERR_GUI_STATUS_INIT_FAILED, "failed to create status message timer connection");

	/* Clear status */
	d.status = new QString ();
	if (!d.status) SET_GUICLS_ERROR (ERR_GUI_STATUS_INIT_FAILED, "failed to create initial status");

	SET_GUICLS_NOERR;
}

GuiStatus::~GuiStatus (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.msgtimer;
	delete d.status;
}

void
GuiStatus::status (const QString &status)
{
	DBG_log ("[%s] status=\"%s\"\n", __FUNCTION__, qPrintable(status));

	*(d.status) = status;
	show_status();
}

void
GuiStatus::message (const QString &message, int ms)
{
	DBG_log ("[%s] message=\"%s\", ms=%d\n", __FUNCTION__, qPrintable(message), ms);

	if (d.msgtimer->isActive() == TRUE) d.msgtimer->stop();
	d.info->setText (message);
	d.info->adjustSize();
	d.msgtimer->setSingleShot (TRUE);
	d.msgtimer->start (ms);
}

void
GuiStatus::clear (void)
{
	d.status->clear();
	show_status();
}


void
GuiStatus::msgdone (void)
{
	show_status();
}

void
GuiStatus::show_status (void)
{
	// A message is being shown, postpone the show_status
	if (d.msgtimer->isActive() == TRUE) return;

	if (d.status->isEmpty()) {
		d.info->clear ();
	} else {
		d.info->setText (*d.status);
	}
	d.info->adjustSize();
}