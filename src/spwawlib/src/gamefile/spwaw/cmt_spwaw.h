/** \file
 * The SPWaW Library - game data handling - SPWaW comment data.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEDATA_CMT_SPWAW_H
#define	GAMEDATA_CMT_SPWAW_H	1

#include <spwawlib_defines.h>
#include "gamefile/gamefile.h"
#include "gamefile/gamedata.h"

extern bool	gamedata_load_spwaw_cmt	(GAMEFILE *file, CMTDATA *dst);
extern bool	gamedata_save_spwaw_cmt	(CMTDATA *src, GAMEFILE *file);


#endif	/* GAMEDATA_CMT_SPWAW_H */
