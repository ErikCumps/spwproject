/** \file
 * The SPWaW Library - SPWaW road connection handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/spwaw/road_connections_spwaw.h"
#include "gamefile/spwaw/gamedata_spwaw.h"
#include "common/internal.h"

SPWAW_ERROR
detect_spwaw_road_connections (GAMEDATA *src, SPWAW_SNAPSHOT *dst)
{
	SPWAW_SNAP_MAP_RAW	*mp;
	DWORD			x, y;

	CNULLARG (src); CNULLARG (dst);

	mp = &(dst->raw.game.map);

	for (x=0; x<mp->width; x++) {
		for (y=0; y<mp->height; y++) {
			mp->data[y*mp->width+x].conn_road1 = GDSPWAW(src)->sec08.u.d.terrain[x][y].uR1.conn_road1;
			mp->data[y*mp->width+x].conn_road2 = GDSPWAW(src)->sec08.u.d.terrain[x][y].uR2.conn_road2;
			mp->data[y*mp->width+x].conn_rail  = GDSPWAW(src)->sec08.u.d.terrain[x][y].uRR.conn_rail;
			mp->data[y*mp->width+x].conn_tram  = 0;
		}
	}

	return (SPWERR_OK);
}
