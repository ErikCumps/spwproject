/** \file
 * The SPWaW Library - gamefile handling - SPWaW game data.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamedata.h"
#include "gamefile/spwaw/gamedata_spwaw.h"
#include "utils/ud.h"
#include "common/internal.h"

static SPWAW_ERROR
setup (SPWAW_SNAP_MAP_RAW *map)
{
	int	cnt;

	CNULLARG (map);

	map->data = NULL;

	cnt = map->width * map->height;
	if (!cnt) RWE (SPWERR_FAILED, "zero map width and/or height!");

	map->data = safe_nmalloc (SPWAW_SNAP_MAP_DRAW, cnt); COOM (map->data, "SPWAW_SNAP_MAP_DRAW array");
	map->size = cnt * sizeof (SPWAW_SNAP_MAP_DRAW);

	return (SPWERR_OK);
}

static void
raw2tfs (SPWAW_TFS &tfs, SPWAW_TFSBITS1 tfsbits1, SPWAW_TFSBITS2 tfsbits2, SPWAW_TFSBITS3 tfsbits3, SPWAW_TFSBITS4 tfsbits4)
{
	tfs.tfs.field		= tfsbits1.has_field;
	tfs.tfs.slope		= tfsbits1.has_slope;
	tfs.tfs.trees		= tfsbits1.has_trees;
	tfs.tfs.stream		= tfsbits1.has_stream;
	tfs.tfs.building_stone	= tfsbits1.has_bld_stone;
	tfs.tfs.building_wood	= tfsbits1.has_bld_wood;
	tfs.tfs.road1		= tfsbits1.has_road1;
	tfs.tfs.road2		= tfsbits1.has_road2;

	tfs.tfs.bridge_wood	= tfsbits2.has_brg_wood;
	tfs.tfs.bridge_stone	= tfsbits2.has_brg_stone;
	tfs.tfs.swamp		= tfsbits2.has_swamp;
	tfs.tfs.water		= tfsbits2.has_water;
	tfs.tfs.rough		= tfsbits2.has_rough;

	tfs.tfs.railroad	= tfsbits3.has_railroad;
	//tfs.tfs.water_shallow	= tfsbits3.has_shwater;		// fixme: this doesn't seem to be right?
	tfs.tfs.water_deep	= tfsbits3.has_dpwater;
	tfs.tfs.orchard		= tfsbits3.has_orchard;

	tfs.tfs.wall		= tfsbits4.has_wall;
	tfs.tfs.path		= tfsbits4.has_path;
	tfs.tfs.bocage		= tfsbits4.has_bocage;
}

SPWAW_ERROR
section08_spwaw_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB * /*stab*/)
{
	SPWAW_SNAP_MAP_RAW	*mp;
	DWORD			x, y;

	CNULLARG (src); CNULLARG (dst);

	mp   = &(dst->raw.game.map);

	setup (mp);
        for (x=0; x<mp->width; x++) {
                for (y=0; y<mp->height; y++) {
			mp->data[y*mp->width+x].height     = GDSPWAW(src)->sec08.u.d.terrain[x][y].height;
			mp->data[y*mp->width+x].has_T1     = GDSPWAW(src)->sec08.u.d.terrain[x][y].uT1.has_T1;
			mp->data[y*mp->width+x].has_T2     = GDSPWAW(src)->sec08.u.d.terrain[x][y].uT2.has_T2;
			mp->data[y*mp->width+x].has_T3     = GDSPWAW(src)->sec08.u.d.terrain[x][y].uT3.has_T3;
			mp->data[y*mp->width+x].has_T4     = GDSPWAW(src)->sec08.u.d.terrain[x][y].uT4.has_T4;
			raw2tfs (mp->data[y*mp->width+x].tfs,
				GDSPWAW(src)->sec08.u.d.terrain[x][y].uT1.bits1,
				GDSPWAW(src)->sec08.u.d.terrain[x][y].uT2.bits2,
				GDSPWAW(src)->sec08.u.d.terrain[x][y].uT3.bits3,
				GDSPWAW(src)->sec08.u.d.terrain[x][y].uT4.bits4
				);
                }
        }

	return (SPWERR_OK);
}
