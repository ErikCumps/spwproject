/** \file
 * The SPWaW Library - internal runtime configuration handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "common/internal.h"

CFG	cfg = { 0 };

SPWAW_ERROR
cfg_set (SPWAW_GAME_TYPE gametype, const char *oobdir, bool withUD)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	cfg_free ();

	cfg.gametype = gametype;

	if (oobdir) {
		char	path[MAX_PATH+1];

		memset (path, 0, sizeof (path));
		_fullpath (path, oobdir, sizeof (path)-1);
		cfg.oobdir = strdup (path);
		COOMGOTO (cfg.oobdir, "CFG.oobdir", handle_error);

		rc = SPWOOB_new (&(cfg.oobptr));
		ERRORGOTO ("spwoob_new()", handle_error);

		rc = SPWOOB_load (cfg.oobptr, cfg.gametype, cfg.oobdir);
		ERRORGOTO ("spwoob_load()", handle_error);
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
	if (cfg.oobptr) {
		SPWOOB_free (&(cfg.oobptr));
	}

	if (cfg.oobdir) {
		safe_free (cfg.oobdir);
		cfg.oobdir = NULL;
	}
	cfg.init = false;
}
