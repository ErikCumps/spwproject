/** \file
 * The SPWaW Library - savegame descriptor.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/savegame_descriptor.h"
#include "common/internal.h"

static SPWAW_ERROR
savegame_descriptor_init_core (SPWAW_SAVEGAME_DESCRIPTOR &sgd, SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype, const char *oobdir, const char *path, STRTAB *stab)
{
	SPWAW_ERROR	rc;

	memset (&sgd, 0, sizeof (sgd));
	sgd.gametype	= gametype;
	sgd.savetype	= savetype;
	if (stab) {
		sgd.oobdir = STRTAB_add (stab, (char *)oobdir);
		if (!sgd.oobdir) FAILGOTO (SPWERR_FAILED, "STRTAB_add(oobdir)", handle_error);

		sgd.path = STRTAB_add (stab, (char *)path);
		if (!sgd.path) FAILGOTO (SPWERR_FAILED, "STRTAB_add(path)", handle_error);
	} else {
		/* An oobdir is optional in a savegame descriptor */
		if (oobdir) {
			sgd.oobdir = safe_strdup ((char *)oobdir);
			COOMGOTO (sgd.oobdir, "savegame descriptor oobdir", handle_error);
		}

		sgd.path = safe_strdup ((char *)path);
		COOMGOTO (sgd.path, "savegame descriptor path", handle_error);
	}

	return (SPWERR_OK);

handle_error:
	SPWAW_savegame_descriptor_clear (sgd);
	return (rc);
}

SPWAW_ERROR
savegame_descriptor_init (SPWAW_SAVEGAME_DESCRIPTOR &sgd, SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype, const char *oobdir, const char *path, unsigned int id, STRTAB *stab)
{
	SPWAW_ERROR	rc;

	rc = savegame_descriptor_init_core (sgd, gametype, savetype, oobdir, path, stab);
	ROE ("savegame_descriptor_init_core()");

	sgd.numeric_id	= true;
	sgd.id.number	= id;

	return (SPWERR_OK);
}

SPWAW_ERROR
savegame_descriptor_init (SPWAW_SAVEGAME_DESCRIPTOR &sgd, SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype, const char *oobdir, const char *path, const char * id, STRTAB *stab)
{
	SPWAW_ERROR	rc;

	rc = savegame_descriptor_init_core (sgd, gametype, savetype, oobdir, path, stab);
	ROE ("savegame_descriptor_init_core()");

	sgd.numeric_id	= false;
	if (stab) {
		sgd.id.name = STRTAB_add (stab, (char *)id);
		if (!sgd.id.name) FAILGOTO (SPWERR_FAILED, "STRTAB_add(id)", handle_error);
	} else {
		sgd.id.name = safe_strdup ((char *)id);
		COOMGOTO (sgd.id.name, "savegame descriptor ID name", handle_error);
	}

	return (SPWERR_OK);

handle_error:
	SPWAW_savegame_descriptor_clear (sgd);
	return (rc);
}

SPWAW_ERROR
savegame_descriptor_init (SPWAW_SAVEGAME_DESCRIPTOR &sgd, SPWAW_SAVEGAME_DESCRIPTOR &src, STRTAB *stab)
{
	SPWAW_ERROR	rc;

	rc = savegame_descriptor_init_core (sgd, src.gametype, src.savetype, src.oobdir, src.path, stab);
	ROE ("savegame_descriptor_init_core()");

	sgd.numeric_id = src.numeric_id;
	if (sgd.numeric_id) {
		sgd.id.number = src.id.number;
	} else {
		if (stab) {
			sgd.id.name = STRTAB_add (stab, (char *)src.id.name);
			if (!sgd.id.name) FAILGOTO (SPWERR_FAILED, "STRTAB_add(src.id.name)", handle_error);
		} else {
			sgd.id.name = safe_strdup ((char *)src.id.name);
			COOMGOTO (sgd.id.name, "savegame descriptor ID name", handle_error);
		}
	}

	return (SPWERR_OK);

handle_error:
	SPWAW_savegame_descriptor_clear (sgd);
	return (rc);
}

SPWAW_ERROR
savegame_descriptor_init (SPWAW_SAVEGAME_DESCRIPTOR &sgd, ULONG gametype, ULONG savetype, ULONG oobdir, ULONG path, ULONG id, STRTAB *stab)
{
	CNULLARG (stab);

	memset (&sgd, 0, sizeof (sgd));
	sgd.gametype	= (SPWAW_GAME_TYPE)gametype;
	sgd.savetype	= (SPWAW_SAVE_TYPE)savetype;
	sgd.oobdir	= STRTAB_getstr (stab, oobdir);
	sgd.path	= STRTAB_getstr (stab, path);
	sgd.numeric_id	= false;
	sgd.id.name	= STRTAB_getstr (stab, id);

	return (SPWERR_OK);
}

void
savegame_descriptor_clear (SPWAW_SAVEGAME_DESCRIPTOR &sgd, STRTAB *stab)
{
	if (sgd.oobdir) {
		if (stab) {
			STRTAB_del (stab, sgd.oobdir);
		} else {
			safe_free (sgd.oobdir);
		}
	}
	if (sgd.path) {
		if (stab) {
			STRTAB_del (stab, sgd.path);
		} else {
			safe_free (sgd.path);
		}
	}
	if (!sgd.numeric_id && sgd.id.name) {
		if (stab) {
			STRTAB_del (stab, sgd.id.name);
		} else {
			safe_free (sgd.id.name);
		}
	}
	memset (&sgd, 0, sizeof (sgd));
}
