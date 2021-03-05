/** \file
 * The SPWaW Library - SPWaW road connection handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/winspmbt/road_connections_winspmbt.h"
#include "gamefile/winspmbt/gamedata_winspmbt.h"
#include "common/internal.h"

/* Road connection builders */
#define	RC1(a_)			(SPWAW_HEX_##a_)
#define	RC2(a_,b_)		(SPWAW_HEX_##a_+SPWAW_HEX_##b_)
#define	RC3(a_,b_,c_)		(SPWAW_HEX_##a_+SPWAW_HEX_##b_+SPWAW_HEX_##c_)
#define	RC4(a_,b_,c_,d_)	(SPWAW_HEX_##a_+SPWAW_HEX_##b_+SPWAW_HEX_##c_+SPWAW_HEX_##d_)
#define	RC5(a_,b_,c_,d_,e_)	(SPWAW_HEX_##a_+SPWAW_HEX_##b_+SPWAW_HEX_##c_+SPWAW_HEX_##d_+SPWAW_HEX_##e_)
#define	RC6(a_,b_,c_,d_,e_,f_)	(SPWAW_HEX_##a_+SPWAW_HEX_##b_+SPWAW_HEX_##c_+SPWAW_HEX_##d_+SPWAW_HEX_##e_+SPWAW_HEX_##f_)

/* winSPMBT road tileId -> road connection mapping */
typedef struct s_WINSPMBT_TILE_RC_MAP {
	USHORT	id;
	BYTE	rc;
} WINSPMBT_TILE_RC_MAP;

WINSPMBT_TILE_RC_MAP winspmbt_tile_rc_map[] = {
	{  1,	RC2(EE, WW)			},
	{  2,	RC2(SW, NE)			},
	{  3,	RC4(EE, SW, WW, NE)		},
	{  4,	RC4(SE, SW, NW, NE)		},
	{  5,	RC2(WW, NE)			},
	{  6,	RC2(WW, SE)			},
	{  7,	RC2(EE, SW)			},
	{  8,	RC2(EE, NW)			},
	{  9,	RC2(SE, NE)			},
	{ 10,	RC2(SW, NW)			},
	{ 11,	RC3(EE, WW, NW)			},
	{ 12,	RC3(SW, NW, NE)			},
	{ 13,	RC3(EE, SW, NE)			},
	{ 14,	RC3(EE, SE, WW)			},
	{ 15,	RC3(EE, SW, NW)			},
	{ 16,	RC3(SE, WW, NE)			},
	{ 17,	RC2(SE, NW)			},
	{ 18,	RC4(EE, SE, WW, NW)		},
	{ 19,	RC2(SE, NE)			},
	{ 20,	RC2(SW, NW)			},
	{ 21,	RC3(EE, WW, NE)			},
	{ 22,	RC3(EE, SW, WW)			},
	{ 23,	RC3(EE, WW, NW)			},
	{ 24,	RC3(EE, SE, WW)			},
	{ 25,	RC4(EE, SE, WW, NE)		},
	{ 26,	RC4(EE, SW, WW, NW)		},
	{ 27,	RC6(EE, SE, SW, WW, NW, NE)	},
	{ 28,	RC3(EE, SE, NW)			},
	{ 29,	RC3(SW, WW, NE)			},
	{ 30,	RC3(SE, WW, NW)			},
	{ 31,	RC3(EE, SW, WW)			},
	{ 32,	RC3(SE, NW, NE)			},
	{ 33,	RC3(EE, WW, NE)			},
	{ 34,	RC3(SE, SW, NE)			},
	{ 35,	RC4(EE, SE, SW, NW)		},
	{ 36,	RC4(EE, SW, NW, NE)		},
	{ 37,	RC4(SE, WW, NW, NE)		},
	{ 38,	RC4(SE, SW, WW, NE)		},
	{ 39,	0				},
	{ 40,	0				},
	{ 41,	0				},
	{ 42,	0				},
	{ 43,	0				},
	{ 44,	0				},
	{ 45,	0				},
	{ 46,	0				},
	{ 47,	0				},
	{ 48,	RC4(EE, SE, WW, NE)		},
	{ 49,	RC4(EE, SW, WW, NW)		},
	{ 50,	RC3(SE, SW, NW)			},
	{ 51,	0				},
	{ 52,	0				},
};

#define	WINSPMBT_ROAD_TILE	24
#define	WINSPMBT_RAIL_TILE	57
#define	WINSPMBT_TILE_SET	52

static inline bool
detect_winspmbt_road_connection (MAP_TILE &tile, BYTE &conn_road1, BYTE &conn_road2, BYTE &conn_rail, BYTE &conn_tram)
{
	int	tid, id;
	BYTE	rc;
	bool	detected = true;

	switch (MapTile2FileId(tile)) {
		case WINSPMBT_ROAD_TILE:
			tid = MapTile2TileId(tile);
			if (tid < WINSPMBT_TILE_SET) {
				rc = winspmbt_tile_rc_map[tid].rc;
				id = winspmbt_tile_rc_map[tid].id;
				//log ("%s: [%s] %2.2u -> id=%2.2u, rc=0x%2.2x\n", __FUNCTION__, "road2", tid, id, rc);
				conn_road2 = winspmbt_tile_rc_map[tid].rc;
			} else {
				rc = winspmbt_tile_rc_map[tid-WINSPMBT_TILE_SET].rc;
				id = winspmbt_tile_rc_map[tid-WINSPMBT_TILE_SET].id;
				//log ("%s: [%s] %2.2u -> id=%2.2u, rc=0x%2.2x\n", __FUNCTION__, "road1", tid, id, rc);
				conn_road1 = winspmbt_tile_rc_map[tid-WINSPMBT_TILE_SET].rc;
			}
			break;
		case WINSPMBT_RAIL_TILE:
			tid = MapTile2TileId(tile);
			if (tid < WINSPMBT_TILE_SET) {
				rc = winspmbt_tile_rc_map[tid].rc;
				id = winspmbt_tile_rc_map[tid].id;
				//log ("%s: [%s] %2.2u -> id=%2.2u, rc=0x%2.2x\n", __FUNCTION__, "rail ", tid, id, rc);
				conn_rail = winspmbt_tile_rc_map[tid].rc;
			} else {
				rc = winspmbt_tile_rc_map[tid-WINSPMBT_TILE_SET].rc;
				id = winspmbt_tile_rc_map[tid-WINSPMBT_TILE_SET].id;
				//log ("%s: [%s] %2.2u -> id=%2.2u, rc=0x%2.2x\n", __FUNCTION__, "tram ", tid, id, rc);
				conn_tram = winspmbt_tile_rc_map[tid-WINSPMBT_TILE_SET].rc;
			}
			break;
		default:
			detected = false;
			break;
	}

	return (detected);
}

SPWAW_ERROR
detect_winspmbt_road_connections (GAMEDATA *src, SPWAW_SNAPSHOT *dst)
{
	SPWAW_SNAP_MAP_RAW	*mp;
	DWORD			x, y;
	bool			b;

	CNULLARG (src); CNULLARG (dst);

	mp = &(dst->raw.game.map);
	log ("%s: map width=%u, map height=%u\n", __FUNCTION__, mp->width, mp->height);

	log ("%s: START\n", __FUNCTION__);
	for (x=0; x<mp->width; x++) {
		for (y=0; y<mp->height; y++) {
			b = detect_winspmbt_road_connection (GDWINSPMBT(src)->sec02.u.d.tile[x][y],
				mp->data[y*mp->width+x].conn_road1,
				mp->data[y*mp->width+x].conn_road2,
				mp->data[y*mp->width+x].conn_rail,
				mp->data[y*mp->width+x].conn_tram);
			if (b) log ("%s: detection for section #%u at (%u,%u)\n", __FUNCTION__, 2, x, y);
			b = detect_winspmbt_road_connection (GDWINSPMBT(src)->sec03.u.d.tile[x][y],
				mp->data[y*mp->width+x].conn_road1,
				mp->data[y*mp->width+x].conn_road2,
				mp->data[y*mp->width+x].conn_rail,
				mp->data[y*mp->width+x].conn_tram);
			if (b) log ("%s: detection for section #%u at (%u,%u)\n", __FUNCTION__, 3, x, y);
			b = detect_winspmbt_road_connection (GDWINSPMBT(src)->sec04.u.d.tile[x][y],
				mp->data[y*mp->width+x].conn_road1,
				mp->data[y*mp->width+x].conn_road2,
				mp->data[y*mp->width+x].conn_rail,
				mp->data[y*mp->width+x].conn_tram);
			if (b) log ("%s: detection for section #%u at (%u,%u)\n", __FUNCTION__, 4, x, y);
			b = detect_winspmbt_road_connection (GDWINSPMBT(src)->sec05.u.d.tile[x][y],
				mp->data[y*mp->width+x].conn_road1,
				mp->data[y*mp->width+x].conn_road2,
				mp->data[y*mp->width+x].conn_rail,
				mp->data[y*mp->width+x].conn_tram);
			if (b) log ("%s: detection for section #%u at (%u,%u)\n", __FUNCTION__, 5, x, y);
			b = detect_winspmbt_road_connection (GDWINSPMBT(src)->sec06.u.d.tile[x][y],
				mp->data[y*mp->width+x].conn_road1,
				mp->data[y*mp->width+x].conn_road2,
				mp->data[y*mp->width+x].conn_rail,
				mp->data[y*mp->width+x].conn_tram);
			if (b) log ("%s: detection for section #%u at (%u,%u)\n", __FUNCTION__, 6, x, y);
			b = detect_winspmbt_road_connection (GDWINSPMBT(src)->sec07.u.d.tile[x][y],
				mp->data[y*mp->width+x].conn_road1,
				mp->data[y*mp->width+x].conn_road2,
				mp->data[y*mp->width+x].conn_rail,
				mp->data[y*mp->width+x].conn_tram);
			if (b) log ("%s: detection for section #%u at (%u,%u)\n", __FUNCTION__, 7, x, y);
		}
	}
	log ("%s: DONE\n", __FUNCTION__);

	return (SPWERR_OK);
}
