/** \file
 * The SPWaW Library - savegame list API implementation.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_savelist.h>
#include "gamefile/gamefile.h"
#include "common/internal.h"

#define	LISTINC	8

static bool
id_from_name (char *name, unsigned int *id)
{
	if (!name || !id) return (false);

	*id = 0;
	return (sscanf (name, "save%u.dat", id) == 1);
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
handle_file (const char *dir, WIN32_FIND_DATA f, SPWAW_SAVELIST *ignore, SPWAW_SAVELIST_NODE **p)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	unsigned int		id;
	SPWAW_SAVELIST_NODE	*ptr = NULL;
	SYSTEMTIME		mt_utc, mt_local;
	GAMEINFO		info;

	if (!p) return (false); *p = NULL;

	/* skip file if no id detected */
	if (!id_from_name (f.cFileName, &id)) goto handle_error;

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
	if (game_load_info (dir, id, &info)) {
		memcpy (ptr->info.stamp, info.stamp, sizeof (info.stamp));
		memcpy (ptr->info.location, info.location, sizeof (info.location));
		memcpy (ptr->info.comment, info.comment, sizeof (info.comment));
		ptr->info.type = info.type;
	}

	/* skip file if on ignore list */
	if (file_on_list (ignore, ptr)) goto handle_error;

	*p = ptr;
	return (true);

handle_error:
	if (ptr) free (ptr);
	return (false);
}

static void
list_files (const char *dir, SPWAW_SAVELIST *ignore, SPWAW_SAVELIST *list)
{
	char			buffer[MAX_PATH+1];
	HANDLE			h;
	WIN32_FIND_DATA		f;
	BOOL			stop = false;
	SPWAW_SAVELIST_NODE	*p;

	memset (buffer, 0, sizeof (buffer));
	snprintf (buffer, sizeof (buffer) - 1, "%s\\*.dat", dir);

	h = FindFirstFile (buffer, &f);
	if (h == INVALID_HANDLE_VALUE) return;	// TODO: flag error!

	while (!stop) {
		if (handle_file (dir, f, ignore, &p)) {
			/* put on list */
			SPWAW_savelist_add (list, p);
		}

		stop = !FindNextFile (h, &f);
	}
	FindClose (h);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_savelist (const char *dir, SPWAW_SAVELIST *ignore, SPWAW_SAVELIST **list)
{
	SPWAW_SAVELIST	*p = NULL;

	CSPWINIT;
	CNULLARG (dir); CNULLARG (list);
	*list = NULL;

	p = safe_malloc (SPWAW_SAVELIST); COOM (p, "SPWAW_SAVELIST");

	list_files (dir, ignore, p);

	*list = p;

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

SPWAWLIB_API SPWAW_ERROR
SPWAW_savelist_add (SPWAW_SAVELIST *list, SPWAW_SAVELIST_NODE *node)
{
	SPWAW_SAVELIST_NODE	**p;
	unsigned long		idx;

	CSPWINIT;
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
