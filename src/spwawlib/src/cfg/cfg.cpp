/** \file
 * The SPWaW Library - internal runtime configuration handling.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "spwawlib.h"
#include "common/internal.h"

#define	OOBLISTINC	4

CFG	cfg = { 0 };

static OOBENTRY *
lookup_oobentry (char *oobdir)
{
	if (!oobdir) return (NULL);

	for (unsigned int i=0; i<cfg.oob_cnt; i++) {
		if (strcmp (cfg.oob_lst[i].oobdir, oobdir) == 0) {
			return (&(cfg.oob_lst[i]));
		}
	}
	return (NULL);
}

static bool
path_filetime (char *path, FILETIME *filetime)
{
	HANDLE	h;
	bool	b;

	if (!path || !filetime) return (false);
	filetime = 0;

	h = CreateFile (path, 0, 0, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS , NULL);
	if (h == INVALID_HANDLE_VALUE) return (false);

	b = GetFileTime (h, NULL, NULL, filetime) != 0;

	CloseHandle (h);

	return (b);
}

static SPWAW_ERROR
oobentry (char *oobdir, SPWAW_GAME_TYPE gametype)
{
	SPWAW_ERROR	rc;
	FILETIME	dirdate;
	OOBENTRY	*oob = NULL;
	bool		update = false;
	OOBENTRY	*l = NULL;
	unsigned int	lidx = SPWAW_BADLONGIDX;

	CNULLARG (oobdir);

	if (!path_filetime (oobdir, &dirdate)) return (SPWERR_NOOOBFILES);

	oob = lookup_oobentry (oobdir);
	if (oob) {
		if (CompareFileTime (&(dirdate), &(oob->dirdate)) > 0) {
			update = true;
		} else {
			return (SPWERR_OK);
		}
	}

	if (!oob) {
		if (cfg.oob_cnt >= cfg.oob_len) {
			cfg.oob_len += OOBLISTINC;
			l = safe_nmalloc (OOBENTRY, cfg.oob_len); COOMGOTO (l, "OOBENTRY list", handle_error);

			if (cfg.oob_lst) {
				memcpy (l, cfg.oob_lst, cfg.oob_cnt * sizeof (OOBENTRY));
				safe_free (cfg.oob_lst);
			}
			cfg.oob_lst = l;
		}
		lidx = cfg.oob_cnt++;
		oob = &(cfg.oob_lst[lidx]);
	}

	if (update) {
		if (oob->oobptr) SPWOOB_free (&(oob->oobptr));
	} else {
		oob->oobdir = safe_strdup (oobdir);
		COOMGOTO (oob->oobdir, "OOBENTRY oobdir", handle_error);
	}

	rc = SPWOOB_new (&(oob->oobptr));
	ERRORGOTO ("spwoob_new()", handle_error);

	rc = SPWOOB_load (oob->oobptr, gametype, oob->oobdir);
	ERRORGOTO ("spwoob_load()", handle_error);

	oob->dirdate = dirdate;

	return (SPWERR_OK);

handle_error:
	if (oob->oobptr) SPWOOB_free (&(oob->oobptr));
	if (oob->oobdir) safe_free (oob->oobdir);
	if (lidx != SPWAW_BADLONGIDX) cfg.oob_cnt--;
	return (rc);
}

SPWAW_ERROR
cfg_set (int cnt, SPWAW_OOBCFG *list, bool withUD)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	cfg_free ();

	for (int i=0; i<cnt; i++){
		if (list[i].oobdir && strlen(list[i].oobdir)) {
			char	path[MAX_PATH+1];

			memset (path, 0, sizeof (path));
			_fullpath (path, list[i].oobdir, sizeof (path)-1);

			rc = oobentry (path, list[i].gametype);
			ERRORGOTO ("oobentry()", handle_error);
		}
	}

	cfg.withUD = withUD;

	cfg.init = true;
	return (SPWERR_OK);

handle_error:
	cfg_free ();
	return (rc);
}

void
cfg_free (void)
{
	for (unsigned int i=0; i<cfg.oob_cnt; i++) {
		if (cfg.oob_lst[i].oobptr) SPWOOB_free (&(cfg.oob_lst[i].oobptr));
		if (cfg.oob_lst[i].oobdir) safe_free (cfg.oob_lst[i].oobdir);
	}
	safe_free (cfg.oob_lst); cfg.oob_len = cfg.oob_cnt = 0;
	cfg.init = false;
}

extern SPWAW_ERROR
cfg_add_oobdir (const char *oobdir, SPWAW_GAME_TYPE gametype)
{
	SPWAW_ERROR	rc;
	char		path[MAX_PATH+1];

	memset (path, 0, sizeof (path));
	_fullpath (path, oobdir, sizeof (path)-1);

	rc = oobentry (path, gametype);	ROE ("oobentry()");

	return (SPWERR_OK);
}

extern SPWOOB *
cfg_oobptr (const char *oobdir)
{
	OOBENTRY	*oobentry;
	char		path[MAX_PATH+1];

	if (!oobdir) return (NULL);

	memset (path, 0, sizeof (path));
	_fullpath (path, oobdir, sizeof (path)-1);

	oobentry = lookup_oobentry (path);
	if (!oobentry) return (NULL);

	return (oobentry->oobptr);
}
