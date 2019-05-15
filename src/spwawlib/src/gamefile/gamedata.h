/** \file
 * The SPWaW Library - game data handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEDATA_H
#define	GAMEDATA_H	1

#include <spwawlib_snapshot.h>
#include "gamefile/gamefile.h"
#include "gamefile/gamedata_secmap.h"

typedef struct s_CMTDATA {
	char		title[SPWAW_AZSCMTTITLE];	/* Savegame title	*/
	char		mapsrc[SPWAW_AZSCMTMAPSRC];	/* Savegame map source	*/
} CMTDATA;

typedef struct s_GAMEDATA {
	SPWAW_GAME_TYPE		gametype;	/* Savegame game type		*/
	SPWAW_BATTLE_TYPE	type;		/* battle type			*/
	CMTDATA			cmt;		/* Savegame comment data	*/
	SECMAP			map;		/* Savegame section map		*/
	void			*data;		/* Savegame section data	*/
} GAMEDATA;

extern GAMEDATA *	gamedata_new		(SPWAW_GAME_TYPE gametype);
extern void		gamedata_free		(GAMEDATA **game);

extern bool		gamedata_load_cmt	(GAMEFILE *file, CMTDATA *dst);
extern bool		gamedata_load_all	(GAMEFILE *file, GAMEDATA *dst);
extern bool		gamedata_load_section	(GAMEFILE *file, DWORD sec, void **dst, unsigned long *len);

extern bool		gamedata_save_cmt	(CMTDATA *src, GAMEFILE *file);
extern bool		gamedata_save_all	(GAMEDATA *src, GAMEFILE *file);

#endif	/* GAMEDATA_H */
