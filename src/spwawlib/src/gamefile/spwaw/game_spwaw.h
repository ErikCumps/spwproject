/** \file
 * The SPWaW Library - savegame handling - SPWaW game data.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAME_SPWAW_H
#define	GAME_SPWAW_H	1

#include "gamefile/game.h"

extern void		setup_spwaw_info	(SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEINFO *info, GAMEFILE *file, GAMEDATA *game);
extern bool		game_load_spwaw_info	(SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEINFO *info);

extern SPWAW_ERROR	load_from_spwaw_game	(GAMEDATA *src, SPWAW_SNAPSHOT *dst);

#endif	/* GAME_SPWAW_H */
