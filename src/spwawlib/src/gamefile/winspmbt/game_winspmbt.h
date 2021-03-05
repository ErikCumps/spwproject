/** \file
 * The SPWaW Library - savegame handling - winSPMBT game data.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAME_WINSPMBT_H
#define	GAME_WINSPMBT_H	1

#include "gamefile/game.h"

extern void		setup_winspmbt_info	(SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEINFO *info, GAMEFILE *file, GAMEDATA *game);
extern bool		game_load_winspmbt_info	(SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEINFO *info);

extern SPWAW_ERROR	load_from_winspmbt_game	(GAMEDATA *src, SPWAW_SNAPSHOT *dst);

#endif	/* GAME_WINSPMBT_H */
