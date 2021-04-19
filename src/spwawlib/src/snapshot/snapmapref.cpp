/** \file
 * The SPWaW Library - snapshot battle map reference handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "snapshot/snapshot.h"
#include "common/internal.h"

SPWAW_ERROR
snap_mapref_create (SPWAW_SNAPSHOT *snap, SPWAW_SNAP_MAP_RAW *raw, SPWAW_SNAP_MAP *map)
{
	CNULLARG (snap); CNULLARG (raw); CNULLARG (map);

	if (snap->raw.game.map.reference) RWE (SPWERR_FAILED, "snapshot already contains raw battle map reference");
	if (snap->game.map.reference) RWE (SPWERR_FAILED, "snapshot already contains battle map reference");

	snap->raw.game.map.reference = true;
	*raw = snap->raw.game.map;

	snap->game.map.reference = true;
	*map = snap->game.map;

	return (SPWERR_OK);
}

SPWAW_ERROR
snap_mapref_set (SPWAW_SNAP_MAP_RAW *raw, SPWAW_SNAP_MAP *map, SPWAW_SNAPSHOT *snap)
{
	CNULLARG (raw); CNULLARG (map); CNULLARG (snap);

	if (!snap->raw.game.map.reference) safe_free (snap->raw.game.map.data);
	snap->raw.game.map = *raw;

	if (!snap->game.map.reference) safe_free (snap->game.map.data);
	snap->game.map = *map;

	return (SPWERR_OK);
}
