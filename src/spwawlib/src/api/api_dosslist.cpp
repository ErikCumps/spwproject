/** \file
 * The SPWaW Library - dossier list API implementation.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_dosslist.h>
#include "common/internal.h"

#define	LISTINC	8

static bool
file_on_list (SPWAW_DOSSLIST *ignore, SPWAW_DOSSLIST_NODE *node)
{
	unsigned long		i;
	SPWAW_DOSSLIST_NODE	*p = NULL;

	if (!ignore) return (false);

	for (i=0; i<ignore->cnt; i++) {
		p = ignore->list[i];

		if (stricmp (p->filepath, node->filepath) == 0) {
			if (CompareFileTime (&(p->filedate), &(node->filedate)) >= 0) break;
			break;
		}

		p = NULL;
	}
	return (p != NULL);
}

static bool
handle_file (const char *dir, WIN32_FIND_DATA f, SPWAW_DOSSLIST *ignore, SPWAW_DOSSLIST_NODE **p)
{
	SPWAW_ERROR		rc;
	SPWAW_DOSSLIST_NODE	*ptr = NULL;
	SYSTEMTIME		mt_utc, mt_local;

	if (!p) return (false); *p = NULL;

	ptr = safe_malloc (SPWAW_DOSSLIST_NODE); COOMGOTO (ptr, "SPWAW_DOSSLIST_NODE", handle_error);

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
	rc = SPWAW_dossier_info (ptr->filepath, &(ptr->info));
	if (rc != SPWERR_OK) goto handle_error;

	/* skip file if on ignore list */
	if (file_on_list (ignore, ptr)) goto handle_error;

	*p = ptr;
	return (true);

handle_error:
	if (ptr) free (ptr);
	return (false);
}

static void
list_files (const char *dir, SPWAW_DOSSLIST *ignore, SPWAW_DOSSLIST *list)
{
	char			buffer[MAX_PATH+1];
	HANDLE			h;
	WIN32_FIND_DATA		f;
	BOOL			stop = false;
	SPWAW_DOSSLIST_NODE	*p;

	memset (buffer, 0, sizeof (buffer));
	snprintf (buffer, sizeof (buffer) - 1, "%s\\*.warcab", dir);

	h = FindFirstFile (buffer, &f);
	if (h == INVALID_HANDLE_VALUE) return;	// TODO: flag error!

	while (!stop) {
		if (handle_file (dir, f, ignore, &p)) {
			/* put on list */
			SPWAW_dosslist_add (list, p);
		}

		stop = !FindNextFile (h, &f);
	}
	FindClose (h);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dosslist (const char *dir, SPWAW_DOSSLIST *ignore, SPWAW_DOSSLIST **list)
{
	SPWAW_DOSSLIST	*p = NULL;

	CSPWINIT;
	CNULLARG (dir); CNULLARG (list);
	*list = NULL;

	p = safe_malloc (SPWAW_DOSSLIST); COOM (p, "SPWAW_DOSSLIST");

	list_files (dir, ignore, p);

	*list = p;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dosslist_new (SPWAW_DOSSLIST **list)
{
	SPWAW_DOSSLIST	*p = NULL;

	CSPWINIT;
	CNULLARG (list);
	*list = NULL;

	p = safe_malloc (SPWAW_DOSSLIST); COOM (p, "SPWAW_DOSSLIST");

	*list = p;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dosslist_free (SPWAW_DOSSLIST **list)
{
	SPWAW_DOSSLIST	*l;

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
SPWAW_dosslist_add (SPWAW_DOSSLIST *list, SPWAW_DOSSLIST_NODE *node)
{
	SPWAW_DOSSLIST_NODE	**p;
	unsigned long		idx;

	CSPWINIT;
	CNULLARG (list); CNULLARG (node);

	if (list->cnt >= list->len) {
		list->len += LISTINC;
		p = safe_nmalloc (SPWAW_DOSSLIST_NODE *, list->len); COOM (p, "SPWAW_DOSSLIST_NODE LIST");

		if (list->list) {
			memcpy (p, list->list, list->cnt * sizeof (SPWAW_DOSSLIST_NODE *));
			free (list->list);
		}
		list->list = p;
	}
	idx = list->cnt++;
	list->list[idx] = node;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dosslist_addcpy (SPWAW_DOSSLIST *list, SPWAW_DOSSLIST_NODE *node)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	SPWAW_DOSSLIST_NODE	*p = NULL;

	CSPWINIT;
	CNULLARG (list); CNULLARG (node);

	p = safe_malloc (SPWAW_DOSSLIST_NODE); COOM (p, "SPWAW_DOSSLIST_NODE");

	memcpy (p, node, sizeof (*p));
	rc = SPWAW_dosslist_add (list, p);
	if (rc != SPWERR_OK) free (p);

	return (rc);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dosslist_clear (SPWAW_DOSSLIST *list)
{
	unsigned int	i;

	CSPWINIT;
	CNULLARG (list);

	for (i=0; i<list->cnt; i++) if (list->list[i]) { free (list->list[i]); list->list[i] = NULL; }
	list->cnt = 0;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dosslist_copy (SPWAW_DOSSLIST *list, SPWAW_DOSSLIST *src)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	unsigned int	i;

	CSPWINIT;
	CNULLARG (list); CNULLARG (src);

	rc = SPWAW_dosslist_clear (list); ROE ("SPWAW_dosslist_clear()");

	for (i=0; i<src->cnt; i++) {
		rc = SPWAW_dosslist_addcpy (list, src->list[i]);
		ERRORGOTO("SPWAW_dosslist_addcpy()", handle_error);
	}

	return (SPWERR_OK);

handle_error:
	SPWAW_dosslist_clear (list);
	return (rc);
}
