/** \file
 * The SPWaW Library - gamefile handling - winSPMBT comment data.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEDATA_CMT_WINSPMBT_H
#define	GAMEDATA_CMT_WINSPMBT_H	1

#include <spwawlib_defines.h>
#include "gamefile/gamefile.h"
#include "gamefile/gamedata.h"
#include "gamefile/winspmbt/defines_winspmbt.h"

#pragma pack(push, r1, 1)

typedef struct s_CMTDATA_WINSPMBT {
	char	title[WINSPMBT_AZSTITLE];	/* Savegame title		*/
} CMTDATA_WINSPMBT;

typedef struct s_CMTDATA_WINSPMBT_ALT {
	char	title[WINSPMBT_AZSTITLE_ALT];	/* Savegame title (alternative)	*/
	/* More data follows, but its meaning is not yet discovered... */
	char	__data00[100];
} CMTDATA_WINSPMBT_ALT;

#pragma pack(pop, r1)

extern bool	gamedata_init_winspmbt_cmt	(METADATA *data);
extern void	gamedata_free_winspmbt_cmt	(METADATA *data);

extern bool	gamedata_load_winspmbt_cmt	(GAMEFILE *file, METADATA *dst);
extern bool	gamedata_save_winspmbt_cmt	(METADATA *src, GAMEFILE *file);

#endif	/* GAMEDATA_CMT_WINSPMBT_H */
