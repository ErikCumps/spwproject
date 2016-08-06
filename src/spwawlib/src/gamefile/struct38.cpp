#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamefile.h"
#include "common/internal.h"

SPWAW_ERROR
sec38_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB * /*stab*/)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	MAP_WIDTH		*data;
	SPWAW_SNAP_MAP_RAW	*map;

	CNULLARG (src); CNULLARG (dst);

	data = &(src->sec38.u.d.data);
	map  = &(dst->raw.game.map);

	map->width = 0;

	if (data->size <= MAPWIDTH)
		map->width = data->size;
	else {
		ERROR1 ("invalid game map width %lu", data->size);
		rc = SPWERR_BADSAVEDATA;
	}

	return (rc);
}