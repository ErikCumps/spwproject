/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/gamefile.h"
#include "common/internal.h"

bool
gamefile_basename (SPWAW_GAME_TYPE type, const char **base)
{
	if (!base) return (false);
	*base = NULL;

	switch (type) {
		case SPWAW_GAME_TYPE_SPWAW:
			*base = "save";
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			*base = "SpSv";
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			ERROR0 ("unsupported game type");
			return (false);
			break;
	}

	return (true);
}

bool
gamefile_open (SPWAW_GAME_TYPE gametype, const char *dir, int id, GAMEFILE *game)
{
	SPWAW_ERROR	rc;
	const char	*base = NULL;
	char		name[MAX_PATH+1];
	HANDLE		hfd;

	if (!dir || !game) return (false);

	if (!gamefile_basename (gametype, &base)) return (false);

	clear_ptr (game);
	game->gametype = gametype;
	game->cmt_fd = game->dat_fd = -1;

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "%s\\%s%03.3d.cmt", dir, base, id);
	game->cmt_name = strdup (name); COOMGOTO (game->cmt_name, "game cmt filename", error);

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "%s\\%s%03.3d.dat", dir, base, id);
	game->dat_name = strdup (name); COOMGOTO (game->cmt_name, "game dat filename", error);

	game->cmt_fd = open (game->cmt_name, O_RDONLY|O_BINARY);
	if (game->cmt_fd == -1) FAILGOTO (SPWERR_FOFAILED, "failed to open game cmt file for reading", error);
	log ("gamefile_open: comment: name=\"%s\", fd=%d\n", game->cmt_name, game->cmt_fd);

	hfd = (void *)_get_osfhandle (game->cmt_fd);
	if (hfd != INVALID_HANDLE_VALUE) GetFileTime (hfd, NULL, NULL, &(game->cmt_date));

	game->dat_fd = open (game->dat_name, O_RDONLY|O_BINARY);
	if (game->dat_fd == -1) FAILGOTO (SPWERR_FOFAILED, "failed to open game dat file for reading", error);
	log ("gamefile_open: data: name=\"%s\", fd=%d\n", game->dat_name, game->dat_fd);

	hfd = (void *)_get_osfhandle (game->dat_fd);
	if (hfd != INVALID_HANDLE_VALUE)
		GetFileTime (hfd, NULL, NULL, &(game->dat_date));

	return (true);

error:
	if (game->dat_fd != -1) close (game->dat_fd);
	if (game->cmt_fd != -1) close (game->cmt_fd);
	if (game->dat_name) free (game->dat_name);
	if (game->cmt_name) free (game->cmt_name);
	clear_ptr (game);
	return (false);
}

bool
gamefile_create (SPWAW_GAME_TYPE gametype, const char *dir, int id, GAMEFILE *game)
{
	SPWAW_ERROR	rc;
	const char	*base = NULL;
	char		name[MAX_PATH+1];
	HANDLE		hfd;

	if (!dir || !game) return (false);

	if (!gamefile_basename (gametype, &base)) return (false);

	clear_ptr (game);
	game->gametype = gametype;
	game->cmt_fd = game->dat_fd = -1;

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "%s\\%s%03.3d.cmt", dir, base, id);
	game->cmt_name = strdup (name); COOMGOTO (game->cmt_name, "game cmt filename", error);

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "%s\\%s%03.3d.dat", dir, base, id);
	game->dat_name = strdup (name); COOMGOTO (game->cmt_name, "game dat filename", error);

	game->cmt_fd = open (game->cmt_name, O_WRONLY|O_BINARY|O_CREAT|O_TRUNC, 0666);
	if (game->cmt_fd == -1) FAILGOTO (SPWERR_FOFAILED, "failed to open game cmt file for writing", error);
	log ("gamefile_create: comment: name=\"%s\", fd=%d\n", game->cmt_name, game->cmt_fd);

	hfd = (void *)_get_osfhandle (game->cmt_fd);
	if (hfd != INVALID_HANDLE_VALUE) GetFileTime (hfd, NULL, NULL, &(game->cmt_date));

	game->dat_fd = open (game->dat_name, O_WRONLY|O_BINARY|O_CREAT|O_TRUNC, 0666);
	if (game->dat_fd == -1) FAILGOTO (SPWERR_FOFAILED, "failed to open game dat file for writing", error);
	log ("gamefile_create: data: name=\"%s\", fd=%d\n", game->dat_name, game->dat_fd);

	hfd = (void *)_get_osfhandle (game->dat_fd);
	if (hfd != INVALID_HANDLE_VALUE)
		GetFileTime (hfd, NULL, NULL, &(game->dat_date));

	return (true);

error:
	if (game->dat_fd != -1) close (game->dat_fd);
	if (game->cmt_fd != -1) close (game->cmt_fd);
	if (game->dat_name) free (game->dat_name);
	if (game->cmt_name) free (game->cmt_name);
	clear_ptr (game);
	return (false);
}

bool
gamefile_close (GAMEFILE *game)
{
	if (!game) return (false);

	if (game->dat_fd != -1) close (game->dat_fd);
	if (game->cmt_fd != -1) close (game->cmt_fd);
	if (game->dat_name) free (game->dat_name);
	if (game->cmt_name) free (game->cmt_name);
	clear_ptr (game);
	game->cmt_fd = game->dat_fd = -1;

	return (true);
}
