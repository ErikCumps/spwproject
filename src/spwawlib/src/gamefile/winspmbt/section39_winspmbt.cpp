/** \file
 * The SPWaW Library - gamefile handling - winSPMBT game data.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamedata.h"
#include "gamefile/winspmbt/gamedata_winspmbt.h"
#include "common/internal.h"

SPWAW_ERROR
section39_winspmbt_save_snapshot  (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB * /*stab*/)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	WINSPMBT_MAP_HEIGHT	*data;
	SPWAW_SNAP_MAP_RAW	*map;

	CNULLARG (src); CNULLARG (dst);

	data = &(GDWINSPMBT(src)->sec39.u.d.data);
	map  = &(dst->raw.game.map);

	map->height = 0;

	if (data->size <= WINSPMBT_MAPHEIGHT)
		map->height = data->size;
	else {
		ERROR1 ("invalid game map height %lu", data->size);
		rc = SPWERR_BADSAVEDATA;
	}

	return (rc);
}
