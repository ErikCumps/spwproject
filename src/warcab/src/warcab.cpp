/** \file
 * The SPWaW war cabinet - application state handling.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "common.h"
#include "gui/gui_private.h"
#include "warcab.h"
#include "tracking.h"

WARCABState	*WARCAB;

/* --- forward declarations --- */

static void	statereport	(SL_STDBG_INFO_LEVEL level);


/* --- private variables --- */

/*! Module name */
static const char	*MODULE = "WARCAB";

/*! Initialization status flag */
static SL_BOOL		initialized = SL_false;


/* --- code --- */

SL_ERROR
WARCAB_init (SL_APP_INFO *info)
{
	if (initialized) RETURN_OK;
	SL_STDBG_add (statereport, MODULE);

	WARCAB = new WARCABState (info);

	atexit (WARCAB_shutdown);

	initialized = SL_true;

	RETURN_OK;
}

void
WARCAB_shutdown (void)
{
	WARCABState	*p;

	if (!initialized) return;
	SL_STDBG_delete (statereport);

	p = WARCAB; WARCAB = NULL; delete p;

	initialized = SL_false;
}

WARCABState::WARCABState (SL_APP_INFO *info)
{
	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.tracking = new WARCABTracking (this);

	/* Reload last dossier? */
	d.options.load = CFG_autoload_get ();

	/* Filename specified on commandline overrides autoload! */
	if (info->context.argc > 1) {
		d.options.load = info->context.argv[1];
	}
}

WARCABState::~WARCABState ()
{
	delete d.tracking;

	close();
}

SL_ERROR
WARCABState::init (void)
{
	SPWAW_ERROR		arc;
	SPWAW_DOSSLIST		*list = NULL;
	SPWAW_DOSSLIST_NODE	node;
	SL_ERROR		rc = SL_ERR_NO_ERROR;

	if (d.options.load) {
		arc = SPWAW_dosslist_new (&list);
		if (SPWAW_HAS_ERROR (arc)) {
			RETURN_ERR_FUNCTION_EX1 (ERR_DOSSIER_INITLOAD_FAILED, "SPWAW_dosslist_new() failed: %s", SPWAW_errstr (arc));
		}

		memset (&node, 0, sizeof (node));
		snprintf (node.filepath, sizeof (node.filepath) - 1, "%s", d.options.load);

		arc = SPWAW_dosslist_addcpy (list, &node);
		if (!SPWAW_HAS_ERROR (arc)) {
			rc = load (list);
		}
		SPWAW_dosslist_free (&list);

		if (SPWAW_HAS_ERROR (arc)) {
			RETURN_ERR_FUNCTION_EX1 (ERR_DOSSIER_INITLOAD_FAILED, "SPWAW_dosslist_addcpy() failed: %s", SPWAW_errstr (arc));
		}

		if (rc == ERR_DOSSIER_NO_FILE) {
			// The dossier file was not found. Ignore this error.
			rc = SL_ERR_NO_ERROR;
		}

		SL_ROE (rc);
	}

	RETURN_OK;
}

SL_ERROR
WARCABState::mknew (SPWAW_GAME_TYPE gametype, const char *name, const char *comment)
{
	SPWAW_ERROR	arc;
	SL_ERROR	rc;

	close();

	arc = SPWAW_dossier_new (gametype, CFG_oob_path(gametype), name, comment, &d.dossier);

	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION_EX1 (ERR_DOSSIER_NEW_FAILED, "SPWAW_dossier_new() failed: %s", SPWAW_errstr (arc));
	}

	rc = refresh_savelists ();
	if (SL_HAS_ERROR (rc)) {
		RETURN_ERR_FUNCTION (ERR_DOSSIER_NEW_FAILED, "WARCAB_refresh_savelists() failed");
	}

	set_dirty (true);
	set_name (d.dossier->name);
	setup_tree ();
	emit was_loaded (d.tree);

	RETURN_OK;
}

SL_ERROR
WARCABState::load (SPWAW_DOSSLIST *list)
{
	SPWAW_ERROR	arc;
	SL_ERROR	rc;
	GuiProgress	gp ("Loading dossier...", 0);

	if (!list || !list->cnt) return (SPWERR_NULLARG);

	DBG_log ("[%s] starting progress", __FUNCTION__);
	gp.setRange (0, 4);

	close();
	gp.inc();

	arc = SPWAW_dossier_load (list->list[0]->filepath, &d.dossier);
	if (SPWAW_HAS_ERROR (arc)) {
		switch (arc) {
			case SPWERR_INCOMPATIBLE:
				rc = ERR_DOSSIER_INCOMPATIBLE;
				break;
			case SPWERR_CORRUPT:
				rc = ERR_DOSSIER_CORRUPT;
				break;
			case SPWERR_FOFAILED:
				rc = ERR_DOSSIER_NO_FILE;
				break;
			default:
				rc = ERR_DOSSIER_LOAD_FAILED;
				break;
		}
		RETURN_ERR_FUNCTION_EX1 (rc, "SPWAW_dossier_load() failed: %s", SPWAW_errstr (arc));
	}
	gp.inc();

	snprintf (d.filename, sizeof (d.filename) - 1, "%s", list->list[0]->filepath);

	rc = refresh_savelists ();
	if (SL_HAS_ERROR (rc)) {
		RETURN_ERR_FUNCTION (ERR_DOSSIER_LOAD_FAILED, "WARCAB_refresh_savelists() failed");
	}
	gp.inc();

	set_dirty (false);
	set_name (d.dossier->name);
	setup_tree ();
	GUI_FIXME;
	emit was_loaded (d.tree);
	GUI_FIXME;

	DBG_log ("[%s] finishing progress, calling done()", __FUNCTION__);
	GUI_FIXME;
	gp.done();
	DBG_log ("[%s] finishing progress, done()", __FUNCTION__);
	RETURN_OK;
}

SL_ERROR
WARCABState::save (void)
{
	SPWAW_ERROR	arc;
	GuiProgress	gp ("Saving dossier...", 0);

	if (!is_loaded()) RETURN_OK;

	gp.setRange (0, 1);
	GUI_FIXME;

	if (d.filename[0] == '\0') {
		RETURN_ERR_FUNCTION (ERR_DOSSIER_SAVE_NONAME, "empty filename");
	}

	arc = SPWAW_dossier_save (&d.dossier, d.filename, CFG_compress());
	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION_EX1 (ERR_DOSSIER_SAVE_FAILED, "SPWAW_dossier_save() failed: %s", SPWAW_errstr (arc));
	}

	gp.inc();
	GUI_FIXME;

	set_dirty (false);

	gp.done();
	RETURN_OK;
}

SL_ERROR
WARCABState::saveas (char *file)
{
	SPWAW_ERROR	arc;
	GuiProgress	gp ("Saving dossier...", 0);

	if (!file) {
		RETURN_ERR_FUNCTION_EX0 (ERR_DOSSIER_SAVE_FAILED, "NULL filename argument");
	}

	if (!is_loaded()) RETURN_OK;

	gp.setRange (0, 1);
	GUI_FIXME;

	arc = SPWAW_dossier_save (&d.dossier, file, CFG_compress());
	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION_EX1 (ERR_DOSSIER_SAVE_FAILED, "SPWAW_dossier_save() failed: %s", SPWAW_errstr (arc));
	}

	gp.inc();
	GUI_FIXME;

	memset (d.filename, 0, sizeof (d.filename));
	snprintf (d.filename, sizeof (d.filename) - 1, "%s", file);

	set_dirty (false);

	gp.done();
	RETURN_OK;
}

SL_ERROR
WARCABState::close (void)
{
	if (!is_loaded()) RETURN_OK;

	if (d.dirty) DBG_log ("[%s] Oh oh, dirty dossier: changes were lost!", __FUNCTION__);

	emit will_close ();

	memset (d.filename, 0, sizeof (d.filename));
	set_dirty (false);
	set_name (NULL);
	free_tree();

	SPWAW_savelist_free (&d.gamelist);
#if	ALLOW_SNAPSHOTS_LOAD
	SPWAW_snaplist_free (&d.snaplist);
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	SPWAW_dossier_free (&d.dossier);

	emit was_closed ();

	RETURN_OK;
}

typedef QMap<QString, QString>			FAILMAP;
typedef QMap<QString, QString>::const_iterator	FAILMAP_it;

SL_ERROR
WARCABState::process_list (PL_LIST &list, PL_ADD add, void *context, GuiProgress &gp)
{
	unsigned long	cnt, i, done;
	SPWAW_BTURN	*added = NULL;
	FAILMAP		failures;

	SL_CWENULLARG (context);

	cnt = list.savelist ? list.list.save->cnt : list.list.snap->cnt;
	for (i=done=0; i<cnt; i++) {
		SPWAW_ERROR	arc;
		SPWAW_SNAPSHOT	*s;
		SPWAW_BTURN	*t;

		if (list.savelist) {
			arc = SPWAW_snap_make (&(list.list.save->list[i]->sgd), &s);
		} else {
			arc = SPWAW_snap_load (list.list.snap->list[i]->filepath, &s);
		}
		gp.inc ();
		if (SPWAW_HAS_ERROR (arc)) {
			if (list.savelist) {
				failures[list.list.save->list[i]->filename] = SPWAW_errstr (arc);
			} else {
				failures[list.list.snap->list[i]->filepath] = SPWAW_errstr (arc);
			}
			gp.inc ();
			continue;
		}

		arc = add (context, s, &t);
		gp.inc ();
		if (SPWAW_HAS_ERROR (arc)) {
			if (list.savelist) {
				failures[list.list.save->list[i]->filename] = SPWAW_errstr (arc);
			} else {
				failures[list.list.snap->list[i]->filepath] = SPWAW_errstr (arc);
			}
			SPWAW_snap_free (&s);
			continue;
		} else {
			if (!added && t) added = t;
		}

		done++;
	}

	if (done) {
		set_dirty (true);

		refresh_tree();
		if (added) {
			MDLD_TREE_ITEM *item = item_from_turn (added); DEVASSERT (item != NULL);
			emit was_added (item);
		}

		SL_ERROR rc = refresh_savelists ();
		if (SL_HAS_ERROR (rc)) {
			RETURN_ERR_FUNCTION (ERR_DOSSIER_ADD_SAVE_FAILED, "WARCAB_refresh_savelists() failed!");
		}
	}
	gp.inc ();

	if (!failures.empty())
	{
		char		buf[16384];
		UtilStrbuf	str(buf, sizeof (buf), true, true);
		char		*title = NULL;

		if (list.savelist) {
			title = (failures.count() == 1) ? "Failed to add savegame" : "Failed to add some savegames";
		} else {
			title = (failures.count() == 1) ? "Failed to add snapshot" : "Failed to add some snapshots";
		}

		for (FAILMAP_it i = failures.constBegin(); i != failures.constEnd(); ++i)
			str.printf ("%s: %s\n", qPrintable(i.key()), qPrintable(i.value()));

		if (list.savelist) {
			GUI_errorbox (SL_ERR_FATAL_WARN, title, "Savegame", buf);
		}else {
			GUI_errorbox (SL_ERR_FATAL_WARN, title, "Snapshot", buf);
		}
	}

	gp.done ();
	RETURN_OK;
}

static SPWAW_ERROR
add_to_campaign (void *target, SPWAW_SNAPSHOT *snap, SPWAW_BTURN **bturn)
{
	return (SPWAW_dossier_add_campaign_snap ((SPWAW_DOSSIER *)target, snap, bturn));
}

static SPWAW_ERROR
add_to_standalone (void *target, SPWAW_SNAPSHOT *snap, SPWAW_BTURN **bturn)
{
	return (SPWAW_dossier_add_battle_snap ((SPWAW_BATTLE *)target, snap, bturn));
}

SL_ERROR
WARCABState::add_campaign (SPWAW_SAVELIST *list)
{
	SL_ERROR	rc;
	PL_LIST		pl;

	if (!list) {
		RETURN_ERR_FUNCTION_EX0 (ERR_DOSSIER_ADD_SAVE_FAILED, "NULL savelist argument");
	}

	GuiProgress gp ("Adding savegame(s) to campaign...", 0);
	gp.setRange (0, (2*list->cnt) + 1);

	pl.list.save = list;
	pl.savelist = true;
	pl.gametype = d.dossier->gametype;
	rc = process_list (pl, add_to_campaign, d.dossier, gp); SL_ROE(rc);

	RETURN_OK;
}

SPWAW_ERROR
WARCABState::basic_campaign_add (SPWAW_SAVEGAME_DESCRIPTOR *sgd)
{
	SPWAW_ERROR	rc;
	SPWAW_SNAPSHOT	*s = NULL;
	SPWAW_BTURN	*t = NULL;

	if (!sgd) {
		rc = SPWERR_NULLARG;
		goto handle_error;
	}

	rc = SPWAW_snap_make (sgd, &s);
	if (SPWAW_HAS_ERROR (rc)) {
		goto handle_error;
	}

	rc = add_to_campaign (d.dossier, s, &t);
	if (SPWAW_HAS_ERROR (rc)) {
		SPWAW_snap_free (&s);
	} else {
		set_dirty (true);

		refresh_tree();
		if (t) {
			MDLD_TREE_ITEM *item = item_from_turn (t); DEVASSERT (item != NULL);
			MDLD_TREE_update_seqnums (item);
			emit was_added (item);
		}

		SPWAW_savelist_free (&d.gamelist);
		rc = SPWAW_dossier_savelist (d.dossier, &d.gamelist);
	}

handle_error:
	return (rc);
}

SL_ERROR
WARCABState::add_stdalone (char *name, SPWAW_SAVELIST *list)
{
	SL_ERROR	rc;
	SPWAW_ERROR	arc;
	SPWAW_SNAPSHOT	*s;
	SPWAW_BATTLE	*b;
	PL_LIST		pl;

	if (!list) {
		RETURN_ERR_FUNCTION_EX0 (ERR_DOSSIER_ADD_SAVE_FAILED, "NULL savelist argument");
	}

	GuiProgress gp ("Adding standalone battle and savegame(s)...", 0);
	gp.setRange (0, (2*list->cnt) + 3);

	arc = SPWAW_snap_make (&(list->list[0]->sgd), &s);
	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION (ERR_DOSSIER_ADD_SAVE_FAILED, "initial SPWAW_snap_make() failed!");
	}
	gp.inc ();

	arc = SPWAW_dossier_add_battle (d.dossier, s, name, &b);
	SPWAW_snap_free (&s);
	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION (ERR_DOSSIER_ADD_SAVE_FAILED, "SPWAW_dossier_add_battle() failed!");
	}
	gp.inc ();

	pl.list.save = list;
	pl.savelist = true;
	pl.gametype = b->dossier->gametype;
	rc = process_list (pl, add_to_standalone, b, gp); SL_ROE(rc);

	RETURN_OK;
}

SL_ERROR
WARCABState::add_stdalone (SPWAW_BATTLE *battle, SPWAW_SAVELIST *list)
{
	SL_ERROR	rc;
	PL_LIST		pl;

	if (!battle) {
		RETURN_ERR_FUNCTION_EX0 (ERR_DOSSIER_ADD_SAVE_FAILED, "NULL battle argument");
	}
	if (!list) {
		RETURN_ERR_FUNCTION_EX0 (ERR_DOSSIER_ADD_SAVE_FAILED, "NULL savelist argument");
	}

	GuiProgress gp ("Adding savegame(s) to standalone battle...", 0);
	gp.setRange (0, (2*list->cnt) + 1);

	pl.list.save = list;
	pl.savelist = true;
	pl.gametype = battle->dossier->gametype;
	rc = process_list (pl, add_to_standalone, battle, gp); SL_ROE(rc);

	RETURN_OK;
}

#if	ALLOW_SNAPSHOTS_LOAD
SL_ERROR
WARCABState::add_campaign (SPWAW_SNAPLIST *list)
{
	SL_ERROR	rc;
	PL_LIST		pl;

	if (!list) {
		RETURN_ERR_FUNCTION_EX0 (ERR_DOSSIER_ADD_SAVE_FAILED, "NULL snaplist argument");
	}

	GuiProgress gp ("Adding snapshot(s) to campaign...", 0);
	gp.setRange (0, (2*list->cnt) + 1);

	pl.list.snap = list;
	pl.savelist = false;
	pl.gametype = d.dossier->gametype;
	rc = process_list (pl, add_to_campaign, d.dossier, gp); SL_ROE(rc);

	RETURN_OK;
}

SL_ERROR
WARCABState::add_stdalone (char *name, SPWAW_SNAPLIST *list)
{
	SL_ERROR	rc;
	SPWAW_ERROR	arc;
	SPWAW_SNAPSHOT	*s;
	SPWAW_BATTLE	*b;
	PL_LIST		pl;

	if (!list) {
		RETURN_ERR_FUNCTION_EX0 (ERR_DOSSIER_ADD_SAVE_FAILED, "NULL snaplist argument");
	}

	GuiProgress gp ("Adding standalone battle and snapshot(s)...", 0);
	gp.setRange (0, (2*list->cnt) + 3);

	arc = SPWAW_snap_load (list->list[0]->filepath, &s);
	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION (ERR_DOSSIER_ADD_SAVE_FAILED, "initial SPWAW_snap_load() failed!");
	}
	gp.inc ();

	arc = SPWAW_dossier_add_battle (d.dossier, s, name, &b);
	SPWAW_snap_free (&s);
	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION (ERR_DOSSIER_ADD_SAVE_FAILED, "SPWAW_dossier_add_battle() failed!");
	}
	gp.inc ();

	pl.list.snap = list;
	pl.savelist = false;
	pl.gametype = b->dossier->gametype;
	rc = process_list (pl, add_to_standalone, b, gp); SL_ROE(rc);

	RETURN_OK;
}

SL_ERROR
WARCABState::add_stdalone (SPWAW_BATTLE *battle, SPWAW_SNAPLIST *list)
{
	SL_ERROR	rc;
	PL_LIST		pl;

	if (!battle) {
		RETURN_ERR_FUNCTION_EX0 (ERR_DOSSIER_ADD_SAVE_FAILED, "NULL battle argument");
	}
	if (!list) {
		RETURN_ERR_FUNCTION_EX0 (ERR_DOSSIER_ADD_SAVE_FAILED, "NULL snaplist argument");
	}

	GuiProgress gp ("Adding snapshot(s) to standalone battle...", 0);
	gp.setRange (0, (2*list->cnt) + 1);

	pl.list.snap = list;
	pl.savelist = false;
	pl.gametype = battle->dossier->gametype;
	rc = process_list (pl, add_to_standalone, battle, gp); SL_ROE(rc);

	RETURN_OK;
}
#endif	/* ALLOW_SNAPSHOTS_LOAD */

SL_ERROR
WARCABState::del (MDLD_TREE_ITEM *item)
{
	MDLD_TREE_ITEM	*next = NULL;
	SPWAW_ERROR	arc;
	SL_ERROR	rc;

	if (!item) RETURN_OK;

	next = next_safe_item (item);
	emit will_delete (item);

	switch (item->type) {
		case MDLD_TREE_STDALONE:
		case MDLD_TREE_BATTLE:
			arc = SPWAW_dossier_del (d.dossier, item->data.b);
			break;
		case MDLD_TREE_BTURN:
			arc = SPWAW_dossier_del (d.dossier, item->data.t);
			break;
		default:
			arc = SPWERR_OK;
			break;
	}
	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION_EX1 (ERR_DOSSIER_DEL_FAILED, "SPWAW_dossier_del() failed: %s", SPWAW_errstr (arc));
	}

	refresh_tree();
	emit was_deleted(next);

	rc = refresh_savelists ();
	if (SL_HAS_ERROR (rc)) {
		RETURN_ERR_FUNCTION (ERR_DOSSIER_DEL_FAILED, "WARCAB_refresh_savelists() failed");
	}

	set_dirty (true);

	RETURN_OK;
}

SL_ERROR
WARCABState::edit (char *name, char *comment)
{
	SPWAW_ERROR	arc;

	if (!is_loaded()) RETURN_OK;

	arc = SPWAW_dossier_edit (d.dossier, name, comment);
	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION_EX1 (ERR_DOSSIER_EDIT_FAILED, "SPWAW_dossier_edit() failed: %s", SPWAW_errstr (arc));
	}

	set_dirty (true);
	set_name (d.dossier->name);

	emit was_edited ();

	RETURN_OK;
}

bool
WARCABState::needs_postedinit (void)
{
	return (d.options.load);
}

bool
WARCABState::is_loaded (void)
{
	return (d.dossier != NULL);
}

bool
WARCABState::is_dirty (void)
{
	return (d.dossier && d.dirty);
}

char *
WARCABState::get_filename (void)
{
	if (!strlen(d.filename)) return (NULL);
	return (d.filename);
}

char *
WARCABState::get_name (void)
{
	if (!d.dossier) return (NULL);
	return (d.dossier->name);
}

char *
WARCABState::get_comment (void)
{
	if (!d.dossier) return (NULL);
	return (d.dossier->comment);
}

SPWAW_SAVELIST *
WARCABState::get_gamelist (void)
{
	return (d.gamelist);
}

#if	ALLOW_SNAPSHOTS_LOAD
SPWAW_SNAPLIST *
WARCABState::get_snaplist (void)
{
	return (d.snaplist);
}
#endif	/* ALLOW_SNAPSHOTS_LOAD */

SPWAW_GAME_TYPE
WARCABState::get_gametype (void)
{
	return (d.dossier?d.dossier->gametype:SPWAW_GAME_TYPE_UNKNOWN);
}

bool
WARCABState::can_track (void)
{
	return (d.tracking->is_ready());
}

SPWAW_DOSSIER *
WARCABState::get_dossier (void)
{
	return (d.dossier);
}

void
WARCABState::set_dirty (bool flag)
{
	d.dirty = flag ? SL_true: SL_false;
	// TODO: replace by signal
	if (GUI_WIN) GUI_WIN->setWindowModified (flag);
}

void
WARCABState::set_name (char *name)
{
	// TODO: replace by signal
	if (GUI_WIN) GUI_WIN->caption (name);
}

SL_ERROR
WARCABState::refresh_savelists (void)
{
	SPWAW_ERROR	arc;

	SPWAW_savelist_free (&d.gamelist);

	arc = SPWAW_dossier_savelist (d.dossier, &d.gamelist);
	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION_EX1 (ERR_DOSSIER_REFRESH_FAILED, "SPWAW_dossier_savelist() failed: %s", SPWAW_errstr (arc));
	}

#if	ALLOW_SNAPSHOTS_LOAD
	SPWAW_snaplist_free (&d.snaplist);

	arc = SPWAW_dossier_snaplist (d.dossier, &d.snaplist);
	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION_EX1 (ERR_DOSSIER_REFRESH_FAILED, "SPWAW_dossier_snaplist() failed: %s", SPWAW_errstr (arc));
	}
#endif	/* ALLOW_SNAPSHOTS_LOAD */

	RETURN_OK;
}

void
WARCABState::setup_tree_data (MDLD_TREE_ITEM *tree, SPWAW_DOSSIER_TYPE type, SPWAW_GAME_TYPE gametype)
{
	DWORD		i, j;
	MDLD_TREE_ITEM	*b, *t;

	if (!tree) return;

	DBG_log ("[%s]", __FUNCTION__);

	tree->dossier_type = type;
	tree->game_type = gametype;

	for (i=0; i<tree->data.d->bcnt; i++) {
		if (!tree->data.d->blist[i]) continue;

		b = MDLD_TREE_new_battle (tree->data.d->blist[i], tree);

		for (j=0; j<b->data.b->tcnt; j++) {
			if (!b->data.b->tlist[j]) continue;

			t = MDLD_TREE_new_bturn (b->data.b->tlist[j], b);
		}
	}
}

void
WARCABState::setup_tree (void)
{
	free_tree();

	if (!d.dossier) return;

	DBG_log ("[%s]", __FUNCTION__);

	d.tree = new MDLD_TREE_ITEM;
	d.tree->parent = NULL;
	d.tree->type = MDLD_TREE_DOSSIER;
	d.tree->data.d = d.dossier;
	d.tree->prev = d.tree->next = NULL;
	d.tree->cfirst = d.tree->clast = NULL;

	setup_tree_data (d.tree, d.tree->data.d->type, d.tree->data.d->gametype);
}

void
WARCABState::refresh_tree_data (MDLD_TREE_ITEM *tree, SPWAW_DOSSIER_TYPE type)
{
	SPWAW_DOSSIER_TYPE	odt;
	bool			transform;
	int			i, j, k;
	MDLD_TREE_ITEM		*b, *t;
	QList<MDLD_TREE_ITEM *>	blist, tlist;

	if (!tree) return;

	DBG_log ("[%s]", __FUNCTION__);

	odt = tree->dossier_type; tree->dossier_type = type;
	transform = (odt != tree->dossier_type);

	MDLD_TREE_extract_children (tree, blist);

	for (i=0; i<tree->data.d->bcnt; i++) {
		b = NULL;
		for (j=0; j<blist.size(); j++) {
			if (blist[j]->data.b == tree->data.d->blist[i]) {
				b = blist[j]; blist.removeAt(j);
				break;
			}
		}

		if (!b) {
			b = MDLD_TREE_new_battle (tree->data.d->blist[i], tree);

			for (j=0; j<b->data.b->tcnt; j++) {
				if (!b->data.b->tlist[j]) continue;

				t = MDLD_TREE_new_bturn (b->data.b->tlist[j], b);
			}

			MDLD_TREE_update_seqnums (b);
		} else {
			if (transform) {
				MDLD_TREE_ITEM *nb = MDLD_TREE_new_battle (tree->data.d->blist[i], tree);
				if (odt) {
					MDLD_TREE_extract_children (b, nb->children);
				} else {
					DEVASSERT (b->cfirst != NULL);

					MDLD_TREE_extract_children (b->cfirst, nb->children);
				}
				MDLD_TREE_delete_battle (b);
				b = nb;
			} else {
				MDLD_TREE_link_as_child (b);
				if (b->type == MDLD_TREE_STDALONE) b = b->cfirst;
			}

			MDLD_TREE_extract_children (b, tlist);

			for (j=0; j<b->data.b->tcnt; j++) {
				t = NULL;
				for (k=0; k<tlist.size(); k++) {
					if (tlist[k]->data.t == b->data.b->tlist[j]) {
						t = tlist[k]; tlist.removeAt(k);
						break;
					}
				}
				if (!t) {
					t = MDLD_TREE_new_bturn (b->data.b->tlist[j], b);

					MDLD_TREE_update_seqnums (t);
				} else {
					MDLD_TREE_link_as_child (t);
				}
			}
		}
		for (j=0; j<tlist.size(); j++) { MDLD_TREE_delete_bturn (tlist[j]); } tlist.clear();
	}
	for (i=0; i<blist.size(); i++) { MDLD_TREE_delete_battle (blist[i]); } blist.clear();
}

void
WARCABState::refresh_tree ()
{
	if (!d.tree || !d.tree->data.d) return;

	DBG_log ("[%s]", __FUNCTION__);

	refresh_tree_data (d.tree, d.tree->data.d->type);
}

void
WARCABState::free_tree_children (MDLD_TREE_ITEM *tree)
{
	int		i;
	MDLD_TREE_ITEM	*child;

	if (!tree) return;

	for (i=0; i<tree->children.size(); i++) {
		child = tree->children[i];
		free_tree_children (child);
		delete (child);
	}
	tree->children.clear();
}

void
WARCABState::free_tree (void)
{
	if (!d.tree) return;

	free_tree_children (d.tree);
	delete (d.tree); d.tree = NULL;
}

MDLD_TREE_ITEM *
WARCABState::item_from_turn (SPWAW_BTURN *turn)
{
	int		i, j;
	MDLD_TREE_ITEM	*b, *p;

	if (!d.tree || !turn) return (NULL);

	p = NULL;
	for (i=0; i<d.tree->children.size(); i++) {
		b = d.tree->children[i];
		if (b->type == MDLD_TREE_STDALONE) b = b->cfirst;
		for (j=0; j<b->children.size(); j++) {
			if (b->children[j]->data.t == turn) {
				p = b->children[j];
				break;
			}
		}
		if (p) break;
	}

	return (p);
}

MDLD_TREE_ITEM *
WARCABState::next_safe_item (MDLD_TREE_ITEM *item)
{
	MDLD_TREE_ITEM	*next = item;

	if (!item) return (NULL);

	if (next->type == MDLD_TREE_BTURN) {
		next = next->prev ? next->prev : (next->next ? next->next : next->parent);
	}
	/* We may arrive here for a battle, but also for the case of a single turn in a battle */
	if (next->type == MDLD_TREE_BATTLE) {
		next = next->prev ? next->prev : (next->next ? next->next : next->parent);
	}

	return (next);
}

void
WARCABState::statereport (SL_STDBG_INFO_LEVEL level)
{
	SAYSTATE2 ("### %s, instance 0x%8.8x, state:\n", MODULE, this);

	/* basic information */
	if (level >= SL_STDBG_LEVEL_BAS) {
		SAYSTATE1 ("\toptions: load       = %s\n", d.options.load ? d.options.load : "none");
		SAYSTATE1 ("\tdossier: dossier    = %s\n", d.dossier ? "loaded" : "none");
		SAYSTATE1 ("\tdossier: filename   = \"%s\"\n", d.filename);
		SAYSTATE1 ("\tdossier: dirty flag = %s\n", SL_BOOL_tostr (d.dirty));
	}

	/* extended information */
	if (level >= SL_STDBG_LEVEL_EXT) {
		SAYSTATE1 ("\tdossier: dossier data  = 0x%8.8x\n", d.dossier);
		SAYSTATE2 ("\tdossier: savegame list = 0x%8.8x, %lu entries\n", d.gamelist, d.gamelist ? d.gamelist->cnt : 0);
#if	ALLOW_SNAPSHOTS_LOAD
		SAYSTATE2 ("\tdossier: snapshot list = 0x%8.8x, %lu entries\n", d.snaplist, d.snaplist ? d.snaplist->cnt : 0);
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
	}

	SAYSTATE0 ("\n");

	d.tracking->statereport (level);
}

static void
statereport (SL_STDBG_INFO_LEVEL level)
{
	SAYSTATE1 ("### %s state:\n", MODULE);

	/* basic information */
	if (level >= SL_STDBG_LEVEL_BAS) {
		SAYSTATE1 ("\tinitialized = %s\n", SL_BOOL_tostr (initialized));
		SAYSTATE1 ("\tinstance    = 0x%8.8x\n", WARCAB);
	}

	/* extended information */
	if (level >= SL_STDBG_LEVEL_EXT) {
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
	}

	SAYSTATE0 ("\n");

	if (WARCAB) WARCAB->statereport (level);
}
