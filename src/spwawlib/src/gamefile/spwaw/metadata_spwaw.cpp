/** \file
 * The SPWaW Library - game data handling - SPWaW meta data.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/spwaw/metadata_spwaw.h"
#include "gamefile/spwaw/cmt_spwaw.h"
#include "gamefile/spwaw/megacam_spwaw.h"
#include "common/internal.h"

static bool
gamedata_spwaw_new_metadata_core (SPWAW_SAVE_TYPE savetype, METADATA *data)
{
	switch (savetype) {
		case SPWAW_SAVE_TYPE_REGULAR:
		default:
			return (gamedata_init_spwaw_cmt (data));
		case SPWAW_SAVE_TYPE_MEGACAM:
			return (gamedata_init_spwaw_megacam (data));
			break;
	}
}

bool
gamedata_spwaw_new_metadata (GAMEDATA *game)
{
	if (!game) return (false);
	return (gamedata_spwaw_new_metadata_core (game->savetype, &(game->metadata)));
}

bool
gamedata_spwaw_new_metadata (GAMEFILE *file, METADATA *data)
{
	if (!file || !data) return (false);
	return (gamedata_spwaw_new_metadata_core (file->savetype, data));
}

void
gamedata_spwaw_free_metadata (METADATA *data)
{
	if (!data) return;

	if (data->data) safe_free (data->data);
	clear_ptr (data);
}

bool
gamedata_spwaw_load_metadata (GAMEFILE *file, METADATA *dst)
{
	switch (file->savetype) {
		case SPWAW_SAVE_TYPE_REGULAR:
		default:
			return (gamedata_load_spwaw_cmt (file, dst));
		case SPWAW_SAVE_TYPE_MEGACAM:
			return (gamedata_load_spwaw_megacam (file, dst));
			break;
	}
}

bool
gamedata_spwaw_save_metadata (METADATA *src, GAMEFILE *file)
{
	switch (file->savetype) {
		case SPWAW_SAVE_TYPE_REGULAR:
		default:
			return (gamedata_save_spwaw_cmt (src, file));
		case SPWAW_SAVE_TYPE_MEGACAM:
			return (gamedata_save_spwaw_megacam (src, file));
			break;
	}
}