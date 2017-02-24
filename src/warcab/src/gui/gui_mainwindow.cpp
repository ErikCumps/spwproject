/** \file
 * The SPWaW war cabinet - GUI - main window.
 *
 * Copyright (C) 2005-2017 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_mainwindow.h"
#include "gui_dlg_load_savegame.h"
#include "gui_dlg_load_snapshot.h"
#include "gui_dlg_load_dossier.h"
#include "gui_dlg_edit_dossier.h"
#include "gui_dlg_about.h"
#include "model/model_sanity.h"

/* Version ID used for saveState() and restoreState() */
#define	STATE_VERSION	0

GuiMainWindow::GuiMainWindow (void)
	: QMainWindow (0, Qt::Window)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* Make sure other objects can reach us during object construction! */
	GUI_WIN = this;

	setUpdatesEnabled(false);

	/* Set application caption and icon */
	caption ();
	d.main_icon = RES_pixmap (RID_MAIN_ICON);
	setWindowIcon (*d.main_icon);

	/* Set application font */
	GUINEW (d.main_font, QFont (font()), ERR_GUI_MAINWINDOW_INIT_FAILED, "font");
	d.main_font->setFamily ("Tahoma");
	setFont (*d.main_font);

	/* Create actions */
	GUINEW (d.actions, GuiActions (), ERR_GUI_MAINWINDOW_INIT_FAILED, "actions");
	GUIERR (d.actions, ERR_GUI_MAINWINDOW_INIT_FAILED);

	/* Create menubar */
	GUINEW (d.menu, GuiMainMenu (), ERR_GUI_MAINWINDOW_INIT_FAILED, "menu");
	GUIERR (d.menu, ERR_GUI_MAINWINDOW_INIT_FAILED);

	/* Create toolbar */
	GUINEW (d.toolbar, GuiToolbar (), ERR_GUI_MAINWINDOW_INIT_FAILED, "toolbar");
	GUIERR (d.toolbar, ERR_GUI_MAINWINDOW_INIT_FAILED);

	/* Create status bar */
	GUINEW (d.status, GuiStatus (), ERR_GUI_MAINWINDOW_INIT_FAILED, "status");
	GUIERR (d.status, ERR_GUI_MAINWINDOW_INIT_FAILED);

	/* Create central widget */
	GUINEW (d.body, QWidget (this), ERR_GUI_MAINWINDOW_INIT_FAILED, "body widget");
	setCentralWidget (d.body);

	/* Create body layout */
	GUINEW (d.body_layout, QVBoxLayout (d.body), ERR_GUI_MAINWINDOW_INIT_FAILED, "body layout");
	d.body_layout->setSpacing (0);
	d.body_layout->setMargin (0);

	/* Create body panes */
	GUINEW (d.body_panes, QSplitter (Qt::Horizontal, d.body), ERR_GUI_MAINWINDOW_INIT_FAILED, "body panes");
	d.body_layout->addWidget (d.body_panes);

	/* Create dossier view */
	GUINEW (d.dossier, GuiDossier (d.body_panes), ERR_GUI_MAINWINDOW_INIT_FAILED, "dossier view");
	GUIERR (d.dossier, ERR_GUI_MAINWINDOW_INIT_FAILED);

	/* Create main report view */
	GUINEW (d.report, GuiMainRpt (d.body_panes), ERR_GUI_MAINWINDOW_INIT_FAILED, "main report view");
	GUIERR (d.report, ERR_GUI_MAINWINDOW_INIT_FAILED);

	/* Final adjustments */
	d.body_panes->adjustSize();
	d.body_panes->setOpaqueResize (false);

	/* Other signal and slot connections */
	if (!connect (qApp, SIGNAL (lastWindowClosed ()), SLOT (action_app_exit ())))
		SET_GUICLS_ERROR (ERR_GUI_MAINWINDOW_INIT_FAILED, "failed to connect <action_app_exit> to lastWindowClosed");

	if (!state_restore()) action_help_reset_gui();

	/* Install the event filter for the initialization event */
	installEventFilter (this);

	setUpdatesEnabled(true);

	SET_GUICLS_NOERR;
}

GuiMainWindow::~GuiMainWindow (void)
{
	DBG_TRACE_DESTRUCT;

	DBG_log (">>>exit<<<");
	fflush (NULL);

	// QT deletes child widgets
	delete d.main_font;
	delete d.actions;
	delete d.menu;
	delete d.toolbar;
	delete d.status;

	GUI_APP->exit (20);
}

void
GuiMainWindow::caption (void)
{
	setWindowTitle (SL_APP_version_ex());
}

void
GuiMainWindow::caption (char *str)
{
	char	caption[256];

	memset (caption, 0, sizeof (caption));
	if (str)
		snprintf (caption, sizeof (caption) - 1, "%s[*] - %s", str, SL_APP_version_ex());
	else
		snprintf (caption, sizeof (caption) - 1, "%s[*]", SL_APP_version_ex());

	setWindowTitle (caption);
}

GuiActions *
GuiMainWindow::get_actions (void)
{
	return (d.actions);
}

GuiStatus *
GuiMainWindow::get_status (void)
{
	return (d.status);
}

GuiDossier *
GuiMainWindow::get_dossier (void)
{
	return (d.dossier);
}

GuiMainRpt *
GuiMainWindow::get_report (void)
{
	return (d.report);
}

bool
GuiMainWindow::state_restore (void)
{
	GUI_STATE	*state;
	QSize		desk, frame, max;
	int		dx, dy;
	bool		rc = true;

	state = CFG_gui_state_get ();
	if (!state) return (false);

	desk = GUI_APP->desktop()->availableGeometry().size();

	frame = frameSize() - size();
	max = desk;
	resize (state->size->boundedTo (max - frame));

	dx = state->size->width() + frame.width() + state->pos->x() - desk.width();
	if (dx > 0) {
		state->pos->rx() -= dx;
		if (state->pos->x() < 0) state->pos->setX (0);
	}
	dy = state->size->height() + frame.height() + state->pos->y() - desk.height();
	if (dy > 0) {
		state->pos->ry() -= dy;
		if (state->pos->y() < 0) state->pos->setY (0);
	}
	move (*(state->pos));

	rc &= d.body_panes->restoreState (*(state->state_panes));
	rc &= restoreState (*(state->state_main), STATE_VERSION);
	setWindowState (state->state);

	return (rc);
}

void
GuiMainWindow::state_save (void)
{
	GUI_STATE	state;

	memset (&state, 0, sizeof (state));

	state.state		= windowState ();
	state.size		= new QSize (size());
	state.pos		= new QPoint (pos());
	state.state_panes	= new QByteArray (d.body_panes->saveState());
	state.state_main	= new QByteArray (saveState (STATE_VERSION));

	CFG_gui_state_set (state);

	delete state.size;
	delete state.pos;
	delete state.state_panes;
	delete state.state_main;
}

void
GuiMainWindow::action_app_exit (void)
{
	DBG_log ("[%s]", __FUNCTION__);

#ifdef	_DEBUG
	SL_STDBG_report (SL_STDBG_LEVEL_DEEP);
#endif	/* _DEBUG */

	CFG_autoload_set (WARCAB->get_filename());

	action_dossier_close ();

	state_save ();

	fflush (NULL);
	DBG_log ("[%s] >>>exit<<<", __FUNCTION__);
	GUI_APP->exit (10);
}

void
GuiMainWindow::action_app_prefs (void)
{
	CFG_DLG ();
}

void
GuiMainWindow::action_dossier_new (void)
{
	SL_ERROR	erc;

	action_dossier_close();

	erc = WARCAB->mknew ();
	if (SL_HAS_ERROR (erc)) SL_ERROR_HANDLE (SL_ERR_FATAL_SOFTERR, "Failed to create new dossier!");

	if (!action_dossier_edit()) {
		WARCAB->close ();
	}
}

void
GuiMainWindow::action_dossier_open (void)
{
	SPWAW_ERROR		arc;
	SPWAW_DOSSLIST		*data = NULL;
	GuiDlgLoadDossier	*dlg;
	int			rc;
	SL_ERROR		erc;
	SL_ERROR_REQUEST	erq;

	action_dossier_close();

	arc = SPWAW_dosslist_new (&data);
	if (SPWAW_HAS_ERROR (arc)) {
		HANDLE_ERR_FUNCTION_EX (SL_ERR_FATAL_ERR, "Failed to open dossier!",
			ERR_GUI_ACTION_ERROR, "SPWAW_dosslist_new() failed: %s", SPWAW_errstr (arc), erq);
	}

	dlg = new GuiDlgLoadDossier (CFG_snap_path(), NULL);
	rc = dlg->exec ();
	dlg->data_get (data);
	delete (dlg);

	if (rc) {
		erc = WARCAB->load (data);
		SPWAW_dosslist_free (&data);

		if (SL_HAS_ERROR (erc)) {
			SL_ERROR_HANDLE (SL_ERR_FATAL_WARN, "Failed to open dossier.");
		}
	}
}

void
GuiMainWindow::action_dossier_close (void)
{
	QMessageBox::StandardButton	b;
	QString				msg;

	if (!WARCAB->is_loaded()) return;

	if (WARCAB->is_dirty()) {
		DBG_log ("[%s] dirty dossier, save?", __FUNCTION__);

		msg.sprintf ("Save changes to %s?", WARCAB->get_name());
		b = QMessageBox::question (this, SL_APP_name(), msg, QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);

		if (b == QMessageBox::Yes) {
			DBG_log ("[%s] yes", __FUNCTION__);
			action_dossier_save();
		} else {
			DBG_log ("[%s] user declined", __FUNCTION__);
		}
	}
	WARCAB->close();
}

void
GuiMainWindow::action_dossier_save (void)
{
	SL_ERROR	erc;

	if (!WARCAB->is_loaded()) return;

	erc = WARCAB->save ();
	if (erc == ERR_DOSSIER_SAVE_NONAME) {
		action_dossier_saveAs();
		return;
	}
	if (SL_HAS_ERROR (erc)) {
		SL_ERROR_HANDLE (SL_ERR_FATAL_WARN, "Failed to save dossier.");
	}
}

void
GuiMainWindow::action_dossier_saveAs (void)
{
	QString		file;
	SL_ERROR	erc;

	if (!WARCAB->is_loaded()) return;

	file = QFileDialog::getSaveFileName (this, "Save dossier as...", CFG_snap_path(), "Dossier Files (*.warcab)", 0, 0);
	if (file.isEmpty()) {
		DBG_log ("[%s] no save filename selected", __FUNCTION__);
		DBG_log ("[%s] save cancelled!", __FUNCTION__);
		return;
	}

	erc = WARCAB->saveas ((char *)qPrintable (file));
	if (SL_HAS_ERROR (erc)) {
		SL_ERROR_HANDLE (SL_ERR_FATAL_WARN, "Failed to save dossier.");
	}
}

bool
GuiMainWindow::action_dossier_edit (void)
{
	GuiDlgEditDossier	*dlg = NULL;
	int			rc;
	char			*sn, *sc;
	SL_ERROR		erc;
	bool			keep;

	if (!WARCAB->is_loaded()) return (false);

	dlg = new GuiDlgEditDossier();
	dlg->data_set (WARCAB->get_name(), WARCAB->get_comment());
	rc = dlg->exec ();
	keep = (rc == QDialog::Accepted);

	if (keep) {
		dlg->data_get (&sn, &sc);
		erc = WARCAB->edit (sn, sc);
		if (sn) free (sn);
		if (sc) free (sc);

		if (SL_HAS_ERROR (erc)) {
			SL_ERROR_HANDLE (SL_ERR_FATAL_WARN, "Failed to edit dossier.");
		}
	}

	delete dlg;

	return (keep);
}

void
GuiMainWindow::action_file_add_game (void)
{
	SPWAW_ERROR		arc;
	SPWAW_SAVELIST		*data = NULL;
	GuiDlgLoadSavegame	*dlg;
	int			rc;
	SL_ERROR		erc;
	SL_ERROR_REQUEST	erq;

	if (!WARCAB->is_loaded()) return;

	arc = SPWAW_savelist_new (&data);
	if (SPWAW_HAS_ERROR (arc)) {
		HANDLE_ERR_FUNCTION_EX (SL_ERR_FATAL_ERR, "Failed to add savegame!",
			ERR_GUI_ACTION_ERROR, "SPWAW_savelist_new() failed: %s", SPWAW_errstr (arc), erq);
	}

	dlg = new GuiDlgLoadSavegame (CFG_save_path(), WARCAB->get_gamelist());
	rc = dlg->exec ();
	dlg->data_get (data);
	delete dlg;

	if (rc) {
		erc = WARCAB->add (data);
		SPWAW_savelist_free (&data);

		if (SL_HAS_ERROR (erc)) {
			SL_ERROR_HANDLE (SL_ERR_FATAL_WARN, "Failed to add savegame.");
		}
	}
}

#if	ALLOW_SNAPSHOTS_LOAD
void
GuiMainWindow::action_file_add_snap (void)
{
	SPWAW_ERROR		arc;
	SPWAW_SNAPLIST		*data = NULL;
	GuiDlgLoadSnapshot	*dlg;
	int			rc;
	SL_ERROR		erc;
	SL_ERROR_REQUEST	erq;

	if (!WARCAB->is_loaded()) return;

	arc = SPWAW_snaplist_new (&data);
	if (SPWAW_HAS_ERROR (arc)) {
		HANDLE_ERR_FUNCTION_EX (SL_ERR_FATAL_ERR, "Failed to add snapshot!",
			ERR_GUI_ACTION_ERROR, "SPWAW_snaplist_new() failed: %s", SPWAW_errstr (arc), erq);
	}

	dlg = new GuiDlgLoadSnapshot (CFG_snap_path(), WARCAB->get_snaplist());
	rc = dlg->exec ();
	dlg->data_get (data);
	delete dlg;

	if (rc) {
		erc = WARCAB->add (data);
		SPWAW_snaplist_free (&data);

		if (SL_HAS_ERROR (erc)) {
			SL_ERROR_HANDLE (SL_ERR_FATAL_WARN, "Failed to add snapshot.");
		}
	}
}
#endif	/* ALLOW_SNAPSHOTS_LOAD */

void
GuiMainWindow::action_delete_turn (void)
{
	if (!WARCAB->is_loaded()) return;

	d.dossier->delete_item ();
}

void
GuiMainWindow::action_delete_battle (void)
{
	if (!WARCAB->is_loaded()) return;

	d.dossier->delete_item ();
}

void
GuiMainWindow::action_delete_dossier (void)
{
	if (!WARCAB->is_loaded()) return;

	action_dossier_close ();
}

void
GuiMainWindow::action_nav_first (void)
{
	if (!WARCAB->is_loaded()) return;

	d.dossier->select_first_item();
}

void
GuiMainWindow::action_nav_previous (void)
{
	if (!WARCAB->is_loaded()) return;

	d.dossier->select_prev_item();
}

void
GuiMainWindow::action_nav_next (void)
{
	if (!WARCAB->is_loaded()) return;

	d.dossier->select_next_item();
}

void
GuiMainWindow::action_nav_last (void)
{
	if (!WARCAB->is_loaded()) return;

	d.dossier->select_last_item();
}

void
GuiMainWindow::action_help_reset_gui (void)
{
	QList<int>	list;

	setWindowState (DEFAULT_GUI_STATE);
	move (DEFAULT_GUI_XPOS, DEFAULT_GUI_YPOS);
	resize (DEFAULT_GUI_WIDTH, DEFAULT_GUI_HEIGHT);
	list << DEFAULT_GUI_PANE1_WIDTH << DEFAULT_GUI_PANE2_WIDTH;
	d.body_panes->setSizes (list);
}

void
GuiMainWindow::action_help_about (void)
{
	GuiDlgAbout	*dlg = NULL;

	// TODO: check for new allocation failure?
	dlg = new GuiDlgAbout();
	dlg->exec ();
}

bool
GuiMainWindow::eventFilter (QObject *obj, QEvent *event)
{
	SL_ERROR	wrc;

	if (event->type() == GUI_EVENT_INIT) {
		DBG_log ("GUI: finalizing initial WARCAB state\n");
		wrc = WARCAB->init ();
		if (SL_HAS_ERROR (wrc)) {
			SL_ERROR_HANDLE (SL_ERR_FATAL_WARN, "Failed to load initial dossier.");
		}
		DBG_log ("GUI: ready for user\n");
		return (true);
	} else {
		// pass the event on to the parent class
		return (QMainWindow::eventFilter (obj, event));
	}
}
