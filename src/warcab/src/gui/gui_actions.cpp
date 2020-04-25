/** \file
 * The SPWaW war cabinet - GUI - actions.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_actions.h"
#include "intel_mode.h"

#define	ACTION(a,t,k,i,g)											\
	do {													\
		GUINEW (p.a, QAction (t, GUI_WIN), ERR_GUI_ACTION_INIT_FAILED, "<" #a "> action");		\
		p.a->setFont (*p.reg_font);									\
		p.a->setShortcut (k);										\
		if (g) p.a->setShortcutContext (Qt::ApplicationShortcut);					\
		if (i) p.a->setIcon (QIcon (*(RES_pixmap (i))));						\
		if (!connect (p.a, SIGNAL (triggered()), GUI_WIN, SLOT (action_##a())))				\
		SET_GUICLS_ERROR (ERR_GUI_ACTION_INIT_FAILED, "failed to create <" #a "> action connection");	\
	} while (0)

#define	CHKACTION(a,t,k,i,g)											\
	do {													\
		GUINEW (p.a, QAction (t, GUI_WIN), ERR_GUI_ACTION_INIT_FAILED, "<" #a "> action");		\
		p.a->setFont (*p.reg_font);									\
		p.a->setShortcut (k);										\
		if (g) p.a->setShortcutContext (Qt::ApplicationShortcut);					\
		if (i) p.a->setIcon (QIcon (*(RES_pixmap (i))));						\
		p.a->setCheckable (true);									\
		if (!connect (p.a, SIGNAL (triggered(bool)), GUI_WIN, SLOT (action_##a(bool))))			\
		SET_GUICLS_ERROR (ERR_GUI_ACTION_INIT_FAILED, "failed to create <" #a "> triggered connection");\
	} while (0)

GuiActions::GuiActions (QFont &font)
	: QObject()
{
	DBG_TRACE_CONSTRUCT;

	/* Initialization */
	memset (&p, 0, sizeof (p));

	/* Fonts */
	GUINEW (p.reg_font, QFont (font), ERR_GUI_ACTION_INIT_FAILED, "failed to create regular font");
	GUINEW (p.sel_font, QFont (font), ERR_GUI_ACTION_INIT_FAILED, "failed to create selection font");
	p.sel_font->setWeight (QFont::Bold);

	/* NOTE: icon appearance on menu is dictated by style!
	 * We can't control it here... */

	/* Application exit action */
	ACTION (app_exit, "Exit", Qt::ALT+Qt::Key_F4, 0, true);
	p.app_exit->setShortcutContext (Qt::ApplicationShortcut);

	/* Application preferences action */
	ACTION (app_prefs, "Preferences...", 0, RID_GUI_ICONS_1_Preferences, true);

	/* Application intel actions group */
	GUINEW (p.app_intel, QActionGroup(this), ERR_GUI_ACTION_INIT_FAILED, "failed to create <app_intel> action group");
	QString intelstr("Intel mode: ");

	/* Application full intel action */
	CHKACTION (app_intel_full, intelstr + QString(intelmode2str(INTEL_MODE_FULL)), 0, RID_ICON_INTEL_FULL, true);

	/* Application limited intel action */
	CHKACTION (app_intel_lmtd, intelstr + QString(intelmode2str(INTEL_MODE_LMTD)), 0, RID_ICON_INTEL_LMTD, true);

	/* Application no intel action */
	CHKACTION (app_intel_none, intelstr + QString(intelmode2str(INTEL_MODE_NONE)), 0, RID_ICON_INTEL_NONE, true);

	/* Populate application intel actions group */
	p.app_intel->addAction(p.app_intel_full);
	p.app_intel->addAction(p.app_intel_lmtd);
	p.app_intel->addAction(p.app_intel_none);

	/* Dossier new action */
	ACTION (dossier_new, "New dossier...", Qt::CTRL+Qt::Key_N, RID_GUI_ICONS_1_New, true);

	/* Dossier open action */
	ACTION (dossier_open, "Open dossier...", Qt::CTRL+Qt::Key_O, RID_GUI_ICONS_1_Open, true);

	/* Dossier close action */
	ACTION (dossier_close, "Close dossier", Qt::CTRL+Qt::Key_F4, 0, true);

	/* Dossier save action */
	ACTION (dossier_save, "Save dossier", Qt::CTRL+Qt::Key_S, RID_GUI_ICONS_1_Save, true);

	/* Dossier saveAs action */
	ACTION (dossier_saveAs, "Save dossier as...", 0, 0, true);

	/* Dossier edit action */
	ACTION (dossier_edit, "Edit dossier...", 0, RID_EDIT_DOSSIER, true);

	/* Game|Add campaign savegame action */
	ACTION (game_add_campaign_savegame, "Add savegame to campaign...", Qt::CTRL+Qt::Key_A, RID_GUI_ICON_ADD_CAMPAIGN_SAVEGAME, true);

	/* Game|Add standalone battle savegame action */
	ACTION (game_add_battle_savegame, "Add standalone battle from savegame...", Qt::CTRL+Qt::Key_B, RID_GUI_ICON_ADD_BATTLE_SAVEGAME, true);

#if	ALLOW_SNAPSHOTS_LOAD
	/* Game|Add campaign snapshot action */
	ACTION (game_add_campaign_snapshot, "Add snapshot to campaign...", Qt::CTRL+Qt::ALT+Qt::Key_A, RID_GUI_ICON_ADD_CAMPAIGN_SNAPSHOT, true);

	/* Game|Add standalone battle snapshot action */
	ACTION (game_add_battle_snapshot, "Add standalone battle from snapshot...", Qt::CTRL+Qt::ALT+Qt::Key_B, RID_GUI_ICON_ADD_BATTLE_SNAPSHOT, true);
#endif	/* ALLOW_SNAPSHOTS_LOAD */

	/* Context menu actions */
	/* Add standalone battle savegame action */
	ACTION (add_battle_savegame, "Add savegame to this standalone battle", Qt::SHIFT+Qt::Key_A, RID_GUI_ICON_ADD_BATTLE_SAVEGAME, false);
#if	ALLOW_SNAPSHOTS_LOAD
	/* Add standalone battle snapshot action */
	ACTION (add_battle_snapshot, "Add snapshot to this standalone battle", Qt::SHIFT+Qt::ALT+Qt::Key_A, RID_GUI_ICON_ADD_BATTLE_SNAPSHOT, false);
#endif	/* ALLOW_SNAPSHOTS_LOAD */

	ACTION (delete_turn, "Delete battle turn", Qt::CTRL+Qt::Key_D, RID_GUI_ICONS_1_Delete, false);
	ACTION (delete_battle, "Delete battle", 0, RID_GUI_ICONS_1_Delete, false);
	ACTION (delete_dossier, "Delete dossier", 0, RID_GUI_ICONS_1_Delete, false);

	/* Navigation actions */
	ACTION (nav_raise, "Parent record", Qt::CTRL+Qt::SHIFT+Qt::Key_PageUp, RID_GUI_ICON_RAISE, true);
	ACTION (nav_first, "First record", Qt::CTRL+Qt::Key_Home, RID_GUI_ICON_FIRST, true);
	ACTION (nav_previous, "Previous record", Qt::CTRL+Qt::Key_PageUp, RID_GUI_ICON_LEFT, true);
	ACTION (nav_next, "Next record", Qt::CTRL+Qt::Key_PageDown, RID_GUI_ICON_RIGHT, true);
	ACTION (nav_last, "Last record", Qt::CTRL+Qt::Key_End, RID_GUI_ICON_LAST, true);
	ACTION (nav_lower, "Child record", Qt::CTRL+Qt::SHIFT+Qt::Key_PageDown, RID_GUI_ICON_LOWER, true);

	/* Help actions */
	ACTION (help_reset_gui, "Reset GUI", 0, RID_XPM_TEST, true);
	ACTION (help_about, "About Warcab...", 0, RID_GUI_ICON_INFO, true);

	/* Set initial enabled/disabled states */
	enable_dossier_actions (false, SPWAW_EMPTY_DOSSIER);

	SET_GUICLS_NOERR;
}

GuiActions::~GuiActions (void)
{
	DBG_TRACE_DESTRUCT;

	delete p.app_exit;
	delete p.app_prefs;
	delete p.app_intel_full;
	delete p.app_intel_lmtd;
	delete p.app_intel_none;

	delete p.dossier_new;
	delete p.dossier_open;
	delete p.dossier_close;
	delete p.dossier_save;
	delete p.dossier_saveAs;
	delete p.dossier_edit;

	delete p.game_add_campaign_savegame;
	delete p.game_add_battle_savegame;
#if	ALLOW_SNAPSHOTS_LOAD
	delete p.game_add_campaign_snapshot;
	delete p.game_add_battle_snapshot;
#endif	/* ALLOW_SNAPSHOTS_LOAD */

	delete p.add_battle_savegame;
#if	ALLOW_SNAPSHOTS_LOAD
	delete p.add_battle_snapshot;
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	delete p.delete_turn;
	delete p.delete_battle;
	delete p.delete_dossier;

	delete p.nav_first;
	delete p.nav_previous;
	delete p.nav_next;
	delete p.nav_last;
}

void
GuiActions::enable_dossier_actions (bool b, SPWAW_DOSSIER_TYPE t)
{
	p.app_exit->setEnabled (true);
	p.app_prefs->setEnabled (true);
	p.app_intel_full->setEnabled (true);
	p.app_intel_lmtd->setEnabled (true);
	p.app_intel_none->setEnabled (true);

	p.dossier_close->setEnabled (b);
	p.dossier_save->setEnabled (b);
	p.dossier_saveAs->setEnabled (b);
	p.dossier_edit->setEnabled (b);

	p.game_add_campaign_savegame->setEnabled (b && t != SPWAW_STDALONE_DOSSIER);
	p.game_add_battle_savegame->setEnabled (b && t != SPWAW_CAMPAIGN_DOSSIER);
#if	ALLOW_SNAPSHOTS_LOAD
	p.game_add_campaign_snapshot->setEnabled (b && t != SPWAW_STDALONE_DOSSIER);
	p.game_add_battle_snapshot->setEnabled (b && t != SPWAW_CAMPAIGN_DOSSIER);
#endif	/* ALLOW_SNAPSHOTS_LOAD */

	p.add_battle_savegame->setEnabled (b && t != SPWAW_CAMPAIGN_DOSSIER);
#if	ALLOW_SNAPSHOTS_LOAD
	p.add_battle_snapshot->setEnabled (b && t != SPWAW_CAMPAIGN_DOSSIER);
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	p.delete_turn->setEnabled (b);
	p.delete_battle->setEnabled (b);
	p.delete_dossier->setEnabled (b);

	p.nav_raise->setEnabled (b);
	p.nav_first->setEnabled (b);
	p.nav_previous->setEnabled (b);
	p.nav_next->setEnabled (b);
	p.nav_last->setEnabled (b);
	p.nav_lower->setEnabled (b);
}

void
GuiActions::select_intel_action (INTEL_MODE mode)
{
	QAction*actns[INTEL_MODE_CNT] = { p.app_intel_full, p.app_intel_lmtd, p.app_intel_none };
	QFont*	fonts[INTEL_MODE_CNT] = { p.reg_font,       p.reg_font,       p.reg_font       };
	bool	check[INTEL_MODE_CNT] = { false,            false,            false            };

	fonts[mode] = p.sel_font;
	check[mode] = true;

	for (int i=0; i<INTEL_MODE_CNT; i++) {
		actns[i]->setChecked(check[i]);
		actns[i]->setFont (*fonts[i]);
	}
}