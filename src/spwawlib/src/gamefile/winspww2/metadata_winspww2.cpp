/** \file
 * The SPWaW Library - game data handling - winSPWW2 meta data.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/winspww2/metadata_winspww2.h"
#include "gamefile/winspww2/cmt_winspww2.h"
#include "common/internal.h"

static bool
gamedata_winspww2_new_metadata_core (METADATA *data)
{
	return (gamedata_init_winspww2_cmt (data));
}

bool
gamedata_winspww2_new_metadata (GAMEDATA *game)
{
	if (!game) return (false);
	return (gamedata_winspww2_new_metadata_core (&(game->metadata)));
}

bool
gamedata_winspww2_new_metadata (GAMEFILE *file, METADATA *data)
{
	if (!file || !data) return (false);
	return (gamedata_winspww2_new_metadata_core (data));
}

void
gamedata_winspww2_free_metadata (METADATA *data)
{
	if (!data) return;

	if (data->data) safe_free (data->data);
	clear_ptr (data);
}

bool
gamedata_winspww2_load_metadata (GAMEFILE *file, METADATA *dst)
{
	return (gamedata_load_winspww2_cmt (file, dst));
}

bool
gamedata_winspww2_save_metadata (METADATA *src, GAMEFILE *file)
{
	return (gamedata_save_winspww2_cmt (src, file));
}
