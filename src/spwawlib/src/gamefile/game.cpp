/** \file
 * The SPWaW Library - savegame handling.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/game.h"
#include "gamefile/spwaw/game_spwaw.h"
#include "gamefile/winspww2/game_winspww2.h"
#include "common/internal.h"

static void
log_savegame_descriptor (SPWAW_SAVEGAME_DESCRIPTOR *sgd, const char *caller)
{
	if (sgd->numeric_id) {
		log ("%s (sgd = {gametype=\"%s\", savetype=\"%s\", path=\"%s\", numeric_id=true, id=%u})\n",
			caller,
			SPWAW_gametype2str(sgd->gametype), SPWAW_savetype2str(sgd->savetype),
			sgd->path, sgd->id.number);
	} else {
		log ("%s (sgd = {gametype=\"%s\", savetype=\"%s\", path=\"%s\", numeric_id=false, id=\"%s\"})\n",
			caller,
			SPWAW_gametype2str(sgd->gametype), SPWAW_savetype2str(sgd->savetype),
			sgd->path, sgd->id.name);
	}

}

GAMEDATA *
game_load_full (SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEINFO *info)
{
	GAMEFILE	game;
	GAMEDATA	*data = NULL;
	bool		mt_rc, dt_rc;

	if (!sgd) return (NULL);
	log_savegame_descriptor (sgd, __FUNCTION__);

	if (info) clear_ptr (info);

	if (!gamefile_open (sgd, &game)) return (NULL);

	data = gamedata_new (game.gametype, game.savetype); COOMRET (data, "GAMEDATA", NULL);

	mt_rc = gamedata_load_metadata (&game, data);
	dt_rc = gamedata_load_data (&game, data);

	if (!mt_rc) {
		ERROR0 ("failed to load game metadata");
		gamedata_free (&data);
	} else if (!dt_rc) {
		ERROR0 ("failed to load game data");
		gamedata_free (&data);
	}

	if (data && info) {
		switch (game.gametype) {
			case SPWAW_GAME_TYPE_SPWAW:
				setup_spwaw_info (info, &game, data);
				break;
			case SPWAW_GAME_TYPE_WINSPWW2:
				setup_winspww2_info (info, &game, data);
				break;
			case SPWAW_GAME_TYPE_UNKNOWN:
			default:
				ERROR0 ("unsupported game type");
				break;
		}
		data->savetype = info->savetype;
		data->type = info->type;
	}

	gamefile_close (&game);

	return (data);
}

bool
game_load_info (SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEINFO *info)
{
	if (!sgd || !info) return (false);
	log_savegame_descriptor (sgd, __FUNCTION__);

	switch (sgd->gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			return (game_load_spwaw_info(sgd, info));
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			return (game_load_winspww2_info(sgd, info));
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			ERROR0 ("unsupported game type");
			return (false);
			break;
	}
}

bool
game_save_full (GAMEDATA *src, SPWAW_SAVEGAME_DESCRIPTOR *sgd)
{
	GAMEFILE	game;
	bool		mt_rc, dt_rc;

	if (!src || !sgd) return (false);
	if (src->gametype != sgd->gametype) return (false);
	if (src->savetype != sgd->savetype) return (false);
	log_savegame_descriptor (sgd, __FUNCTION__);

	if (!gamefile_create (sgd, &game)) return false;

	mt_rc = gamedata_save_metadata (src, &game);
	dt_rc = gamedata_save_data (src, &game);

	gamefile_close (&game);

	if (!mt_rc) {
		ERROR0 ("failed to save game metadata");
		return (false);
	} else if (!dt_rc) {
		ERROR0 ("failed to save game data");
		return (false);
	}

	return (true);
}
