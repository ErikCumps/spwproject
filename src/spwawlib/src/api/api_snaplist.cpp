/** \file
 * The SPWaW Library - snapshot list API implementation.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_snaplist.h>
#include "gamefile/gamefile.h"
#include "common/internal.h"

/* --- forward declarations --- */
static SPWAW_ERROR SPWAW_snaplist_add (SPWAW_SNAPLIST *list, SPWAW_SNAPLIST_NODE *node);

#define	LISTINC	8

static bool
file_on_list (SPWAW_SNAPLIST *ignore, SPWAW_SNAPLIST_NODE *node)
{
	unsigned long		i;
	SPWAW_SNAPLIST_NODE	*p = NULL;

	if (!ignore) return (false);

	for (i=0; i<ignore->cnt; i++) {
		p = ignore->list[i];

		if (	(strcmp (p->info.stamp, node->info.stamp) == 0) &&
			(strcmp (p->info.location, node->info.location) == 0)
		) {
			if (CompareFileTime (&(p->filedate), &(node->filedate)) >= 0) break;
			break;
		}

		p = NULL;
	}
	return (p != NULL);
}

static bool
handle_file (SPWAW_SNAPLIST_TARGET &target, const char *dir, WIN32_FIND_DATA f, SPWAW_SNAPLIST *ignore, SPWAW_SNAPLIST_NODE **p)
{
	SPWAW_ERROR		rc;
	SPWAW_SNAPLIST_NODE	*ptr = NULL;
	SYSTEMTIME		mt_utc, mt_local;

	if (!p) return (false); *p = NULL;

	ptr = safe_malloc (SPWAW_SNAPLIST_NODE); COOMGOTO (ptr, "SPWAW_SNAPLIST_NODE", handle_error);

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

	/* snapshot title, game turn and date info*/
	rc = SPWAW_snap_info (ptr->filepath, &(ptr->info));
	if (rc != SPWERR_OK) goto handle_error;

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
list_files (const char *dir, SPWAW_SNAPLIST *ignore, SPWAW_SNAPLIST *list)
{
	char			buffer[MAX_PATH+1];
	HANDLE			h;
	WIN32_FIND_DATA		f;
	BOOL			stop = false;
	SPWAW_SNAPLIST_NODE	*p;

	memset (buffer, 0, sizeof (buffer));
	snprintf (buffer, sizeof (buffer) - 1, "%s\\*.snap", dir);

	h = FindFirstFile (buffer, &f);
	if (h == INVALID_HANDLE_VALUE) return;	// TODO: flag error!

	while (!stop) {
		if (handle_file (list->target, dir, f, ignore, &p)) {
			/* put on list */
			SPWAW_snaplist_add (list, p);
		}

		stop = !FindNextFile (h, &f);
	}
	FindClose (h);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_snaplist (SPWAW_SNAPLIST_TARGET *target, const char *dir, SPWAW_SNAPLIST *ignore, SPWAW_SNAPLIST **list)
{
	SPWAW_ERROR	rc;

	CSPWINIT;
	CNULLARG (target); CNULLARG (dir); CNULLARG (list);

	rc = SPWAW_snaplist_new (target, list); ROE ("SPWAW_snaplist_new()");

	list_files (dir, ignore, *list);

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_snaplist_new (SPWAW_SNAPLIST_TARGET *target, SPWAW_SNAPLIST **list)
{
	SPWAW_SNAPLIST	*p = NULL;

	CSPWINIT;
	CNULLARG (list);
	*list = NULL;

	p = safe_malloc (SPWAW_SNAPLIST); COOM (p, "SPWAW_SNAPLIST");
	p->target = *target;

	*list = p;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_snaplist_free (SPWAW_SNAPLIST **list)
{
	SPWAW_SNAPLIST	*l;

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
SPWAW_snaplist_add (SPWAW_SNAPLIST *list, SPWAW_SNAPLIST_NODE *node)
{
	SPWAW_SNAPLIST_NODE	**p;
	unsigned long		idx;

	CNULLARG (list); CNULLARG (node);

	if (list->cnt >= list->len) {
		list->len += LISTINC;
		p = safe_nmalloc (SPWAW_SNAPLIST_NODE *, list->len); COOM (p, "SPWAW_SNAPLIST_NODE LIST");

		if (list->list) {
			memcpy (p, list->list, list->cnt * sizeof (SPWAW_SNAPLIST_NODE *));
			free (list->list);
		}
		list->list = p;
	}
	idx = list->cnt++;
	list->list[idx] = node;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_snaplist_add (SPWAW_SNAPLIST *list, SPWAW_SNAPSHOT *snap)
{
	SPWAW_SNAPLIST_NODE	**p;
	unsigned long		idx;

	CNULLARG (list); CNULLARG (snap);

	SPWAW_SNAPLIST_NODE *node = safe_malloc (SPWAW_SNAPLIST_NODE);
	COOM (node, "SPWAW_SNAPLIST_NODE element");

	snprintf (node->dir, sizeof (node->dir) - 1, "%s", snap->src.path);
	snprintf (node->filename, sizeof (node->filename) - 1, "%s", snap->src.file);
	snprintf (node->filepath, sizeof (node->filepath) - 1, "%s\\%s", snap->src.path, snap->src.file);
	node->filedate = snap->src.date;

	snprintf (node->info.title, sizeof (node->info.title) - 1, "%s", snap->raw.game.meta.title);
	node->info.start = snap->game.campaign.data.start;
	node->info.tdate = snap->game.battle.data.tdate;
	snprintf (node->info.stamp, sizeof (node->info.stamp) - 1, "%s, turn %u",
		snap->game.battle.strings.date, snap->game.battle.data.tdate.turn);
	snprintf (node->info.location, sizeof (node->info.location) - 1, "%s", snap->raw.game.battle.location);
	snprintf (node->info.filename, sizeof (node->info.filename) - 1, "%s", snap->src.file);
	node->info.filedate = node->filedate;
	node->info.type = snap->type;
	node->info.gametype = snap->gametype;

	if (list->cnt >= list->len) {
		list->len += LISTINC;
		p = safe_nmalloc (SPWAW_SNAPLIST_NODE *, list->len); COOM (p, "SPWAW_SNAPLIST_NODE LIST");

		if (list->list) {
			memcpy (p, list->list, list->cnt * sizeof (SPWAW_SNAPLIST_NODE *));
			free (list->list);
		}
		list->list = p;
	}
	idx = list->cnt++;
	list->list[idx] = node;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_snaplist_addcpy (SPWAW_SNAPLIST *list, SPWAW_SNAPLIST_NODE *node)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	SPWAW_SNAPLIST_NODE	*p = NULL;

	CSPWINIT;
	CNULLARG (list); CNULLARG (node);

	p = safe_malloc (SPWAW_SNAPLIST_NODE); COOM (p, "SPWAW_SNAPLIST_NODE");

	memcpy (p, node, sizeof (*p));
	rc = SPWAW_snaplist_add (list, p);
	if (rc != SPWERR_OK) free (p);

	return (rc);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_snaplist_clear (SPWAW_SNAPLIST *list)
{
	unsigned int	i;

	CSPWINIT;
	CNULLARG (list);

	for (i=0; i<list->cnt; i++) if (list->list[i]) { free (list->list[i]); list->list[i] = NULL; }
	list->cnt = 0;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_snaplist_copy (SPWAW_SNAPLIST *list, SPWAW_SNAPLIST *src)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	unsigned int	i;

	CSPWINIT;
	CNULLARG (list); CNULLARG (src);

	rc = SPWAW_snaplist_clear (list); ROE ("SPWAW_snaplist_clear()");

	for (i=0; i<src->cnt; i++) {
		rc = SPWAW_snaplist_addcpy (list, src->list[i]);
		ERRORGOTO("SPWAW_snaplist_addcpy()", handle_error);
	}

	return (SPWERR_OK);

handle_error:
	SPWAW_snaplist_clear (list);
	return (rc);
}
