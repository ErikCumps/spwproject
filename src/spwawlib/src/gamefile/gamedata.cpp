/** \file
 * The SPWaW Library - game data handling.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/gamedata.h"
#include "gamefile/spwaw/metadata_spwaw.h"
#include "gamefile/spwaw/gamedata_spwaw.h"
#include "gamefile/winspww2/metadata_winspww2.h"
#include "gamefile/winspww2/gamedata_winspww2.h" 
#include "gamefile/packing.h"
#include "common/internal.h"

static bool
gamedata_new_data (GAMEDATA *game)
{
	CWVNULL (game, false);

	switch (game->gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			gamedata_spwaw_new_metadata (game);
			game->data = gamedata_spwaw_new_data();
			return (true);
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			gamedata_winspww2_new_metadata (game);
			game->data = gamedata_winspww2_new_data();
			return (true);
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			ERROR0 ("unsupported game type");
			return (false);
			break;
	}
}

static void
gamedata_free_data (GAMEDATA *game)
{
	CWRNULL (game);

	switch (game->gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			gamedata_spwaw_free_metadata (&(game->metadata));
			gamedata_spwaw_free_data(&(game->data));
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			gamedata_winspww2_free_metadata (&(game->metadata));
			gamedata_winspww2_free_data(&(game->data));
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			ERROR0 ("unsupported game type");
			break;
	}
}

GAMEDATA *
gamedata_new (SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype)
{
	SPWAW_ERROR	rc;
	SECMAP		*map;
	GAMEDATA	*ptr;
	int		i;

	map = gamedata_SECMAP (gametype);
	if (!map) return (NULL);

	ptr = safe_malloc (GAMEDATA); COOMGOTO (ptr, "GAMEDATA", handle_error);
	clear_ptr (ptr);

	ptr->gametype = gametype;
	ptr->savetype = savetype;

	if (!gamedata_secmap_create (map, &(ptr->map)))
		FAILGOTO (SPWERR_FAILED, "failed to create section map", handle_error);

	if (!gamedata_new_data(ptr))
		FAILGOTO (SPWERR_FAILED, "failed to create data and metadata buffers", handle_error);

	for (i=0; i<ptr->map.cnt; i++) {
		DWORD	p = 0;
		if (ptr->map.list[i].size) {
			p = (DWORD)ptr->map.list[i].ptr + (DWORD)ptr->data;
		} else {
			ptr->map.list[i].freeme = true;
		}
		ptr->map.list[i].ptr = (void *)p;
	}

	return (ptr);

handle_error:
	if (ptr) {
		if (ptr->data) gamedata_free_data (ptr);
		if (ptr->map.cnt) gamedata_secmap_destroy (&(ptr->map));
		safe_free (ptr);
	}
	return (NULL);
}

void
gamedata_free (GAMEDATA **game)
{
	if (!game || !*game) return;

	gamedata_secmap_destroy (&((*game)->map));
	gamedata_free_data (*game);
	safe_free (*game); *game = NULL;
}

bool
gamedata_load_metadata (GAMEFILE *file, GAMEDATA *dst)
{
	CWVNULL (file, false); CWVNULL (dst, false);

	switch (file->gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			return (gamedata_spwaw_load_metadata (file, &(dst->metadata)));
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			return (gamedata_winspww2_load_metadata (file, &(dst->metadata)));
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			ERROR0 ("unsupported game type");
			return (false);
			break;
	}
}

bool
gamedata_load_data (GAMEFILE *file, GAMEDATA *dst)
{
	return (load_packed (file, dst));
}

bool
gamedata_load_section (GAMEFILE *file, DWORD sec, void **dst, unsigned long *len)
{
	return (load_packed_section (file, sec, dst, len));
}

bool
gamedata_save_metadata (GAMEDATA *src, GAMEFILE *file)
{
	CWVNULL (src, false); CWVNULL (file, false);

	switch (file->gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			return (gamedata_spwaw_save_metadata (&(src->metadata), file));
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			return (gamedata_winspww2_save_metadata (&(src->metadata), file));
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			ERROR0 ("unsupported game type");
			return (false);
			break;
	}

}

bool
gamedata_save_data (GAMEDATA *src, GAMEFILE *file)
{
	return (save_packed (src, file));
}
