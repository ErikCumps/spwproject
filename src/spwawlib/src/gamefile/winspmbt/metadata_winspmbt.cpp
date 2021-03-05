/** \file
 * The SPWaW Library - game data handling - winSPMBT meta data.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/winspmbt/metadata_winspmbt.h"
#include "gamefile/winspmbt/cmt_winspmbt.h"
#include "common/internal.h"

static bool
gamedata_winspmbt_new_metadata_core (METADATA *data)
{
	return (gamedata_init_winspmbt_cmt (data));
}

bool
gamedata_winspmbt_new_metadata (GAMEDATA *game)
{
	if (!game) return (false);
	return (gamedata_winspmbt_new_metadata_core (&(game->metadata)));
}

bool
gamedata_winspmbt_new_metadata (GAMEFILE *file, METADATA *data)
{
	if (!file || !data) return (false);
	return (gamedata_winspmbt_new_metadata_core (data));
}

void
gamedata_winspmbt_free_metadata (METADATA *data)
{
	if (!data) return;

	if (data->data) safe_free (data->data);
	clear_ptr (data);
}

bool
gamedata_winspmbt_load_metadata (GAMEFILE *file, METADATA *dst)
{
	return (gamedata_load_winspmbt_cmt (file, dst));
}

bool
gamedata_winspmbt_save_metadata (METADATA *src, GAMEFILE *file)
{
	return (gamedata_save_winspmbt_cmt (src, file));
}
