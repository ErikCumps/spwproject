/** \file
 * The SPWaW war cabinet - GUI - tool bar.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_toolbar.h"

#define	TOOLBAR(t,n)												\
	do {													\
		if (!(d.t = GUI_WIN->addToolBar (n)))								\
			SET_GUICLS_ERROR (ERR_GUI_TOOLBAR_INIT_FAILED, "failed to create <" #t "> toolbar");	\
		d.t->setFont (font);										\
		d.t->setObjectName (n);										\
		d.t->setMovable (false);									\
	} while (0)

GuiToolbar::GuiToolbar (QFont &font)
	: QObject()
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Create application tool bar */
	TOOLBAR (toolbar, "Application Toolbar");

	/* Add dossier actions */
	d.toolbar->addAction (GUI_ACTIONS->p.dossier_new);
	d.toolbar->addAction (GUI_ACTIONS->p.dossier_open);
	d.toolbar->addAction (GUI_ACTIONS->p.dossier_close);
	d.toolbar->addAction (GUI_ACTIONS->p.dossier_save);
	d.toolbar->addAction (GUI_ACTIONS->p.dossier_saveAs);
	d.toolbar->addAction (GUI_ACTIONS->p.dossier_edit);
	d.toolbar->addSeparator();

	/* Add game actions */
	d.toolbar->addAction (GUI_ACTIONS->p.game_add_campaign_savegame);
#if	ALLOW_SNAPSHOTS_LOAD
	d.toolbar->addAction (GUI_ACTIONS->p.game_add_campaign_snapshot);
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	d.toolbar->addAction (GUI_ACTIONS->p.game_add_battle_savegame);
#if	ALLOW_SNAPSHOTS_LOAD
	d.toolbar->addAction (GUI_ACTIONS->p.game_add_battle_snapshot);
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	d.toolbar->addSeparator();

	/* Add navigation actions */
	d.toolbar->addAction (GUI_ACTIONS->p.nav_raise);
	d.toolbar->addAction (GUI_ACTIONS->p.nav_first);
	d.toolbar->addAction (GUI_ACTIONS->p.nav_previous);
	d.toolbar->addAction (GUI_ACTIONS->p.nav_next);
	d.toolbar->addAction (GUI_ACTIONS->p.nav_last);
	d.toolbar->addAction (GUI_ACTIONS->p.nav_lower);
	d.toolbar->addSeparator();

	/* Add application actions */
	d.toolbar->addAction (GUI_ACTIONS->p.app_prefs);
	d.toolbar->addSeparator();
	d.toolbar->addAction (GUI_ACTIONS->p.app_intel_full);
	d.toolbar->addAction (GUI_ACTIONS->p.app_intel_lmtd);
	d.toolbar->addAction (GUI_ACTIONS->p.app_intel_none);

	SET_GUICLS_NOERR;
}

GuiToolbar::~GuiToolbar (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
}

