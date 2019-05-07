/** \file
 * The SPWaW Library - gamefile handling - map tiles.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MAPTILE_H
#define	MAPTILE_H	1

/* Common savegame map tile data */
typedef struct s_MAP_TILE {
	USHORT	FileTileId;	/* FileID * 250 + TileId	*/
} MAP_TILE;

/* Map tile FileId extractor */
#define	MapTile2FileId(tile_)		((tile_).FileTileId/250)

/* Map tile TileId extractor */
#define	MapTile2TileId(tile_)		((tile_).FileTileId%250)

/* Map tile creator */
#define	FileTile2MapTile(file_,tile_)	((file_)*250+(tile_))

#endif	/* MAPTILE_H */
