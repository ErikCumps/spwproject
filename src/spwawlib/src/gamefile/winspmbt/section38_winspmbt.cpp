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
section38_winspmbt_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB * /*stab*/)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	WINSPMBT_MAP_WIDTH	*data;
	SPWAW_SNAP_MAP_RAW	*map;

	CNULLARG (src); CNULLARG (dst);

	data = &(GDWINSPMBT(src)->sec38.u.d.data);
	map  = &(dst->raw.game.map);

	map->width = 0;

	if (data->size <= WINSPMBT_MAPWIDTH)
		map->width = data->size;
	else {
		ERROR1 ("invalid game map width %lu", data->size);
		rc = SPWERR_BADSAVEDATA;
	}

	return (rc);
}
