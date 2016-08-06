#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamefile.h"
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

SPWAW_ERROR
sec08_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB * /*stab*/)
{
	SPWAW_SNAP_MAP_RAW	*mp;
	DWORD			x, y;

	CNULLARG (src); CNULLARG (dst);

	mp   = &(dst->raw.game.map);

	setup (mp);
        for (x=0; x<mp->width; x++) {
                for (y=0; y<mp->height; y++) {
			mp->data[y*mp->width+x].height = src->sec08.u.d.terrain[x][y].height;
			mp->data[y*mp->width+x].has_T1 = src->sec08.u.d.terrain[x][y].uT1.has_T1;
			mp->data[y*mp->width+x].has_T2 = src->sec08.u.d.terrain[x][y].uT2.has_T2;
			mp->data[y*mp->width+x].has_T3 = src->sec08.u.d.terrain[x][y].uT3.has_T3;
			mp->data[y*mp->width+x].has_T4 = src->sec08.u.d.terrain[x][y].uT4.has_T4;
			mp->data[y*mp->width+x].conn_road1 = src->sec08.u.d.terrain[x][y].uR1.conn_road1;
			mp->data[y*mp->width+x].conn_road2 = src->sec08.u.d.terrain[x][y].uR2.conn_road2;
			mp->data[y*mp->width+x].conn_rail  = src->sec08.u.d.terrain[x][y].uRR.conn_rail;
                }
        }

	return (SPWERR_OK);
}
