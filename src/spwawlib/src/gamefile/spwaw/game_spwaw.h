/** \file
 * The SPWaW Library - savegame handling - SPWaW game data.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAME_SPWAW_H
#define	GAME_SPWAW_H	1

#include "gamefile/game.h"

extern void		setup_spwaw_info	(GAMEINFO *info, GAMEFILE *file, GAMEDATA *game);
extern bool		game_load_spwaw_info	(const char *dir, unsigned int id, GAMEINFO *info);

extern SPWAW_ERROR	load_from_spwaw_game	(GAMEDATA *src, SPWAW_SNAPSHOT *dst);

#endif	/* GAME_SPWAW_H */
