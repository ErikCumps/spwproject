/** \file
 * The SPWaW Library - savegame handling.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAME_H
#define	GAME_H	1

#include <spwawlib_snapshot.h>
#include "gamefile/gamedata.h"

typedef struct s_GAMEINFO {
	SPWAW_GAME_TYPE		gametype;
	SPWAW_SAVE_TYPE		savetype;
	char			path[MAX_PATH+1];
	char			file[MAX_PATH+1];
	FILETIME		date;
	char			stamp[32];
	char			location[SPWAW_AZSLOCATION+1];
	char			title[SPWAW_AZSCMTTITLE+1];
	SPWAW_BATTLE_TYPE	type;
} GAMEINFO;

extern GAMEDATA *	game_load_full		(SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEINFO *info);
extern bool		game_load_info		(SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEINFO *info);

extern bool		game_save_full		(GAMEDATA *src, SPWAW_SAVEGAME_DESCRIPTOR *sgd);

#endif	/* GAME_H */
