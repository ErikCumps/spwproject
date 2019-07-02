/** \file
 * The SPWaW Library - savegame list API implementation.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
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
	const char	*base = NULL;
	char		match[MAX_PATH+1];

	if (!name || !id) return (false);
	if (!gamefile_basename (gametype, &base)) return (false);

	memset (match, 0, sizeof (match));
	snprintf (match, sizeof (match) - 1, "%s%%u.dat", base);

	*id = 0;
	return (sscanf (name, match, id) == 1);
}

static bool
file_on_list (SPWAW_SAVELIST *ignore, SPWAW_SAVELIST_NODE *node)
{
	unsigned long		i;
	SPWAW_SAVELIST_NODE	*p = NULL;

	if (!ignore) return (false);

	for (i=0; i<ignore->cnt; i++) {
		p = ignore->list[i];

		if (	(strcmp (p->info.stamp, node->info.stamp) == 0) &&
			(strcmp (p->info.location, node->info.location) == 0)
			)
		{
			if (CompareFileTime (&(p->filedate), &(node->filedate)) >= 0) break;
		}

		p = NULL;
	}
	return (p != NULL);
}

static bool
handle_file (SPWAW_GAME_TYPE gametype, const char *dir, WIN32_FIND_DATA f, SPWAW_SAVELIST *ignore, SPWAW_BATTLE_TYPE battletype, SPWAW_SAVELIST_NODE **p)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	unsigned int		id;
	SPWAW_SAVELIST_NODE	*ptr = NULL;
	SYSTEMTIME		mt_utc, mt_local;
	GAMEINFO		info;

	if (!p) return (false); *p = NULL;

	/* skip file if no id detected */
	if (!id_from_name (gametype, f.cFileName, &id)) goto handle_error;

	ptr = safe_malloc (SPWAW_SAVELIST_NODE); COOMGOTO (ptr, "SPWAW_SAVELIST_NODE", handle_error);

	/* id */
	ptr->id = id;

	/* file directory */
	_fullpath (ptr->dir, dir, sizeof (ptr->dir)-1);

	/* file name */
	snprintf (ptr->filename, sizeof (ptr->filename) - 1, "%s", f.cFileName);

	/* file path */
	snprintf (ptr->filepath, sizeof (ptr->filepath) - 1, "%s\\%s", ptr->dir, ptr->filename);

	/* file size */
	ptr->filesize = f.nFileSizeLow;

	/* file modification date */
	ptr->filedate = f.ftLastWriteTime;
	FileTimeToSystemTime (&(f.ftLastWriteTime), &mt_utc);
	SystemTimeToTzSpecificLocalTime (NULL, &mt_utc, &mt_local);
	snprintf (ptr->filestamp, sizeof (ptr->filestamp) - 1, "%4.4d/%02d/%02d %02d:%02d:%02d",
		mt_local.wYear, mt_local.wMonth, mt_local.wDay, mt_local.wHour, mt_local.wMinute, mt_local.wSecond);

	/* obtain game info */
	if (game_load_info (gametype, dir, id, &info)) {
		memcpy (ptr->info.stamp, info.stamp, sizeof (info.stamp));
		memcpy (ptr->info.location, info.location, sizeof (info.location));
		memcpy (ptr->info.comment, info.comment, sizeof (info.comment));
		ptr->info.type = info.type;
		ptr->info.gametype = info.gametype;
	}

	/* skip file if wrong battle type */
	switch (battletype) {
		case SPWAW_CAMPAIGN_BATTLE:
		case SPWAW_STDALONE_BATTLE:
			if (ptr->info.type != battletype) goto skip_file;
			break;
		default:
			break;
	}

	/* skip file if on ignore list */
	if (file_on_list (ignore, ptr)) goto skip_file;

	*p = ptr;
	return (true);

skip_file:
handle_error:
	if (ptr) free (ptr);
	return (false);
}

static void
list_files (SPWAW_GAME_TYPE gametype, const char *dir, SPWAW_SAVELIST *ignore, SPWAW_BATTLE_TYPE battletype, SPWAW_SAVELIST *list)
{
	const char		*base = NULL;
	char			buffer[MAX_PATH+1];
	HANDLE			h;
	WIN32_FIND_DATA		f;
	BOOL			stop = false;
	SPWAW_SAVELIST_NODE	*p;

	if (!gamefile_basename (gametype, &base)) return;

	memset (buffer, 0, sizeof (buffer));
	snprintf (buffer, sizeof (buffer) - 1, "%s\\%s*.dat", dir, base);

	h = FindFirstFile (buffer, &f);
	if (h == INVALID_HANDLE_VALUE) return;	// TODO: flag error!

	while (!stop) {
		if (handle_file (gametype, dir, f, ignore, battletype, &p)) {
			/* put on list */
			SPWAW_savelist_add (list, p);
		}

		stop = !FindNextFile (h, &f);
	}
	FindClose (h);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_savelist (SPWAW_GAME_TYPE gametype, const char *dir, SPWAW_SAVELIST *ignore, SPWAW_SAVELIST **list)
{
	SPWAW_ERROR	rc;

	rc = SPWAW_savelist_new (list); ROE ("SPWAW_savelist_new()");

	list_files (gametype, dir, ignore, SPWAW_UNKNOWN_BATTLE, *list);

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_savelist (SPWAW_GAME_TYPE gametype, const char *dir, SPWAW_SAVELIST *ignore, SPWAW_BATTLE_TYPE battletype, SPWAW_SAVELIST **list)
{
	SPWAW_ERROR	rc;

	rc = SPWAW_savelist_new (list); ROE ("SPWAW_savelist_new()");

	list_files (gametype, dir, ignore, battletype, *list);

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_savelist_new (SPWAW_SAVELIST **list)
{
	SPWAW_SAVELIST	*p = NULL;

	CSPWINIT;
	CNULLARG (list);
	*list = NULL;

	p = safe_malloc (SPWAW_SAVELIST); COOM (p, "SPWAW_SAVELIST");

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
			for (unsigned int i=0; i<l->cnt; i++) if (l->list[i]) free (l->list[i]);
			free (l->list);
		}
		free (l);
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
			free (list->list);
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
	SPWAW_SAVELIST_NODE	**p;
	unsigned long		idx;

	CSPWINIT;
	CNULLARG (list); CNULLARG (snap);

	SPWAW_SAVELIST_NODE *node = safe_malloc (SPWAW_SAVELIST_NODE);
	COOM (node, "SPWAW_SAVELIST_NODE element");

	snprintf (node->dir, sizeof (node->dir) - 1, "%s", snap->src.path);
	snprintf (node->filename, sizeof (node->filename) - 1, "%s", snap->src.file);
	snprintf (node->filepath, sizeof (node->filepath) - 1, "%s\\%s", snap->src.path, snap->src.file);
	node->filedate = snap->src.date;

	snprintf (node->info.stamp, sizeof (node->info.stamp) - 1, "%s, turn %u",
		snap->game.battle.strings.date, snap->game.battle.data.tdate.turn);
	snprintf (node->info.location, sizeof (node->info.location) - 1, "%s", snap->raw.game.battle.location);
	snprintf (node->info.comment, sizeof (node->info.comment) - 1, "%s", snap->raw.game.cmt.title);
	node->info.type = snap->type;
	node->info.gametype = snap->gametype;

	if (list->cnt >= list->len) {
		list->len += LISTINC;
		p = safe_nmalloc (SPWAW_SAVELIST_NODE *, list->len); COOM (p, "SPWAW_SAVELIST_NODE list");

		if (list->list) {
			memcpy (p, list->list, list->cnt * sizeof (SPWAW_SAVELIST_NODE *));
			free (list->list);
		}
		list->list = p;
	}
	idx = list->cnt++;
	list->list[idx] = node;

	return (SPWERR_OK);
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
	rc = SPWAW_savelist_add (list, p);
	if (rc != SPWERR_OK) free (p);

	return (rc);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_savelist_clear (SPWAW_SAVELIST *list)
{
	unsigned int	i;

	CSPWINIT;
	CNULLARG (list);

	for (i=0; i<list->cnt; i++) if (list->list[i]) { free (list->list[i]); list->list[i] = NULL; }
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
