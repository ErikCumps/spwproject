/** \file
 * The SPWaW Library - game data handling - section map.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/gamedata_secmap.h"
#include "gamefile/spwaw/gamedata_spwaw.h"
#include "common/internal.h"

SECMAP *
gamedata_SECMAP (SPWAW_GAME_TYPE gametype)
{
	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			return (gamedata_spwaw_SECMAP());
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			ERROR0 ("unsupported game type");
			return (NULL);
			break;
	}
}

bool
gamedata_secmap_create(SECMAP *MAP, SECMAP *map)
{
	SPWAW_ERROR	rc;

	if (!MAP || !map) return (false);

	map->cnt = MAP->cnt;
	map->list = safe_nmalloc (SECMAPEL, map->cnt); COOMGOTO (map->list, "SECMAPEL list", handle_error);
	for (int i=0; i<map->cnt; i++) map->list[i] = MAP->list[i];

	return (true);

handle_error:
	return (false);
}

bool
gamedata_secmap_destroy(SECMAP *map)
{
	if (!map) return (false);

	for (int i=0; i<map->cnt; i++){
		if (map->list[i].freeme) safe_free (map->list[i].ptr);
	}
	safe_free (map->list);
	memset (map, 0, sizeof (*map));

	return (true);
}

SECMAPEL *
gamedata_section (SECMAP *map, int idx)
{
	SECMAPEL	*sp = NULL;
	int		i;

	CWVNULL (map, NULL);

	for (i=0; i<map->cnt; i++) {
		if (map->list[i].idx == idx) {
			sp = &(map->list[i]);
			break;
		}
	}
	return (sp);
}
