/** \file
 * The SPWaW Library - game data handling - winSPWW2 meta data.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEDATA_METADATA_WINSPWW2_H
#define	GAMEDATA_METADATA_WINSPWW2_H	1

#include "gamefile/gamefile.h"
#include "gamefile/gamedata.h"

extern bool		gamedata_winspww2_new_metadata	(GAMEDATA *game);
extern bool		gamedata_winspww2_new_metadata	(GAMEFILE *file, METADATA *data);
extern void		gamedata_winspww2_free_metadata	(METADATA *data);

extern bool		gamedata_winspww2_load_metadata	(GAMEFILE *file, METADATA *dst);
extern bool		gamedata_winspww2_save_metadata	(METADATA *src, GAMEFILE *file);

#endif	/* GAMEDATA_METADATA_WINSPWW2_H */
