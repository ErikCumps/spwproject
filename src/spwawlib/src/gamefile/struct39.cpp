#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamefile.h"
#include "common/internal.h"

SPWAW_ERROR
sec39_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB * /*stab*/)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	MAP_HEIGHT		*data;
	SPWAW_SNAP_MAP_RAW	*map;

	CNULLARG (src); CNULLARG (dst);

	data = &(src->sec39.u.d.data);
	map  = &(dst->raw.game.map);

	map->height = 0;

	if (data->size <= MAPHEIGHT)
		map->height = data->size;
	else {
		ERROR1 ("invalid game map height %lu", data->size);
		rc = SPWERR_BADSAVEDATA;
	}

	return (rc);
}
