/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/gamefile.h"
#include "common/internal.h"

bool
gamefile_info (SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype, GAMEFILE_META *meta)
{
	if (!meta) return (false);
	meta->base = meta->ext_data = meta->ext_metadata = NULL;

	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			switch (savetype) {
				case SPWAW_SAVE_TYPE_REGULAR:
					meta->base         = "save";
					meta->ext_data     = "dat";
					meta->ext_metadata = "cmt";
					break;
				case SPWAW_SAVE_TYPE_MEGACAM:
					meta->base         = NULL;
					meta->ext_data     = "sav";
					meta->ext_metadata = "cam";
					break;
				default:
					/* unsupported save type */
					break;
			}
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			switch (savetype) {
				case SPWAW_SAVE_TYPE_REGULAR:
					meta->base         = "SpSv";
					meta->ext_data     = "dat";
					meta->ext_metadata = "cmt";
					break;
				default:
					/* unsupported save type */
					break;
			}
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			RWE (false, "unsupported game type");
			break;
	}

	return (true);
}

bool
gamefile_basename (SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype, char *filename, char *dst, unsigned int len)
{
	GAMEFILE_META	meta;
	size_t		todo;
	char		*p;

	if (!filename) return (false);
	if (!dst || !len) return (false);
	memset (dst, 0, len);
	todo = strlen(filename) + 1;

	if (!gamefile_info (gametype, savetype, &meta)) return (false);
	p = strrchr (filename, '.');
	if (p) {
		if (memcmp (p+1, meta.ext_data, strlen(meta.ext_data)+1) != 0) return (false);
		todo -= (strlen(meta.ext_data)+1);
	}
	if (todo > len) return (false);
	memcpy (dst, filename, todo);

	return (true);
}

static void
gamefile_file_init (GAMEFILE_FILE &file)
{
	clear_ptr (&file);
	file.fd = -1;
}

static void
gamefile_file_init (GAMEFILE_FILE &file, char *name, const char *description)
{
	clear_ptr (&file);
	file.fd = -1;
	snprintf (file.name, sizeof(file.name) - 1, "%s", name);
	snprintf (file.description, sizeof (file.description) - 1, "game %s filename", description);
}

static SPWAW_ERROR
gamefile_file_open (GAMEFILE_FILE &file, const char *goal, int mode, int perm)
{
	const char	*fmt = "failed to open %s for %s";
	char		buf[256];
	HANDLE		hfd;

	if (perm == -1) {
		file.fd = open (file.name, mode);
	} else {
		file.fd = open (file.name, mode, perm);
	}
	if (file.fd == -1) {
		memset (buf, 0, sizeof(buf));
		snprintf (buf, sizeof(buf) - 1, fmt, file.description, goal);
		RWE (SPWERR_FOFAILED, buf);
	}
	log ("gamefile_file_open: name=\"%s\", description=\"%s\", fd=%d\n", file.name, file.description, file.fd);

	hfd = (void *)_get_osfhandle (file.fd);
	if (hfd != INVALID_HANDLE_VALUE) GetFileTime (hfd, NULL, NULL, &(file.date));

	return (SPWERR_OK);
}

static void
gamefile_file_close (GAMEFILE_FILE &file)
{
	if (file.fd != -1) close (file.fd);
	gamefile_file_init (file);
}

static void
gamefile_init (GAMEFILE *game, SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype)
{
	if (!game) return;

	clear_ptr (game);
	game->gametype = gametype;
	game->savetype = savetype;
}

static bool
gamefile_prepare (SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEFILE *game)
{
	GAMEFILE_META	gfm;
	char		name[MAX_PATH+1];

	if (!sgd || !game) return (false);
	if (!sgd->path) return (false);
	if (!gamefile_info (sgd->gametype, sgd->savetype, &gfm)) return (false);

	gamefile_init (game, sgd->gametype, sgd->savetype);

	if (sgd->numeric_id) {
		memset (name, 0, sizeof (name));
		snprintf (name, sizeof (name) - 1, "%s\\%s%03.3d.%s", sgd->path, gfm.base, sgd->id.number, gfm.ext_metadata);
		gamefile_file_init (game->metadata, name, "metadata");

		memset (name, 0, sizeof (name));
		snprintf (name, sizeof (name) - 1, "%s\\%s%03.3d.%s", sgd->path, gfm.base, sgd->id.number, gfm.ext_data);
		gamefile_file_init (game->data, name, "data");
	} else {
		memset (name, 0, sizeof (name));
		snprintf (name, sizeof (name) - 1, "%s\\%s.%s", sgd->path, sgd->id.name, gfm.ext_metadata);
		gamefile_file_init (game->metadata, name, "metadata");

		memset (name, 0, sizeof (name));
		snprintf (name, sizeof (name) - 1, "%s\\%s.%s", sgd->path, sgd->id.name, gfm.ext_data);
		gamefile_file_init (game->data, name, "data");
	}

	return (true);
}

bool
gamefile_open (SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEFILE *game)
{
	SPWAW_ERROR	rc;

	if (!gamefile_prepare (sgd, game)) return (false);

	rc = gamefile_file_open (game->metadata, "reading", O_RDONLY|O_BINARY, -1);
	ERRORGOTO ("gamefile_file_open(metadata)", error);

	rc = gamefile_file_open (game->data, "reading", O_RDONLY|O_BINARY, -1);
	ERRORGOTO ("gamefile_file_open(data)", error);

	return (true);

error:
	gamefile_close (game);
	return (false);
}

bool
gamefile_create (SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEFILE *game)
{
	SPWAW_ERROR	rc;

	if (!gamefile_prepare (sgd, game)) return (false);

	rc = gamefile_file_open (game->metadata, "writing", O_WRONLY|O_BINARY|O_CREAT|O_TRUNC, 0666);
	ERRORGOTO ("gamefile_file_open(metadata)", error);

	rc = gamefile_file_open (game->data, "writing", O_WRONLY|O_BINARY|O_CREAT|O_TRUNC, 0666);
	ERRORGOTO ("gamefile_file_open(data)", error);

	return (true);

error:
	gamefile_close (game);
	return (false);
}

bool
gamefile_close (GAMEFILE *game)
{
	if (!game) return (false);

	gamefile_file_close (game->metadata);
	gamefile_file_close (game->data);
	clear_ptr (game);
	gamefile_file_init (game->metadata);
	gamefile_file_init (game->data);

	return (true);
}
