/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

/* NOTE: none of the FORMATION and UNIT list handling code is thread-safe! */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamefile.h"
#include "common/internal.h"

/* Map tile FileId extractor */
#define	MapTile2FileId(tile_)		((tile_).FileTileId/250)

/* Map tile TileId extractor */
#define	MapTile2TileId(tile_)		((tile_).FileTileId%250)

/* Map tile creator */
#define	FileTile2MapTile(file_,tile_)	((file_)*250+(tile_))

/* Road connection builders */
#define	RC1(a_)			(SPWAW_HEX_##a_)
#define	RC2(a_,b_)		(SPWAW_HEX_##a_+SPWAW_HEX_##b_)
#define	RC3(a_,b_,c_)		(SPWAW_HEX_##a_+SPWAW_HEX_##b_+SPWAW_HEX_##c_)
#define	RC4(a_,b_,c_,d_)	(SPWAW_HEX_##a_+SPWAW_HEX_##b_+SPWAW_HEX_##c_+SPWAW_HEX_##d_)
#define	RC5(a_,b_,c_,d_,e_)	(SPWAW_HEX_##a_+SPWAW_HEX_##b_+SPWAW_HEX_##c_+SPWAW_HEX_##d_+SPWAW_HEX_##e_)
#define	RC6(a_,b_,c_,d_,e_,f_)	(SPWAW_HEX_##a_+SPWAW_HEX_##b_+SPWAW_HEX_##c_+SPWAW_HEX_##d_+SPWAW_HEX_##e_+SPWAW_HEX_##f_)

/* SPWW2 road tileId -> road connection mapping */
typedef struct s_SPWW2_TILE_RC_MAP {
	USHORT	id;
	BYTE	rc;
} SPWW2_TILE_RC_MAP;

SPWW2_TILE_RC_MAP spww2_tile_rc_map[] = {
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

//static SPWAW_ERROR
//detect_road_connections (GAMEDATA *src, SPWAW_SNAPSHOT *dst)
//{
//	SPWAW_SNAP_MAP_RAW	*mp;
//	DWORD			x, y;
//
//	CNULLARG (src); CNULLARG (dst);
//
//	mp = &(dst->raw.game.map);
//
//	for (x=0; x<mp->width; x++) {
//		for (y=0; y<mp->height; y++) {
//			mp->data[y*mp->width+x].conn_road1 = src->sec08.u.d.terrain[x][y].uR1.conn_road1;
//			mp->data[y*mp->width+x].conn_road2 = src->sec08.u.d.terrain[x][y].uR2.conn_road2;
//			mp->data[y*mp->width+x].conn_rail  = src->sec08.u.d.terrain[x][y].uRR.conn_rail;
//			mp->data[y*mp->width+x].conn_tram  = 0;
//		}
//	}
//
//	return (SPWERR_OK);
//}

#define	SPWW2_ROAD_TILE	24
#define	SPWW2_RAIL_TILE	57
#define	SPWW2_TILE_SET	52

static inline bool
detect_road_connection (MAP_TILE &tile, BYTE &conn_road1, BYTE &conn_road2, BYTE &conn_rail, BYTE &conn_tram)
{
	int	tid, id;
	BYTE	rc;
	bool	detected = true;

	switch (MapTile2FileId(tile)) {
		case SPWW2_ROAD_TILE:
			tid = MapTile2TileId(tile);
			if (tid < SPWW2_TILE_SET) {
				rc = spww2_tile_rc_map[tid].rc;
				id = spww2_tile_rc_map[tid].id;
				log ("%s: [%s] %2.2u -> id=%2.2u, rc=0x%2.2x\n", __FUNCTION__, "road2", tid, id, rc);
				conn_road2 = spww2_tile_rc_map[tid].rc;
			} else {
				rc = spww2_tile_rc_map[tid-SPWW2_TILE_SET].rc;
				id = spww2_tile_rc_map[tid-SPWW2_TILE_SET].id;
				log ("%s: [%s] %2.2u -> id=%2.2u, rc=0x%2.2x\n", __FUNCTION__, "road1", tid, id, rc);
				conn_road1 = spww2_tile_rc_map[tid-SPWW2_TILE_SET].rc;
			}
			break;
		case SPWW2_RAIL_TILE:
			tid = MapTile2TileId(tile);
			if (tid < SPWW2_TILE_SET) {
				rc = spww2_tile_rc_map[tid].rc;
				id = spww2_tile_rc_map[tid].id;
				log ("%s: [%s] %2.2u -> id=%2.2u, rc=0x%2.2x\n", __FUNCTION__, "rail ", tid, id, rc);
				conn_rail = spww2_tile_rc_map[tid].rc;
			} else {
				rc = spww2_tile_rc_map[tid-SPWW2_TILE_SET].rc;
				id = spww2_tile_rc_map[tid-SPWW2_TILE_SET].id;
				log ("%s: [%s] %2.2u -> id=%2.2u, rc=0x%2.2x\n", __FUNCTION__, "tram ", tid, id, rc);
				conn_tram = spww2_tile_rc_map[tid-SPWW2_TILE_SET].rc;
			}
			break;
		default:
			detected = false;
			break;
	}

	return (detected);
}

static SPWAW_ERROR
detect_road_connections (GAMEDATA *src, SPWAW_SNAPSHOT *dst)
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
			b = detect_road_connection (src->sec02.u.d.tile[x][y],
				mp->data[y*mp->width+x].conn_road1,
				mp->data[y*mp->width+x].conn_road2,
				mp->data[y*mp->width+x].conn_rail,
				mp->data[y*mp->width+x].conn_tram);
			if (b) log ("%s: detection for section #%u at (%u,%u)\n", __FUNCTION__, 2, x, y);
			b = detect_road_connection (src->sec03.u.d.tile[x][y],
				mp->data[y*mp->width+x].conn_road1,
				mp->data[y*mp->width+x].conn_road2,
				mp->data[y*mp->width+x].conn_rail,
				mp->data[y*mp->width+x].conn_tram);
			if (b) log ("%s: detection for section #%u at (%u,%u)\n", __FUNCTION__, 3, x, y);
			b = detect_road_connection (src->sec04.u.d.tile[x][y],
				mp->data[y*mp->width+x].conn_road1,
				mp->data[y*mp->width+x].conn_road2,
				mp->data[y*mp->width+x].conn_rail,
				mp->data[y*mp->width+x].conn_tram);
			if (b) log ("%s: detection for section #%u at (%u,%u)\n", __FUNCTION__, 4, x, y);
			b = detect_road_connection (src->sec05.u.d.tile[x][y],
				mp->data[y*mp->width+x].conn_road1,
				mp->data[y*mp->width+x].conn_road2,
				mp->data[y*mp->width+x].conn_rail,
				mp->data[y*mp->width+x].conn_tram);
			if (b) log ("%s: detection for section #%u at (%u,%u)\n", __FUNCTION__, 5, x, y);
			b = detect_road_connection (src->sec06.u.d.tile[x][y],
				mp->data[y*mp->width+x].conn_road1,
				mp->data[y*mp->width+x].conn_road2,
				mp->data[y*mp->width+x].conn_rail,
				mp->data[y*mp->width+x].conn_tram);
			if (b) log ("%s: detection for section #%u at (%u,%u)\n", __FUNCTION__, 6, x, y);
			b = detect_road_connection (src->sec07.u.d.tile[x][y],
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

SPWAW_ERROR
load_from_game (GAMEDATA *src, SPWAW_SNAPSHOT *dst)
{
	SPWAW_ERROR	rc;
	STRTAB		*stab = NULL;
	FULIST		ful1, ful2;

	CNULLARG (src); CNULLARG (dst);
	stab = (STRTAB *)(dst->stab);

	dst->raw.game.cmt.title = azstrstab (src->cmt.title, stab);
	//dst->raw.game.cmt.mapsrc = azstrstab (src->cmt.mapsrc, stab);

	rc = sec37_save_snapshot (src, dst, stab);		ROE ("sec37_save_snapshot()");

	rc = sec38_save_snapshot (src, dst, stab);		ROE ("sec38_save_snapshot()");
	rc = sec39_save_snapshot (src, dst, stab);		ROE ("sec39_save_snapshot()");
	rc = sec08_save_snapshot (src, dst, stab);		ROE ("sec08_save_snapshot()");
	rc = detect_road_connections (src, dst);		ROE ("detect_road_connections()");

	rc = sec14_save_snapshot (src, dst, stab);		ROE ("sec14_save_snapshot()");

	init_FULIST (ful1);
	init_FULIST (ful2);

	rc = sec35_detection (src, ful1, ful2);			ROE ("sec35_detection()");
	rc = sec01_detection (src, dst, ful1, ful2);		ROE ("sec01_detection()");
	rc = sec35_save_snapshot (src, dst, stab, ful1, ful2);	ROE ("sec35_save_snapshot()");
	rc = sec01_save_snapshot (src, dst, stab, ful1, ful2);	ROE ("sec01_save_snapshot()");
	rc = sec34_save_snapshot (src, dst, stab, ful1, ful2);	ROE ("sec34_save_snapshot()");
	rc = sec17_save_snapshot (src, dst, stab, ful1, ful2);	ROE ("sec17_save_snapshot()");

	rc = sec26_save_snapshot (src, dst, stab);		ROE ("sec26_save_snapshot()");

	return (SPWERR_OK);
}



/* Initializes a unit list */
void
init_ULIST (ULIST &ul)
{
	memset (&ul, 0, sizeof(ul));
}

/* Dumps a unit list */
void
dump_ULIST (ULIST &ul)
{
	UEL	*p, *uel;

	UFDTRACE2 ("ULIST: { { s.nidx=%hu }, cnt=%hu }\n", ul.s.nidx, ul.cnt);

	p = ul.head;
	while (p)
	{
		uel = p; p = p->l.next;

		dump_UEL (uel, uel->d.formation ? "    " : "!!! ");
	}
}

/* Dumps a unit element */
void
dump_UEL (UEL *uel, char *prefix)
{
	UFDTRACE1 ("%s", prefix ? prefix : "    ");

	UFDTRACE3 ("[%5.5u] (%16.16s) %4.4s",
		uel->d.RID, uel->d.name, SPWAW_unittype2str(uel->d.type));
	UFDTRACE2 (" leader=[%5.5u] abandoned=%d",
		uel->d.LRID, uel->d.aband);
	UFDTRACE3 (" loader<%5.5u> vrf=%s vrfneed=%s ",
		uel->d.loader, uel->d.vrfloader ? "Y" : "N", uel->d.needvrfldrtst ? "Y" : "N");
	UFDTRACE2 (" OOB %3.3u: [%5.5u]",
		uel->d.OOB, uel->d.OOBrid);
	UFDTRACE5 (" F[%5.5u]<%5.5u,%3.3u> %s[%5.5u]",
		uel->d.FRID, uel->d.FMID, uel->d.FSID,
		uel->d.formation ? "ptr" : "nil", uel->d.formation ? uel->d.formation->d.RID : SPWAW_BADIDX);
	UFDTRACE4 (" link crew=%s[%5.5u], parent=%s[%5.5u]\n",
		uel->d.link.crew ? "ptr" : "nil", uel->d.link.crew ? uel->d.link.crew->d.RID : SPWAW_BADIDX,
		uel->d.link.parent ? "ptr" : "nil", uel->d.link.parent ? uel->d.link.parent->d.RID : SPWAW_BADIDX);
}

/* Reserves the next available element for the unit list */
UEL *
reserve_UEL (ULIST &ul)
{
	UEL *uel = NULL;

	if (ul.s.nidx < UNITCOUNT) {
		uel = &(ul.s.list[ul.s.nidx]);

		uel->l.prev = uel->l.next = NULL;

		memset (&(uel->d), 0, sizeof (uel->d));

		uel->d.OOBtype = SPWOOB_UTYPE__NONE;
		uel->d.type = SPWAW_UNIT_TYPE_UNIT;
	}

	return (uel);
}

/* Commits the (currently reserved) element to the unit list */
bool
commit_UEL (ULIST &ul, UEL *uel)
{
	UEL	*prev;

	if (uel != &(ul.s.list[ul.s.nidx])) return (false);

	if (ul.s.nidx > 0) {
		prev = &(ul.s.list[ul.s.nidx-1]);
		prev->l.next = uel;
		uel->l.prev = prev;
	}

	if (!ul.head) ul.head = uel;

	ul.s.nidx++;
	ul.cnt++;

	return (true);
}

/* Drops the element from the unit list */
void
drop_UEL (ULIST &ul, UEL *uel)
{
	if (ul.cnt == 0) return;

	if (uel->l.prev) {
		uel->l.prev->l.next = uel->l.next;
	} else {
		ul.head = uel->l.next;
	}
	if (uel->l.next) {
		uel->l.next->l.prev = uel->l.prev;
	}
	ul.cnt--;

	if ((uel->d.type == SPWAW_UNIT_TYPE_UNIT) && uel->d.link.crew) {
		drop_UEL (ul, uel->d.link.crew);
	}
}

/* Looks up the element in the unit list for the given unit record ID */
UEL *
lookup_ULIST (ULIST &ul, USHORT urid)
{
	UEL	*p = ul.head;

	while (p) {
		if (urid == p->d.RID) break;
		p = p->l.next;
	}

	return (p);
}



/* Initializes a formation list */
void
init_FLIST (FLIST &fl)
{
	memset (&fl, 0, sizeof(fl));
}

/* Dumps a formation list */
void
dump_FLIST (FLIST &fl)
{
	FEL	*p, *fel;

	UFDTRACE2 ("FLIST: { { s.nidx=%hu }, cnt=%hu }\n", fl.s.nidx, fl.cnt);

	p = fl.head;
	while (p)
	{
		fel = p; p = fel->l.next;

		dump_FEL (fel, "    ");
	}
}

void
dump_FEL (FEL *fel, char *prefix)
{
	UFDTRACE1 ("%s", prefix ? prefix : "    ");

	UFDTRACE7 ("[%5.5u] (%16.16s) P<%3.3u> L<%5.5u> H<%5.5u> status=%3.3u units=%3.3u",
		fel->d.RID, fel->d.name, fel->d.player, fel->d.leader, fel->d.hcmd, fel->d.status, fel->d.unit_cnt);
	UFDTRACE4 (" OOB %3.3u: [%5.5u] FID<%5.5u> RAW<%5.5u>\n",
		fel->d.OOB, fel->d.OOBrid, fel->d.FID, fel->d.rawFID);
	for (BYTE i=0; i<fel->d.unit_cnt; i++) {
		UFDTRACE4 ("    %s [%3.3u] %s[%5.5u]\n",
			fel->d.unit_lst[i] ? "   " : "---",
			i,
			fel->d.unit_lst[i] ? "ptr" : "nil",
			fel->d.unit_lst[i] ? fel->d.unit_lst[i]->d.RID : SPWAW_BADIDX);
	}
}
/* Reserves the next available element for the formation list */
FEL *
reserve_FEL (FLIST &fl)
{
	FEL	*fel = NULL;

	if (fl.s.nidx < FORMCOUNT) {
		fel = &(fl.s.list[fl.s.nidx]);

		fel->l.prev = fel->l.next = NULL;

		memset (&(fel->d), 0, sizeof (fel->d));
	}

	return (fel);
}

/* Commits the (currently reserved) element to the formation list */
bool
commit_FEL (FLIST &fl, FEL *fel)
{
	FEL	*prev;

	if (fel != &(fl.s.list[fl.s.nidx])) return (false);

	if (fl.s.nidx > 0) {
		prev = &(fl.s.list[fl.s.nidx-1]);
		prev->l.next = fel;
		fel->l.prev = prev;
	}

	if (!fl.head) fl.head = fel;

	// Compensate for raw formation IDs that are offset by a multiple of FORMPCOUNT
	fel->d.FID = (fel->d.rawFID % FORMPCOUNT) - (fl.head->d.rawFID % FORMPCOUNT);

	fl.s.nidx++;
	fl.cnt++;

	return (true);
}

/* Looks up the element in the formation list for the given formation record ID */
FEL *
lookup_FLIST (FLIST &fl, USHORT frid)
{
	FEL	*p = fl.head;

	while (p) {
		if (frid == p->d.RID) break;
		p = p->l.next;
	}

	return (p);
}


/* Associates a crew with its parent unit */
bool
add_crew_to_unit (UEL *cel, UEL *uel)
{
	if (!cel || !uel) return (false);

	if (uel->d.type == SPWAW_UNIT_TYPE_CREW) return (false);

	uel->d.link.crew = cel;

	cel->d.type = SPWAW_UNIT_TYPE_CREW;
	cel->d.link.parent = uel;

	return (true);
}

/* Associates a unit with its formation */
bool
add_unit_to_formation (FEL *fel, UEL *uel)
{
	if (!fel || !uel) return (false);

	if (uel->d.FSID >= MAXFORMATIONUNITS) {
		log ("failed to add unit to formation: max unit count (%u) reached\n", MAXFORMATIONUNITS);
		return (false);
	}
	if (fel->d.unit_lst[uel->d.FSID]) {
		log ("failed to add unit to formation: duplicate unit FSID (%u) detected\n", uel->d.FSID);
		return (false);
	}

	fel->d.unit_lst[uel->d.FSID] = uel;
	fel->d.unit_cnt++;
	uel->d.formation = fel;

	return (true);
}


/* Initializes a combined formation/unit list */
void
init_FULIST (FULIST &l)
{
	init_FLIST (l.fl);
	init_ULIST (l.ul);
}

/* Dumps a combined formation/unit list */
void
dump_FULIST (FULIST &l)
{
	dump_FLIST (l.fl);
	dump_ULIST (l.ul);
}

/* Looks up the first element in the unit list for the given formation element */
UEL *
lookup_FFUEL (FULIST &l, FEL *fel)
{
	UEL	*p = l.ul.head;

	if (!fel) return (NULL);

	while (p) {
		if ((fel->d.RID == p->d.FRID) && (fel->d.FID == p->d.FMID)) break;
		p = p->l.next;
	}

	return (p);
}

/* Drops the formation and all the units it contains from the formation and unit lists */
void
drop_FEL (FULIST &l, FEL *fel)
{
	if (l.fl.cnt == 0) return;

	if (fel->l.prev) {
		fel->l.prev->l.next = fel->l.next;
	} else {
		l.fl.head = fel->l.next;
	}
	if (fel->l.next) {
		fel->l.next->l.prev = fel->l.prev;
	}
	l.fl.cnt--;

	for (BYTE i = 0; i < fel->d.unit_cnt; i++) {
		if (fel->d.unit_lst[i]) drop_UEL (l.ul, fel->d.unit_lst[i]);
	}
}
