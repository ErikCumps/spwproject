/** \file
 * The SPWaW Library - game data handling.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEDATA_H
#define	GAMEDATA_H	1

#include <spwawlib_snapshot.h>
#include "gamefile/gamefile.h"
#include "gamefile/gamedata_secmap.h"

typedef struct s_METADATA {
	SPWAW_SAVE_TYPE		savetype;	/* Save type			*/
	void			*data;		/* Pointer to meta data storage	*/
	DWORD			size;		/* Total size of storage	*/
	DWORD			used;		/* Number of bytes used 	*/
} METADATA;

typedef struct s_GAMEDATA {
	SPWAW_GAME_TYPE		gametype;	/* Game type			*/
	SPWAW_SAVE_TYPE		savetype;	/* Save type			*/
	SPWAW_BATTLE_TYPE	type;		/* Battle type			*/
	METADATA		metadata;	/* Savegame meta data		*/
	SECMAP			map;		/* Savegame section map		*/
	void			*data;		/* Savegame section data	*/
} GAMEDATA;

extern GAMEDATA *	gamedata_new		(SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype);
extern void		gamedata_free		(GAMEDATA **game);

extern bool		gamedata_load_metadata	(GAMEFILE *file, GAMEDATA *dst);
extern bool		gamedata_load_data	(GAMEFILE *file, GAMEDATA *dst);
extern bool		gamedata_load_section	(GAMEFILE *file, DWORD sec, void **dst, unsigned long *len);

extern bool		gamedata_save_metadata	(GAMEDATA *src, GAMEFILE *file);
extern bool		gamedata_save_data	(GAMEDATA *src, GAMEFILE *file);

#endif	/* GAMEDATA_H */
