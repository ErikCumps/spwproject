/** \file
 * The SPWaW Library - savegame handling - winSPWW2 game data.
 *
 * Copyright (C) 2019-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAME_WINSPWW2_H
#define	GAME_WINSPWW2_H	1

#include "gamefile/game.h"

extern void		setup_winspww2_info	(GAMEINFO *info, GAMEFILE *file, GAMEDATA *game);
extern bool		game_load_winspww2_info	(SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEINFO *info);

extern SPWAW_ERROR	load_from_winspww2_game	(GAMEDATA *src, SPWAW_SNAPSHOT *dst);

#endif	/* GAME_WINSPWW2_H */
