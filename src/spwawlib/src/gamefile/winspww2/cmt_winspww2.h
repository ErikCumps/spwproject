/** \file
 * The SPWaW Library - game data handling - winSPWW2 comment data.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEDATA_CMT_WINSPWW2_H
#define	GAMEDATA_CMT_WINSPWW2_H	1

#include <spwawlib_defines.h>
#include "gamefile/gamefile.h"
#include "gamefile/gamedata.h"

extern bool	gamedata_load_winspww2_cmt	(GAMEFILE *file, CMTDATA *dst);
extern bool	gamedata_save_winspww2_cmt	(CMTDATA *src, GAMEFILE *file);


#endif	/* GAMEDATA_CMT_WINSPWW2_H */
