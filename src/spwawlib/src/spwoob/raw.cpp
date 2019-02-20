/** \file
 * The SPWaW Library - SPWaW OOB handling - raw data handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "spwoob/spwoob.h"
#include "spwoob/raw.h"
#include "spwoob/raw_spwaw_api.h"
#include "common/internal.h"

SPWAW_ERROR
spwoob_load_raw_files (SPWOOB *oob)
{
	switch (oob->gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			return (spwoob_load_raw_spwaw_files (oob));
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			RWE (SPWERR_FAILED, "winSPWW2 game type not yet supported");
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			RWE (SPWERR_FAILED, "unsupported game type");
			break;
	}
}

SPWAW_ERROR
spwoob_load_raw_data (SPWOOB_DATA *dst)
{
	switch (dst->spwoob->gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			return (spwoob_load_raw_spwaw_data (dst));
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			RWE (SPWERR_FAILED, "winSPWW2 game type not yet supported");
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			RWE (SPWERR_FAILED, "unsupported game type");
			break;
	}
}

void
spwoob_dump_raw_data (SPWAW_GAME_TYPE gametype, void *rdata, BYTE id, char *base)
{
	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			spwoob_dump_raw_spwaw_data (rdata, id, base);
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			ERROR0 ("winSPWW2 game type not yet supported");
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			ERROR0 ("unsupported game type");
			break;
	}
}

static SPWAW_ERROR
spwoob_load_raw_file_core (SPWOOB *oob, BYTE id, const char *file,  ULONG rsize, load_raw_data_cb load)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWOOB_DATA	*dst;
	char		path[MAX_PATH+1];
	int		fd = -1;
	HANDLE		maph;
	void		*raw = NULL;

	CNULLARG (oob); CNULLARG (file);
	if (oob->data[id]) return (SPWERR_OK);

	dst = safe_malloc (SPWOOB_DATA); COOMGOTO (dst, "SPWOOB_DATA", handle_error);
	dst->id = id;

	dst->rdata = safe_smalloc (void, rsize); COOMGOTO (dst, "storage for raw OOB data", handle_error);
	dst->rsize = rsize;

	memset (path, 0, sizeof (path));
	snprintf (path, sizeof (path) - 1, "%s\\%s", oob->srcdir, file);

	fd = open (path, O_RDONLY|O_BINARY);
	if (fd < 0) FAILGOTO (SPWERR_FOFAILED, "open(oob file) failed", handle_error);

	raw = mmapfile (fd, &maph);
	if (!raw) FAILGOTO (SPWERR_FRFAILED, "mmapfile(oob file) failed", handle_error);

	memcpy (dst->rdata, raw, dst->rsize);
	unmmapfile ((void **)&raw, &maph); raw = NULL;
	close (fd); fd = -1;

	rc = load (dst); ERRORGOTO ("load_raw_data_cb()", handle_error);

	oob->data[id] = dst;
	oob->count++;

	return (SPWERR_OK);

handle_error:
	if (dst) {
		if (dst->rdata) safe_free (dst->rdata);
		safe_free (dst);
	}
	if (raw) unmmapfile ((void **)&raw, &maph);
	if (fd != -1) close (fd);
	return (rc);
}

SPWAW_ERROR
spwoob_load_raw_files_core (SPWOOB *oob, char *pattern, name2id_cb name2id, ULONG rsize, load_raw_data_cb load)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	char			glob[MAX_PATH+1];
	intptr_t		f_hndl = -1;
	int			f_stat;
	struct _finddata_t	f_data;

	CNULLARG (oob); CNULLARG(pattern);

	if (oob->count) return (SPWERR_OK);

	memset (glob, 0, sizeof (glob));
	snprintf (glob, sizeof (glob) - 1, "%s\\%s", oob->srcdir, pattern);

	if ((f_hndl = _findfirst (glob, &f_data)) == -1)
		FAILGOTO (SPWERR_NOOOBFILES, "_findfirst() failed", handle_error);

	f_stat = 0;
	while (f_stat != -1) {
		BYTE id = name2id (f_data.name);
		if ((id != SPWOOB_BADOOBID) && (oob->data[id] == NULL)) {
			rc = spwoob_load_raw_file_core (oob, id, f_data.name, rsize, load);
			ERRORGOTO ("spwoob_load_raw_file_core()", handle_error);
		}

		f_stat = _findnext (f_hndl, &f_data);
	}
	_findclose (f_hndl);

	if (oob->count == 0) RWE (SPWERR_NOOOBFILES, "no valid OOB files found");

	return (SPWERR_OK);

handle_error:
	if (f_hndl != -1) _findclose (f_hndl);
	return (rc);
}