/** \file
 * The SPWaW war cabinet - application state handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "common.h"
#include "gui/gui_private.h"

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

	/* Reload last dossier? */
	d.options.load = CFG_autoload_get ();

	/* Filename specified on commandline overrides autoload! */
	if (info->context.argc > 1) {
		d.options.load = info->context.argv[1];
	}
}

WARCABState::~WARCABState ()
{
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
WARCABState::mknew (void)
{
	SPWAW_ERROR	arc;
	SL_ERROR	rc;

	close();

	arc = SPWAW_dossier_new (DEFAULT_NEW_NAME, DEFAULT_NEW_COMMENT, &d.dossier);
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

	DBG_log ("[%s] starting progress", __FUNCTION__);
	gp.setRange (0, 4);

	close();
	gp.inc();

	if (!list || !list->cnt) return (mknew());

	arc = SPWAW_dossier_load (list->list[0]->filepath, &d.dossier);
	if (SPWAW_HAS_ERROR (arc)) {
		switch (arc) {
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

	if (d.filename[0] == '\0') {
		RETURN_ERR_FUNCTION (ERR_DOSSIER_SAVE_NONAME, "empty filename");
	}

	arc = SPWAW_dossier_save (&d.dossier, d.filename, CFG_compress());
	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION_EX1 (ERR_DOSSIER_SAVE_FAILED, "SPWAW_dossier_save() failed: %s", SPWAW_errstr (arc));
	}

	set_dirty (false);

	gp.done();
	RETURN_OK;
}

SL_ERROR
WARCABState::saveas (char *file)
{
	SPWAW_ERROR	arc;

	if (!file) {
		RETURN_ERR_FUNCTION_EX0 (ERR_DOSSIER_SAVE_FAILED, "NULL filename argument");
	}

	if (!is_loaded()) RETURN_OK;

	arc = SPWAW_dossier_save (&d.dossier, file, CFG_compress());
	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION_EX1 (ERR_DOSSIER_SAVE_FAILED, "SPWAW_dossier_save() failed: %s", SPWAW_errstr (arc));
	}

	memset (d.filename, 0, sizeof (d.filename));
	snprintf (d.filename, sizeof (d.filename) - 1, "%s", file);

	set_dirty (false);

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
	SPWAW_snaplist_free (&d.snaplist);
	SPWAW_dossier_free (&d.dossier);

	emit was_closed ();

	RETURN_OK;
}

SL_ERROR
WARCABState::add (SPWAW_SAVELIST *list)
{
	unsigned long	i, done;
	SPWAW_SNAPSHOT	*snap;
	SPWAW_BTURN	*t = NULL;
	SPWAW_ERROR	arc = SPWERR_OK;
	SL_ERROR	rc;

	GuiProgress	gp ("Adding savegame(s)...", 0);

	if (!list) {
		RETURN_ERR_FUNCTION_EX0 (ERR_DOSSIER_ADD_SAVE_FAILED, "NULL savelist argument");
	}

	gp.setRange (0, (2*list->cnt) + 1);

	for (i=done=0; i<list->cnt; i++) {
		arc = SPWAW_snap_make (list->list[i]->dir, list->list[i]->id, &snap);
		if (SPWAW_HAS_ERROR (arc)) {
			SET_ERR_FUNCTION_EX1 (ERR_DOSSIER_ADD_SAVE_FAILED, "SPWAW_snap_make() failed: %s", SPWAW_errstr (arc));
			break;
		}
		gp.inc ();

		arc = SPWAW_dossier_add (d.dossier, snap, &t);
		if (SPWAW_HAS_ERROR (arc)) {
			SET_ERR_FUNCTION_EX1 (ERR_DOSSIER_ADD_SAVE_FAILED, "SPWAW_dossier_add() failed: %s", SPWAW_errstr (arc));
			SPWAW_snap_free (&snap);
			break;
		}
		gp.inc ();

		done++;
	}

	if (done) {
		set_dirty (true);
		rc = refresh_savelists ();
		if (SL_HAS_ERROR (rc)) {
			RETURN_ERR_FUNCTION (ERR_DOSSIER_ADD_SAVE_FAILED, "WARCAB_refresh_savelists() failed!");
		}
	}

	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION (ERR_DOSSIER_ADD_SAVE_FAILED, "failed to add a savegame");
	} else {
		refresh_tree();
		// FIXME: this will only work for the last savegame in the list. Is this intentional?
		MDLD_TREE_ITEM *item = item_from_turn (t); DEVASSERT (item != NULL);
		emit was_added (item);
	}

	gp.done ();
	RETURN_OK;
}

SL_ERROR
WARCABState::add (SPWAW_SNAPLIST *list)
{
	unsigned long	i, done;
	SPWAW_SNAPSHOT	*snap;
	SPWAW_BTURN	*t = NULL;
	SPWAW_ERROR	arc = SPWERR_OK;
	SL_ERROR	rc;

	GuiProgress	gp ("Adding snapshot(s)...", 0);

	if (!list) {
		RETURN_ERR_FUNCTION_EX0 (ERR_DOSSIER_ADD_SNAP_FAILED, "NULL snaplist argument");
	}

	gp.setRange (0, (2*list->cnt) + 1);

	for (i=done=0; i<list->cnt; i++) {
		arc = SPWAW_snap_load (list->list[i]->filepath, &snap);
		if (SPWAW_HAS_ERROR (arc)) {
			SET_ERR_FUNCTION_EX1 (ERR_DOSSIER_ADD_SNAP_FAILED, "SPWAW_snap_load() failed: %s", SPWAW_errstr (arc));
			break;
		}
		gp.inc ();

		arc = SPWAW_dossier_add (d.dossier, snap, &t);
		if (SPWAW_HAS_ERROR (arc)) {
			SET_ERR_FUNCTION_EX1 (ERR_DOSSIER_ADD_SNAP_FAILED, "SPWAW_dossier_add() failed: %s", SPWAW_errstr (arc));
			SPWAW_snap_free (&snap);
			break;
		}
		gp.inc ();

		done++;
	}

	if (done) {
		set_dirty (true);
		rc = refresh_savelists ();
		if (SL_HAS_ERROR (rc)) {
			RETURN_ERR_FUNCTION (ERR_DOSSIER_ADD_SAVE_FAILED, "WARCAB_refresh_savelists() failed!");
		}
	}

	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION (ERR_DOSSIER_ADD_SNAP_FAILED, "failed to add a snapshot");
	} else {
		refresh_tree();
		MDLD_TREE_ITEM *item = item_from_turn (t); DEVASSERT (item != NULL);
		emit was_added (item);
	}

	gp.done ();
	RETURN_OK;
}

SL_ERROR
WARCABState::del (MDLD_TREE_ITEM *item)
{
	MDLD_TREE_ITEM	*next = NULL;
	SPWAW_ERROR	arc;
	SL_ERROR	rc;

	if (!item) RETURN_OK;

	next = next_safe_item (item);
	if (next == d.tree)
		emit will_delete (NULL);
	else
		emit will_delete (item);

	switch (item->type) {
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

SPWAW_SNAPLIST *
WARCABState::get_snaplist (void)
{
	return (d.snaplist);
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
	SPWAW_snaplist_free (&d.snaplist);

	arc = SPWAW_dossier_savelist (d.dossier, &d.gamelist);
	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION_EX1 (ERR_DOSSIER_REFRESH_FAILED, "SPWAW_dossier_savelist() failed: %s", SPWAW_errstr (arc));
	}

	arc = SPWAW_dossier_snaplist (d.dossier, &d.snaplist);
	if (SPWAW_HAS_ERROR (arc)) {
		RETURN_ERR_FUNCTION_EX1 (ERR_DOSSIER_REFRESH_FAILED, "SPWAW_dossier_snaplist() failed: %s", SPWAW_errstr (arc));
	}

	RETURN_OK;
}

void
WARCABState::setup_tree (void)
{
	DWORD		i, j;
	MDLD_TREE_ITEM	*b, *t;
	MDLD_TREE_ITEM	*pb, *pt;

	free_tree();

	if (!d.dossier) return;

	d.tree = new MDLD_TREE_ITEM;
	d.tree->parent = NULL;
	d.tree->type = MDLD_TREE_DOSSIER;
	d.tree->data.d = d.dossier;

	pb = NULL;
	for (i=0; i<d.tree->data.d->bcnt; i++) {
		if (!d.tree->data.d->blist[i]) continue;

		b = new MDLD_TREE_ITEM;
		b->parent = d.tree;
		b->type = MDLD_TREE_BATTLE;
		b->data.b = b->parent->data.d->blist[i];

		b->prev = pb; b->next = NULL; pb = b;
		if (b->prev) b->prev->next = b;

		pt = NULL;
		for (j=0; j<b->data.b->tcnt; j++) {
			if (!b->data.b->tlist[j]) continue;

			t = new MDLD_TREE_ITEM;
			t->parent = b;
			t->type = MDLD_TREE_BTURN;
			t->data.t = b->data.b->tlist[j];

			t->cfirst = t->clast = NULL;

			t->prev = pt; t->next = NULL; pt = t;
			if (t->prev) t->prev->next = t;

			t->parent->children.append (t);
		}
		if (b->children.size()) {
			b->cfirst = b->children[0];
			b->clast  = b->children[b->children.size()-1];
		}else {
			b->cfirst = b->clast = NULL;
		}
		b->parent->children.append (b);
	}
	if (d.tree->children.size()) {
		d.tree->cfirst = d.tree->children[0];
		d.tree->clast  = d.tree->children[d.tree->children.size()-1];
	} else {
		d.tree->cfirst = d.tree->clast = NULL;
	}
}

void
WARCABState::refresh_tree ()
{
	int			i, j, k;
	MDLD_TREE_ITEM		*b, *t;
	MDLD_TREE_ITEM		*pb, *pt;
	QList<MDLD_TREE_ITEM *>	blist, tlist;

	if (!d.tree) return;

	pb = NULL;
	blist = d.tree->children; d.tree->children.clear();

	for (i=0; i<d.tree->data.d->bcnt; i++) {
		b = NULL;
		for (j=0; j<blist.size(); j++) {
			if (blist[j]->data.b == d.tree->data.d->blist[i]) {
				b = blist[j]; blist.removeAt(j);
				break;
			}
		}

		pt = NULL;
		if (!b) {
			b = new MDLD_TREE_ITEM;
			b->parent = d.tree;
			b->type = MDLD_TREE_BATTLE;
			b->data.b = b->parent->data.d->blist[i];

			for (j=0; j<b->data.b->tcnt; j++) {
				if (!b->data.b->tlist[j]) continue;

				t = new MDLD_TREE_ITEM;
				t->parent = b;
				t->type = MDLD_TREE_BTURN;
				t->data.t = b->data.b->tlist[j];

				t->cfirst = t->clast = NULL;

				t->prev = pt; t->next = NULL; pt = t;
				if (t->prev) t->prev->next = t;

				t->parent->children.append (t);
			}

			b->parent->seqnum.update();
		} else {
			tlist = b->children; b->children.clear();

			for (j=0; j<b->data.b->tcnt; j++) {
				t = NULL;
				for (k=0; k<tlist.size(); k++) {
					if (tlist[k]->data.t == b->data.b->tlist[j]) {
						t = tlist[k]; tlist.removeAt(k);
						break;
					}
				}
				if (!t) {
					t = new MDLD_TREE_ITEM;
					t->parent = b;
					t->type = MDLD_TREE_BTURN;
					t->data.t = b->data.b->tlist[j];

					t->cfirst = t->clast = NULL;

					t->parent->seqnum.update();
					t->parent->parent->seqnum.update();
				}
				t->prev = pt; t->next = NULL; pt = t;
				if (t->prev) t->prev->next = t;

				t->parent->children.append (t);
			}
		}
		if (b->children.size()) {
			b->cfirst = b->children[0];
			b->clast  = b->children[b->children.size()-1];
		} else {
			b->cfirst = b->clast = NULL;
		}

		for (j=0; j<tlist.size(); j++) { t = tlist[j]; t->parent->seqnum.update(); delete (t); } tlist.clear();

		b->prev = pb; b->next = NULL; pb = b;
		if (b->prev) b->prev->next = b;

		b->parent->children.append (b);
	}
	if (d.tree->children.size()) {
		d.tree->cfirst = d.tree->children[0];
		d.tree->clast  = d.tree->children[d.tree->children.size()-1];
	} else {
		d.tree->cfirst = d.tree->clast = NULL;
	}

	for (i=0; i<blist.size(); i++) { b = blist[i]; b->parent->seqnum.update(); delete (b); } blist.clear();
}

void
WARCABState::free_tree (void)
{
	int		i, j;
	MDLD_TREE_ITEM	*b, *t;

	if (!d.tree) return;

	for (i=0; i<d.tree->children.size(); i++) {
		b = d.tree->children[i];
		for (j=0; j<b->children.size(); j++) {
			t = b->children[j];
			delete (t);
		}
		b->children.clear();
		delete (b);
	}
	d.tree->children.clear();
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
		SAYSTATE2 ("\tdossier: snapshot list = 0x%8.8x, %lu entries\n", d.snaplist, d.snaplist ? d.snaplist->cnt : 0);
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
	}

	SAYSTATE0 ("\n");
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