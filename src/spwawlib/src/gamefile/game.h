/** \file
 * The SPWaW Library - savegame handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAME_H
#define	GAME_H	1

#include <spwawlib_snapshot.h>
#include "gamefile/gamedata.h"

typedef struct s_GAMEINFO {
	SPWAW_GAME_TYPE		gametype;
	char			path[MAX_PATH+1];
	char			file[MAX_PATH+1];
	FILETIME		date;
	char			stamp[32];
	char			location[SPWAW_AZSLOCATION+1];
	char			comment[SPWAW_AZSCMTTITLE+1];
	SPWAW_BATTLE_TYPE	type;
} GAMEINFO;

extern GAMEDATA *	game_load_full		(SPWAW_GAME_TYPE gametype, const char *dir, unsigned int id, GAMEINFO *info);
extern bool		game_load_info		(SPWAW_GAME_TYPE gametype, const char *dir, unsigned int id, GAMEINFO *info);

extern bool		game_save_full		(GAMEDATA *src, const char *dir, unsigned int id);

#endif	/* GAME_H */
