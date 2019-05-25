/** \file
 * The SPWaW Library - winSPWW2 gamefile handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION_COMMON_WINSPWW2_H
#define	SECTION_COMMON_WINSPWW2_H	1

#include <spwawlib_api.h>
#include <spwawlib_snapshot.h>
#include "gamefile/gamedata.h"
#include "gamefile/winspww2/gamedata_winspww2.h"
#include "gamefile/fulist.h"
#include "strtab/strtab.h"

extern SPWAW_ERROR	section01_winspww2_detection	(GAMEDATA *src, SPWAW_SNAPSHOT *dst, FULIST &ful1, FULIST &ful2);
extern SPWAW_ERROR	section01_winspww2_save_snapshot(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2);

extern SPWAW_ERROR	section08_winspww2_save_snapshot(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

extern SPWAW_ERROR	section17_winspww2_save_snapshot(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2);

extern SPWAW_ERROR	section34_winspww2_save_snapshot(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2);

extern SPWAW_ERROR	section35_winspww2_detection	(WINSPWW2_SECTION35 *src, FULIST &ful1, FULIST &ful2);
extern SPWAW_ERROR	section35_winspww2_detection	(GAMEDATA *src, FULIST &ful1, FULIST &ful2);
extern SPWAW_ERROR	section35_winspww2_save_snapshot(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2);

extern void		section37_winspww2_prepare	(WINSPWW2_SECTION37 *src);
extern SPWAW_ERROR	section37_winspww2_save_snapshot(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

extern SPWAW_ERROR	section38_winspww2_save_snapshot(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

extern SPWAW_ERROR	section39_winspww2_save_snapshot(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

#endif	/* SECTION_COMMON_WINSPWW2_H */
