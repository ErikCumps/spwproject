/** \file
 * The SPWaW Library - savegame list API implementation.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_savelist.h>
#include "gamefile/game.h"
#include "common/internal.h"

/* --- forward declarations --- */
static SPWAW_ERROR SPWAW_savelist_add (SPWAW_SAVELIST *list, SPWAW_SAVELIST_NODE *node);

#define	LISTINC	8

static bool
id_from_name (SPWAW_GAME_TYPE gametype, char *name, unsigned int *id)
{
	GAMEFILE_META	gfm;
	char		match[MAX_PATH+1];

	if (!name || !id) return (false);
	if (!gamefile_info (gametype, SPWAW_SAVE_TYPE_REGULAR, &gfm)) return (false);
	if (!gfm.base) return (false);

	memset (match, 0, sizeof (match));
	snprintf (match, sizeof (match) - 1, "%s%%u.%s", gfm.base, gfm.ext_data);

	*id = 0;
	return (sscanf (name, match, id) == 1);
}

static void
basename_from_name (char *name, char *dst, unsigned int len)
{
	char		*p;
	unsigned int	todo;

	if (!dst || !len) return;
	memset (dst, 0, len);

	if (!name) return;

	p = strrchr (name, '.');
	todo = p ? (p - name) : strlen (name);
	if (todo >= len) todo = len-1;
	memcpy (dst, name, todo);
}

static bool
file_on_list (SPWAW_SAVELIST_TARGET &target, SPWAW_SAVELIST *ignore, SPWAW_SAVELIST_NODE *node)
{
	unsigned long		i;
	bool			matched = false;

	if (!ignore) return (false);

	for (i=0; i<ignore->cnt; i++) {
		SPWAW_SAVELIST_NODE *p = ignore->list[i];
		matched = false;

		switch (target.type) {
			case SPWAW_CAMPAIGN_DOSSIER:
			case SPWAW_MEGACAM_DOSSIER:
				// match on battle timestamp
				if (	strcmp (p->info.stamp, node->info.stamp) == 0
					) matched = true;
				break;
			case SPWAW_STDALONE_DOSSIER:
			default:
				// match on filename and not-newer file
				if (	(strcmp (p->filepath, node->filepath) == 0) &&
					(CompareFileTime (&(p->filedate), &(node->filedate)) >= 0)
					) matched = true;
				break;
		}

		if (matched) break;
	}
	return (matched);
}

static bool
handle_file (SPWAW_SAVELIST_TARGET &target, SPWAW_SAVE_TYPE savetype, const char *dir, WIN32_FIND_DATA f, SPWAW_SAVELIST *ignore, SPWAW_SAVELIST_NODE **p)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	unsigned int		id;
	char			base[MAX_PATH+1];
	SPWAW_SAVELIST_NODE	*ptr = NULL;
	SYSTEMTIME		mt_utc, mt_local;
	GAMEINFO		info;

	if (!p) return (false); *p = NULL;

	ptr = safe_malloc (SPWAW_SAVELIST_NODE); COOMGOTO (ptr, "SPWAW_SAVELIST_NODE", handle_error);

	if (savetype == SPWAW_SAVE_TYPE_REGULAR) {
		/* skip file if no id detected */
		if (!id_from_name (target.gametype, f.cFileName, &id)) goto handle_error;

		rc = SPWAW_savegame_descriptor_init (ptr->sgd, target.gametype, savetype, target.oobdir, dir, id);
		ERRORGOTO ("SPWAW_savegame_descriptor_init()", handle_error);
	} else {
		/* Non-regular save type games have no id */
		basename_from_name (f.cFileName, base, sizeof (base));

		rc = SPWAW_savegame_descriptor_init (ptr->sgd, target.gametype, savetype, target.oobdir, dir, base);
		ERRORGOTO ("SPWAW_savegame_descriptor_init()", handle_error);
	}

	/* obtain game info */
	if (game_load_info (&ptr->sgd, &info)) {
		memcpy (ptr->info.stamp, info.stamp, sizeof (info.stamp));
		memcpy (ptr->info.location, info.location, sizeof (info.location));
		memcpy (ptr->info.title, info.title, sizeof (info.title));
		ptr->info.type = info.type;
		ptr->info.gametype = info.gametype;
		ptr->info.savetype = info.savetype;
	}

	/* skip file if wrong battle type */
	switch (target.type) {
		case SPWAW_CAMPAIGN_DOSSIER:
		case SPWAW_MEGACAM_DOSSIER:
			if (ptr->info.type != target.type) goto skip_file;
			break;
		case SPWAW_STDALONE_DOSSIER:
		default:
			break;
	}

	/* file name */
	snprintf (ptr->filename, sizeof (ptr->filename) - 1, "%s", f.cFileName);

	/* file path */
	snprintf (ptr->filepath, sizeof (ptr->filepath) - 1, "%s\\%s", ptr->sgd.path, ptr->filename);

	/* file size */
	ptr->filesize = f.nFileSizeLow;

	/* file modification date */
	ptr->filedate = f.ftLastWriteTime;
	FileTimeToSystemTime (&(f.ftLastWriteTime), &mt_utc);
	SystemTimeToTzSpecificLocalTime (NULL, &mt_utc, &mt_local);
	snprintf (ptr->filestamp, sizeof (ptr->filestamp) - 1, "%4.4d/%02d/%02d %02d:%02d:%02d",
		mt_local.wYear, mt_local.wMonth, mt_local.wDay, mt_local.wHour, mt_local.wMinute, mt_local.wSecond);


	/* skip file if on ignore list */
	if (file_on_list (target, ignore, ptr)) goto skip_file;

	*p = ptr;
	return (true);

skip_file:
handle_error:
	if (ptr) {
		SPWAW_savegame_descriptor_clear (ptr->sgd);
		safe_free (ptr);
	}
	return (false);
}

static void
list_files (SPWAW_SAVE_TYPE savetype, const char *dir, SPWAW_SAVELIST *ignore, SPWAW_SAVELIST *list)
{
	GAMEFILE_META	gfm;
	char			buffer[MAX_PATH+1];
	HANDLE			h;
	WIN32_FIND_DATA		f;
	BOOL			stop = false;
	SPWAW_SAVELIST_NODE	*p;

	if (!gamefile_info (list->target.gametype, savetype, &gfm)) return;

	memset (buffer, 0, sizeof (buffer));
	if (gfm.base) {
		snprintf (buffer, sizeof (buffer) - 1, "%s\\%s*.%s", dir, gfm.base, gfm.ext_data);
	} else {
		snprintf (buffer, sizeof (buffer) - 1, "%s\\*.%s", dir, gfm.ext_data);
	}

	h = FindFirstFile (buffer, &f);
	if (h == INVALID_HANDLE_VALUE) return;	// TODO: flag error!

	while (!stop) {
		if (handle_file (list->target, savetype, dir, f, ignore, &p)) {
			/* put on list */
			SPWAW_savelist_add (list, p);
		}

		stop = !FindNextFile (h, &f);
	}
	FindClose (h);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_savelist (SPWAW_SAVELIST_TARGET *target, const char *dir, SPWAW_SAVELIST *ignore, SPWAW_SAVELIST **list)
{
	SPWAW_ERROR	rc;
	SPWAW_SAVE_TYPE	types[2];

	CSPWINIT;
	CNULLARG (target); CNULLARG (dir); CNULLARG (list);

	rc = SPWAW_savelist_new (target, list); ROE ("SPWAW_savelist_new()");

	memset (types, 0, sizeof (types));
	switch (target->type) {
		case SPWAW_CAMPAIGN_DOSSIER:
		case SPWAW_STDALONE_DOSSIER:
			types[0] = SPWAW_SAVE_TYPE_REGULAR;
			types[1] = SPWAW_SAVE_TYPE_MEGACAM;
			break;
		case SPWAW_MEGACAM_DOSSIER:
			types[0] = SPWAW_SAVE_TYPE_MEGACAM;
			break;
	}

	for (int i=0; i<ARRAYCOUNT(types); i++) {
		list_files (types[i], dir, ignore, *list);
	}

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_savelist_new (SPWAW_SAVELIST_TARGET *target, SPWAW_SAVELIST **list)
{
	SPWAW_SAVELIST	*p = NULL;

	CSPWINIT;
	CNULLARG (target); CNULLARG (list);
	*list = NULL;

	p = safe_malloc (SPWAW_SAVELIST); COOM (p, "SPWAW_SAVELIST");
	p->target = *target;

	*list = p;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_savelist_free (SPWAW_SAVELIST **list)
{
	SPWAW_SAVELIST	*l;

	CSPWINIT;
	CNULLARG (list);

	l = *list; *list = NULL;
	if (l) {
		if (l->list) {
			for (unsigned int i=0; i<l->cnt; i++) {
				if (l->list[i]) {
					SPWAW_savegame_descriptor_clear (l->list[i]->sgd);
					safe_free (l->list[i]);
				}
			}
			safe_free (l->list);
		}
		safe_free (l);
	}

	return (SPWERR_OK);
}

static SPWAW_ERROR
SPWAW_savelist_add (SPWAW_SAVELIST *list, SPWAW_SAVELIST_NODE *node)
{
	SPWAW_SAVELIST_NODE	**p;
	unsigned long		idx;

	CNULLARG (list); CNULLARG (node);

	if (list->cnt >= list->len) {
		list->len += LISTINC;
		p = safe_nmalloc (SPWAW_SAVELIST_NODE *, list->len); COOM (p, "SPWAW_SAVELIST_NODE list");

		if (list->list) {
			memcpy (p, list->list, list->cnt * sizeof (SPWAW_SAVELIST_NODE *));
			safe_free (list->list);
		}
		list->list = p;
	}
	idx = list->cnt++;
	list->list[idx] = node;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_savelist_add (SPWAW_SAVELIST *list, SPWAW_SNAPSHOT *snap)
{
	SPWAW_ERROR		rc;

	CSPWINIT;
	CNULLARG (list); CNULLARG (snap);

	SPWAW_SAVELIST_NODE *node = safe_malloc (SPWAW_SAVELIST_NODE);
	COOM (node, "SPWAW_SAVELIST_NODE element");

	rc = SPWAW_savegame_descriptor_init (node->sgd, snap->gametype, snap->savetype, list->target.oobdir, snap->src.path, SPWAW_BADLONGIDX);
	ROE ("SPWAW_savegame_descriptor_init()");

	snprintf (node->filename, sizeof (node->filename) - 1, "%s", snap->src.file);
	snprintf (node->filepath, sizeof (node->filepath) - 1, "%s\\%s", snap->src.path, snap->src.file);
	node->filedate = snap->src.date;

	snprintf (node->info.stamp, sizeof (node->info.stamp) - 1, "%s, turn %u",
		snap->game.battle.strings.date, snap->game.battle.data.tdate.turn);
	snprintf (node->info.location, sizeof (node->info.location) - 1, "%s", snap->raw.game.battle.location);
	snprintf (node->info.title, sizeof (node->info.title) - 1, "%s", snap->raw.game.meta.title);
	node->info.type = snap->type;
	node->info.gametype = snap->gametype;
	node->info.savetype = snap->savetype;

	return (SPWAW_savelist_add (list, node));
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_savelist_addcpy (SPWAW_SAVELIST *list, SPWAW_SAVELIST_NODE *node)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	SPWAW_SAVELIST_NODE	*p = NULL;

	CSPWINIT;
	CNULLARG (list); CNULLARG (node);

	p = safe_malloc (SPWAW_SAVELIST_NODE); COOM (p, "SPWAW_SAVELIST_NODE");

	memcpy (p, node, sizeof (*p));

	rc = SPWAW_savegame_descriptor_init (p->sgd, node->sgd);
	ERRORGOTO ("SPWAW_savegame_descriptor_init()", handle_error);

	rc = SPWAW_savelist_add (list, p);
	ERRORGOTO ("SPWAW_savelist_add()", handle_error);

	return (SPWERR_OK);

handle_error:
	if (p) {
		SPWAW_savegame_descriptor_clear (p->sgd);
		safe_free (p);
	}
	return (rc);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_savelist_clear (SPWAW_SAVELIST *list)
{
	unsigned int	i;

	CSPWINIT;
	CNULLARG (list);

	for (i=0; i<list->cnt; i++) if (list->list[i]) {
		SPWAW_savegame_descriptor_clear (list->list[i]->sgd);
		safe_free (list->list[i]);
	}
	list->cnt = 0;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_savelist_copy (SPWAW_SAVELIST *list, SPWAW_SAVELIST *src)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	unsigned int	i;

	CSPWINIT;
	CNULLARG (list); CNULLARG (src);

	rc = SPWAW_savelist_clear (list); ROE ("SPWAW_savelist_clear()");

	for (i=0; i<src->cnt; i++) {
		rc = SPWAW_savelist_addcpy (list, src->list[i]);
		ERRORGOTO("SPWAW_savelist_addcpy()", handle_error);
	}

	return (SPWERR_OK);

handle_error:
	SPWAW_savelist_clear (list);
	return (rc);
}
