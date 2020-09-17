/** \file
 * The SPWaW Library - gamefile handling - winSPWW2 comment data.
 *
 * Copyright (C) 2019-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEDATA_CMT_WINSPWW2_H
#define	GAMEDATA_CMT_WINSPWW2_H	1

#include <spwawlib_defines.h>
#include "gamefile/gamefile.h"
#include "gamefile/gamedata.h"
#include "gamefile/winspww2/defines_winspww2.h"

#pragma pack(push, r1, 1)

typedef struct s_CMTDATA_WINSPWW2 {
	char	title[WINSPWW2_AZSTITLE];	/* Savegame title		*/
} CMTDATA_WINSPWW2;

typedef struct s_CMTDATA_WINSPWW2_ALT {
	char	title[WINSPWW2_AZSTITLE_ALT];	/* Savegame title (alternative)	*/
	/* More data follows, but its meaning is not yet discovered... */
	char	__data00[100];
} CMTDATA_WINSPWW2_ALT;

#pragma pack(pop, r1)

extern bool	gamedata_init_winspww2_cmt	(METADATA *data);
extern void	gamedata_free_winspww2_cmt	(METADATA *data);

extern bool	gamedata_load_winspww2_cmt	(GAMEFILE *file, METADATA *dst);
extern bool	gamedata_save_winspww2_cmt	(METADATA *src, GAMEFILE *file);

#endif	/* GAMEDATA_CMT_WINSPWW2_H */
