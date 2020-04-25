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
	d.status = GUI_WIN->statusBar(); DEVASSERT (d.status != NULL);
	d.status->setFont (font);
	d.status->setSizeGripEnabled (FALSE);

	GUINEW (d.infoFrame, QFrame (d.status), ERR_GUI_STATUS_INIT_FAILED, "info frame");

	GUINEW (d.info, QLabel ("", d.infoFrame), ERR_GUI_STATUS_INIT_FAILED, "info label");
	d.info->adjustSize();
	d.status->addPermanentWidget (d.infoFrame, 1);

	GUINEW (d.selinfFrame, QFrame (d.status), ERR_GUI_STATUS_INIT_FAILED, "selinf frame");

	GUINEW (d.selinf, QLabel ("", d.selinfFrame), ERR_GUI_STATUS_INIT_FAILED, "selinf label");
	d.selinf->adjustSize();
	d.status->addPermanentWidget (d.selinfFrame, 1);

	/* Create info message timer */
	GUINEW (d.infotimer, QTimer (this), ERR_GUI_STATUS_INIT_FAILED, "info timer");

	if (!connect (d.infotimer, SIGNAL (timeout()), this, SLOT (infodone())))
		SET_GUICLS_ERROR (ERR_GUI_STATUS_INIT_FAILED, "failed to create status info timer connection");

	SET_GUICLS_NOERR;
}

GuiStatus::~GuiStatus (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.infotimer;
}

#if	!EXPERIMENTAL
void
GuiStatus::message (const QString &/*message*/, int /*ms*/)
{
}
#else	/* EXPERIMENTAL */
void
GuiStatus::message (const QString &message, int ms)
{
	printf ("[GuiStatus::message (message=\"%s\", ms=%d)]\n", qPrintable(message), ms);
	if (d.infotimer->isActive() == TRUE) d.infotimer->stop();
	d.info->setText (message);
	d.info->adjustSize();
	d.infotimer->setSingleShot (TRUE);
	d.infotimer->start (ms);
}
#endif	/* EXPERIMENTAL */

void
GuiStatus::infodone (void)
{
#if	EXPERIMENTAL
	d.info->clear ();
	d.info->adjustSize();
#endif	/* EXPERIMENTAL */
}
