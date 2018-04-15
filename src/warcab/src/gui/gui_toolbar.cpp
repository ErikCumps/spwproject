/** \file
 * The SPWaW war cabinet - GUI - tool bar.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_toolbar.h"

#define	TOOLBAR(t,n)												\
	do {													\
		if (!(d.t = GUI_WIN->addToolBar (n)))								\
			SET_GUICLS_ERROR (ERR_GUI_TOOLBAR_INIT_FAILED, "failed to create <" #t "> toolbar");	\
		d.t->setObjectName (n);										\
		d.t->setMovable (false);									\
	} while (0)

GuiToolbar::GuiToolbar (void)
	: QObject()
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Create dossier tool bar */
	TOOLBAR (dossier, "Dossier Toolbar");

	d.dossier->addAction (GUI_ACTIONS->p.dossier_new);
	d.dossier->addAction (GUI_ACTIONS->p.dossier_open);
	d.dossier->addAction (GUI_ACTIONS->p.dossier_close);
	d.dossier->addAction (GUI_ACTIONS->p.dossier_save);
	d.dossier->addAction (GUI_ACTIONS->p.dossier_saveAs);
	d.dossier->addAction (GUI_ACTIONS->p.dossier_edit);

	/* Create file tool bar */
	TOOLBAR (file, "File Toolbar");

	d.file->addAction (GUI_ACTIONS->p.file_add_game);
#if	ALLOW_SNAPSHOTS_LOAD
	d.file->addAction (GUI_ACTIONS->p.file_add_snap);
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	d.file->addAction (GUI_ACTIONS->p.app_prefs);

	/* Create navigation tool bar */
	TOOLBAR (nav, "Navigation Toolbar");

	d.nav->addAction (GUI_ACTIONS->p.nav_raise);
	d.nav->addAction (GUI_ACTIONS->p.nav_first);
	d.nav->addAction (GUI_ACTIONS->p.nav_previous);
	d.nav->addAction (GUI_ACTIONS->p.nav_next);
	d.nav->addAction (GUI_ACTIONS->p.nav_last);
	d.nav->addAction (GUI_ACTIONS->p.nav_lower);

	SET_GUICLS_NOERR;
}

GuiToolbar::~GuiToolbar (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
}

