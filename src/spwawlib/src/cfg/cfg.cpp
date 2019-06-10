/** \file
 * The SPWaW Library - internal runtime configuration handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "spwawlib.h"
#include "common/internal.h"

CFG	cfg = { 0 };

SPWAW_ERROR
cfg_set (int cnt, SPWAW_OOBCFG *list, bool withUD)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	cfg_free ();

	cfg.oob_lst = safe_nmalloc (OOBCFG, cnt); COOMGOTO (cfg.oob_lst, "CFG.oob_lst", handle_error);
	cfg.oob_cnt = cnt;

	for (int i=0; i<cfg.oob_cnt; i++){
		cfg.oob_lst[i].gametype = list[i].gametype;

		if (list[i].oobdir) {
			char	path[MAX_PATH+1];

			memset (path, 0, sizeof (path));
			_fullpath (path, list[i].oobdir, sizeof (path)-1);
			cfg.oob_lst[i].oobdir = strdup (path);
			COOMGOTO (cfg.oob_lst[i].oobdir, "CFG.oob_lst[i].oobdir", handle_error);

			rc = SPWOOB_new (&(cfg.oob_lst[i].oobptr));
			ERRORGOTO ("spwoob_new()", handle_error);

			rc = SPWOOB_load (cfg.oob_lst[i].oobptr, cfg.oob_lst[i].gametype, cfg.oob_lst[i].oobdir);
			ERRORGOTO ("spwoob_load()", handle_error);
		}
	}

	cfg.withUD = withUD;

	cfg.init = true;
	return (SPWERR_OK);

handle_error:
	cfg_free ();
	return (rc);
}

const char *
cfg_oobdir (SPWAW_GAME_TYPE gametype)
{
	for (int i=0; i<cfg.oob_cnt; i++) {
		if (cfg.oob_lst[i].gametype == gametype) {
			return (cfg.oob_lst[i].oobdir);
			break;
		}
	}

	return (NULL);
}

SPWOOB *
cfg_oobptr (SPWAW_GAME_TYPE gametype)
{
	for (int i=0; i<cfg.oob_cnt; i++) {
		if (cfg.oob_lst[i].gametype == gametype) {
			return (cfg.oob_lst[i].oobptr);
			break;
		}
	}

	return (NULL);
}

void
cfg_free (void)
{
	for (int i=0; i<cfg.oob_cnt; i++) {
		if (cfg.oob_lst[i].oobptr) {
			SPWOOB_free (&(cfg.oob_lst[i].oobptr));
		}

		if (cfg.oob_lst[i].oobdir) {
			safe_free (cfg.oob_lst[i].oobdir);
		}
	}
	safe_free (cfg.oob_lst); cfg.oob_cnt = 0;
	cfg.init = false;
}
