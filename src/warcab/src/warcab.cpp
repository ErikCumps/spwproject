/** \file
 * The SPWaW war cabinet - application state handling.
 *
 * Copyright (C) 2005-2018 Erik Cumps <erik.cumps@gmail.com>
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

typedef QMap<QString, QString>			FAILMAP;
typedef QMap<QString, QString>::const_iterator	FAILMAP_it;

SL_ERROR
WARCABState::add (SPWAW_SAVELIST *list)
{
	unsigned long	i, done;
	SPWAW_BTURN	*added = NULL;
	FAILMAP		failures;

	GuiProgress		gp ("Adding savegame(s)...", 0);

	if (!list) {
		RETURN_ERR_FUNCTION_EX0 (ERR_DOSSIER_ADD_SAVE_FAILED, "NULL savelist argument");
	}

	gp.setRange (0, (2*list->cnt) + 1);

	for (i=done=0; i<list->cnt; i++) {
		SPWAW_ERROR	arc;
		SPWAW_SNAPSHOT	*s;
		SPWAW_BTURN	*t;
		
		arc = SPWAW_snap_make (list->list[i]->dir, list->list[i]->id, &s);
		if (SPWAW_HAS_ERROR (arc)) {
			failures[list->list[i]->filename] = SPWAW_errstr (arc);
			continue;
		}
		gp.inc ();

		arc = SPWAW_dossier_add (d.dossier, s, &t);
		if (SPWAW_HAS_ERROR (arc)) {
			failures[list->list[i]->filename] = SPWAW_errstr (arc);
			SPWAW_snap_free (&s);
			continue;
		} else {
			if (!added) added = t;
		}
		gp.inc ();

		done++;
	}

	if (done) {
		set_dirty (true);

		refresh_tree();
		MDLD_TREE_ITEM *item = item_from_turn (added); DEVASSERT (item != NULL);
		emit was_added (item);

		SL_ERROR rc = refresh_savelists ();
		if (SL_HAS_ERROR (rc)) {
			RETURN_ERR_FUNCTION (ERR_DOSSIER_ADD_SAVE_FAILED, "WARCAB_refresh_savelists() failed!");
		}
	}

	if (!failures.empty())
	{
		char		buf[4096];
		UtilStrbuf	str(buf, sizeof (buf), true, true);

		char *title = (failures.count() == 1) ? "Failed to add savegame" : "Failed to add some savegames";

		for (FAILMAP_it i = failures.constBegin(); i != failures.constEnd(); ++i)
			str.printf ("%s: %s\n", qPrintable(i.key()), qPrintable(i.value()));

		GUI_errorbox (SL_ERR_FATAL_WARN, title, "Savegame", buf);
	}

	gp.done ();
	RETURN_OK;
}

SL_ERROR
WARCABState::add (SPWAW_SNAPLIST *list)
{
	unsigned long	i, done;
	SPWAW_BTURN	*added = NULL;
	FAILMAP		failures;

	GuiProgress	gp ("Adding snapshot(s)...", 0);

	if (!list) {
		RETURN_ERR_FUNCTION_EX0 (ERR_DOSSIER_ADD_SNAP_FAILED, "NULL snaplist argument");
	}

	gp.setRange (0, (2*list->cnt) + 1);

	for (i=done=0; i<list->cnt; i++) {
		SPWAW_ERROR	arc;
		SPWAW_SNAPSHOT	*s;
		SPWAW_BTURN	*t;

		arc = SPWAW_snap_load (list->list[i]->filepath, &s);
		if (SPWAW_HAS_ERROR (arc)) {
			failures[list->list[i]->filepath] = SPWAW_errstr (arc);
			continue;
		}
		gp.inc ();

		arc = SPWAW_dossier_add (d.dossier, s, &t);
		if (SPWAW_HAS_ERROR (arc)) {
			failures[list->list[i]->filepath] = SPWAW_errstr (arc);
			SPWAW_snap_free (&s);
			continue;
		} else {
			if (!added) added = t;
		}
		gp.inc ();

		done++;
	}

	if (done) {
		set_dirty (true);

		refresh_tree();
		MDLD_TREE_ITEM *item = item_from_turn (added); DEVASSERT (item != NULL);
		emit was_added (item);

		SL_ERROR rc = refresh_savelists ();
		if (SL_HAS_ERROR (rc)) {
			RETURN_ERR_FUNCTION (ERR_DOSSIER_ADD_SAVE_FAILED, "WARCAB_refresh_savelists() failed!");
		}
	}

	if (!failures.empty())
	{
		char		buf[4096];
		UtilStrbuf	str(buf, sizeof (buf), true, true);

		char *title = (failures.count() == 1) ? "Failed to add snapshot" : "Failed to add some snapshots";

		for (FAILMAP_it i = failures.constBegin(); i != failures.constEnd(); ++i)
			str.printf ("%s: %s\n", qPrintable(i.key()), qPrintable(i.value()));

		GUI_errorbox (SL_ERR_FATAL_WARN, title, "Snapshot", buf);
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

static void
link_as_child (MDLD_TREE_ITEM *item)
{
	MDLD_TREE_ITEM	*parent;

	if (!item) return;

	parent = item->parent;
	if (!parent) {
		item->prev = item->next = NULL;
	} else {
		parent->children.append (item);

		item->prev = parent->clast; item->next = NULL;
		if (item->prev) item->prev->next = item;

		parent->cfirst = parent->children[0];
		parent->clast  = parent->children[parent->children.size()-1];
	}
}

static MDLD_TREE_ITEM *
new_battle (SPWAW_BATTLE *data, MDLD_TREE_ITEM *tree)
{
	MDLD_TREE_ITEM	*p = tree;
	MDLD_TREE_ITEM	*b;

	if (!tree->campaign) {
		p = new MDLD_TREE_ITEM;
		p->parent   = tree;
		p->type     = MDLD_TREE_STDALONE;
		p->data.b   = data;
		p->campaign = tree->campaign;

		p->cfirst = p->clast = NULL;

		link_as_child (p);
	}

	b = new MDLD_TREE_ITEM;
	b->parent   = p;
	b->type     = MDLD_TREE_BATTLE;
	b->data.b   = data;
	b->campaign = tree->campaign;

	b->cfirst = b->clast = NULL;

	link_as_child (b);

	return (b);
}

static void
delete_battle (MDLD_TREE_ITEM *b)
{
	MDLD_TREE_ITEM	*p, *q;

	b->parent->seqnum.update();

	if (!b->campaign) {
		 b = b->cfirst;
		 delete (b->parent);
	}

	p = b->cfirst;
	while (p) {
		q = p; p = p->next; delete (q);
	}

	delete (b);
}

static MDLD_TREE_ITEM *
new_bturn (SPWAW_BTURN *data, MDLD_TREE_ITEM *tree)
{
	MDLD_TREE_ITEM	*t;

	t = new MDLD_TREE_ITEM;
	t->parent   = tree;
	t->type     = MDLD_TREE_BTURN;
	t->data.t   = data;
	t->campaign = tree->campaign;

	t->cfirst = t->clast = NULL;

	link_as_child (t);

	return (t);
}

static void
delete_bturn (MDLD_TREE_ITEM *t)
{
	t->parent->seqnum.update();
	delete (t);
}

void
WARCABState::setup_tree_data (MDLD_TREE_ITEM *tree, bool campaign)
{
	DWORD		i, j;
	MDLD_TREE_ITEM	*b, *t;

	if (!tree) return;

	DBG_log ("[%s]", __FUNCTION__);

	tree->campaign = campaign;

	for (i=0; i<tree->data.d->bcnt; i++) {
		if (!tree->data.d->blist[i]) continue;

		b = new_battle (tree->data.d->blist[i], tree);

		for (j=0; j<b->data.b->tcnt; j++) {
			if (!b->data.b->tlist[j]) continue;

			t = new_bturn (b->data.b->tlist[j], b);
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

	setup_tree_data (d.tree, d.tree->data.d->type == SPWAW_CAMPAIGN_DOSSIER);
}

void
WARCABState::refresh_tree_data (MDLD_TREE_ITEM *tree, bool campaign)
{
	bool			ocf, transform;
	int			i, j, k;
	MDLD_TREE_ITEM		*b, *t;
	QList<MDLD_TREE_ITEM *>	blist, tlist;

	if (!tree) return;

	DBG_log ("[%s]", __FUNCTION__);

	ocf = tree->campaign; tree->campaign = campaign;
	transform = (ocf != tree->campaign);

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
			b = new_battle (tree->data.d->blist[i], tree);

			for (j=0; j<b->data.b->tcnt; j++) {
				if (!b->data.b->tlist[j]) continue;

				t = new_bturn (b->data.b->tlist[j], b);
			}

			b->parent->seqnum.update();
		} else {
			if (transform) {
				MDLD_TREE_ITEM *nb = new_battle (tree->data.d->blist[i], tree);
				if (ocf) {
					MDLD_TREE_extract_children (b, nb->children);
				} else {
					DEVASSERT (b->cfirst != NULL);

					MDLD_TREE_extract_children (b->cfirst, nb->children);
				}
				delete_battle (b);
				b = nb;
			} else {
				link_as_child (b);
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
					t = new_bturn (b->data.b->tlist[j], b);

					t->parent->seqnum.update();
					t->parent->parent->seqnum.update();
				} else {
					link_as_child (t);
				}
			}
		}
		for (j=0; j<tlist.size(); j++) { delete_bturn (tlist[j]); } tlist.clear();
	}
	for (i=0; i<blist.size(); i++) { delete_battle (blist[i]); } blist.clear();
}

void
WARCABState::refresh_tree ()
{
	if (!d.tree || !d.tree->data.d) return;

	DBG_log ("[%s]", __FUNCTION__);

	refresh_tree_data (d.tree, d.tree->data.d->type == SPWAW_CAMPAIGN_DOSSIER);
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
