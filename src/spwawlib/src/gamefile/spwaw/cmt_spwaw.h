/** \file
 * The SPWaW Library - gamefile handling - SPWaW comment data.
 *
 * Copyright (C) 2019-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEDATA_CMT_SPWAW_H
#define	GAMEDATA_CMT_SPWAW_H	1

#include <spwawlib_defines.h>
#include "gamefile/gamefile.h"
#include "gamefile/gamedata.h"
#include "gamefile/spwaw/defines_spwaw.h"

#pragma pack(push, r1, 1)

typedef struct s_CMTDATA_SPWAW {
	char	title[SPWAW_AZSTITLE];		/* Savegame title		*/
	char	mapsrc[SPWAW_AZSMAPSRC];	/* Savegame map source		*/
} CMTDATA_SPWAW;

#pragma pack(pop, r1)

extern bool	gamedata_init_spwaw_cmt	(METADATA *data);
extern void	gamedata_free_spwaw_cmt	(METADATA *data);

extern bool	gamedata_load_spwaw_cmt	(GAMEFILE *file, METADATA *dst);
extern bool	gamedata_save_spwaw_cmt	(METADATA *src, GAMEFILE *file);

#endif	/* GAMEDATA_CMT_SPWAW_H */
