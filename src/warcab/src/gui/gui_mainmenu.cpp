/** \file
 * The SPWaW war cabinet - GUI - main menu.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_mainmenu.h"

GuiMainMenu::GuiMainMenu (void)
	: QObject()
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Create menu bar */
	d.menu = GUI_WIN->menuBar (); DEVASSERT (d.menu != NULL);

	/* Create and hook dossier menu */
	GUINEW (d.dossier, QMenu ("Dossier", GUI_WIN), ERR_GUI_MAINMENU_INIT_FAILED, "<dossier> menu");

	d.dossier->addAction (GUI_ACTIONS->p.dossier_new);
	d.dossier->addAction (GUI_ACTIONS->p.dossier_open);
	d.dossier->addAction (GUI_ACTIONS->p.dossier_close);
	d.dossier->addSeparator ();
	d.dossier->addAction (GUI_ACTIONS->p.dossier_save);
	d.dossier->addAction (GUI_ACTIONS->p.dossier_saveAs);
	d.dossier->addSeparator ();
	d.dossier->addAction (GUI_ACTIONS->p.dossier_edit);
	d.dossier->addSeparator ();
	d.dossier->addAction (GUI_ACTIONS->p.app_prefs);
	d.dossier->addSeparator ();
	d.dossier->addAction (GUI_ACTIONS->p.app_exit);

	d.menu->addMenu (d.dossier);

	/* Create and hook file menu */
	GUINEW (d.file, QMenu ("File", GUI_WIN), ERR_GUI_MAINMENU_INIT_FAILED, "<file> menu");

	d.file->addAction (GUI_ACTIONS->p.file_add_campaign_savegame);
#if	ALLOW_SNAPSHOTS_LOAD
	d.file->addAction (GUI_ACTIONS->p.file_add_campaign_snapshot);
#endif	/* ALLOW_SNAPSHOTS_LOAD */

	d.file->addAction (GUI_ACTIONS->p.file_add_battle_savegame);
#if	ALLOW_SNAPSHOTS_LOAD
	d.file->addAction (GUI_ACTIONS->p.file_add_battle_snapshot);
#endif	/* ALLOW_SNAPSHOTS_LOAD */

	d.menu->addMenu (d.file);

	/* Create and hook navigation menu */
	GUINEW (d.nav, QMenu ("Navigation", GUI_WIN), ERR_GUI_MAINMENU_INIT_FAILED, "<nav> menu");

	d.nav->addAction (GUI_ACTIONS->p.nav_raise);
	d.nav->addAction (GUI_ACTIONS->p.nav_first);
	d.nav->addAction (GUI_ACTIONS->p.nav_previous);
	d.nav->addAction (GUI_ACTIONS->p.nav_next);
	d.nav->addAction (GUI_ACTIONS->p.nav_last);
	d.nav->addAction (GUI_ACTIONS->p.nav_lower);

	d.menu->addMenu (d.nav);

	/* Create and hook help menu */
	GUINEW (d.help, QMenu ("Help", GUI_WIN), ERR_GUI_MAINMENU_INIT_FAILED, "<help> menu");

	d.help->addAction (GUI_ACTIONS->p.help_reset_gui);
	d.help->addAction (GUI_ACTIONS->p.help_about);

	d.menu->addMenu (d.help);

	/* Menu status connections */
	if (!connect (d.dossier, SIGNAL (hovered(QAction *)), SLOT (MenuStatus(QAction *))))
		SET_GUICLS_ERROR (ERR_GUI_MAINMENU_INIT_FAILED, "failed to create <dossier> menu status connection");
	if (!connect (d.file, SIGNAL (hovered(QAction *)), SLOT (MenuStatus(QAction *))))
		SET_GUICLS_ERROR (ERR_GUI_MAINMENU_INIT_FAILED, "failed to create <file> menu status connection");
	if (!connect (d.nav, SIGNAL (hovered(QAction *)), SLOT (MenuStatus(QAction *))))
		SET_GUICLS_ERROR (ERR_GUI_MAINMENU_INIT_FAILED, "failed to create <nav> menu status connection");
	if (!connect (d.help, SIGNAL (hovered(QAction *)), SLOT (MenuStatus(QAction *))))
		SET_GUICLS_ERROR (ERR_GUI_MAINMENU_INIT_FAILED, "failed to create <help> menu status connection");

	SET_GUICLS_NOERR;
}

GuiMainMenu::~GuiMainMenu (void)
{
	DBG_TRACE_DESTRUCT;

	// QT does not delete our child widgets
	delete d.dossier;
	delete d.file;
	delete d.nav;
	delete d.help;
}

void
GuiMainMenu::MenuStatus (QAction *action)
{
	QString name, msg;

	printf ("[GuiMainMenu::MenuStatus(0x%8.8x)]\n", (unsigned int)action);

#if	!EXPERIMENTAL
	name = action->text ();
	msg.sprintf ("Menu id 0x%8.8x = <%s>", (void *)action, qPrintable (name));
	GUI_WIN->get_status()->message (msg, 1000);
#endif	/* !EXPERIMENTAL */
}
