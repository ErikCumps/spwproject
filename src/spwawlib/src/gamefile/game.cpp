/** \file
 * The SPWaW Library - savegame handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/game.h"
#include "gamefile/spwaw/game_spwaw.h"
#include "common/internal.h"

GAMEDATA *
game_load_full (SPWAW_GAME_TYPE gametype, const char *dir, unsigned int id, GAMEINFO *info)
{
	GAMEFILE	game;
	GAMEDATA	*data = NULL;
	bool		cmt_rc, dat_rc;

	log ("game_load_full (gametype=\"%s\" (%d), dir=\"%s\", id=%u)\n", SPWAW_gametype2str(gametype), gametype, dir, id);

	if (info) clear_ptr (info);

	if (!gamefile_open (gametype, dir, id, &game)) return (NULL);

	data = gamedata_new(gametype); COOMRET (data, "GAMEDATA", NULL);

	cmt_rc = gamedata_load_cmt (&game, &(data->cmt));
	dat_rc = gamedata_load_all (&game, data);

	if (!cmt_rc) {
		ERROR0 ("failed to read game comment data");
		gamedata_free (&data);
	} else if (!dat_rc) {
		ERROR0 ("failed to decompress game data");
		gamedata_free (&data);
	}

	if (data && info) {
		switch (gametype) {
			case SPWAW_GAME_TYPE_SPWAW:
				setup_spwaw_info (info, &game, data);
				break;
			case SPWAW_GAME_TYPE_WINSPWW2:
			case SPWAW_GAME_TYPE_UNKNOWN:
			default:
				ERROR0 ("unsupported game type");
				break;
		}
		data->type = info->type;
	}

	gamefile_close (&game);

	return (data);
}

bool
game_load_info (SPWAW_GAME_TYPE gametype, const char *dir, unsigned int id, GAMEINFO *info)
{
	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			return (game_load_spwaw_info(dir, id, info));
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			ERROR0 ("unsupported game type");
			return (false);
			break;
	}
}

bool
game_save_full (GAMEDATA *src, const char *dir, unsigned int id)
{
	GAMEFILE	game;
	bool		cmt_rc, dat_rc;

	log ("game_save_full (src=0x%8.8x, dir=\"%s\", id=%u)\n", src, dir, id);

	if (!gamefile_create (src->gametype, dir, id, &game)) return false;

	cmt_rc = gamedata_save_cmt (&(src->cmt), &game);
	dat_rc = gamedata_save_all (src, &game);

	gamefile_close (&game);

	if (!cmt_rc) {
		ERROR0 ("failed to write game comment data");
		return (false);
	} else if (!dat_rc) {
		ERROR0 ("failed to write game data");
		return (false);
	}

	return (true);
}
