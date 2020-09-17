/** \file
 * The SPWaW Library - game data handling - SPWaW meta data.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEDATA_METADATA_SPWAW_H
#define	GAMEDATA_METADATA_SPWAW_H	1

#include "gamefile/gamefile.h"
#include "gamefile/gamedata.h"

extern bool		gamedata_spwaw_new_metadata	(GAMEDATA *game);
extern bool		gamedata_spwaw_new_metadata	(GAMEFILE *file, METADATA *data);
extern void		gamedata_spwaw_free_metadata	(METADATA *data);

extern bool		gamedata_spwaw_load_metadata	(GAMEFILE *file, METADATA *dst);
extern bool		gamedata_spwaw_save_metadata	(METADATA *src, GAMEFILE *file);

#endif	/* GAMEDATA_METADATA_SPWAW_H */
