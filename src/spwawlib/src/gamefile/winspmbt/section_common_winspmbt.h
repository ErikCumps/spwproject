/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION_COMMON_WINSPMBT_H
#define	SECTION_COMMON_WINSPMBT_H	1

#include <spwawlib_api.h>
#include <spwawlib_snapshot.h>
#include "gamefile/gamedata.h"
#include "gamefile/winspmbt/gamedata_winspmbt.h"
#include "gamefile/fulist.h"
#include "strtab/strtab.h"

extern SPWAW_ERROR	section01_winspmbt_detection	(GAMEDATA *src, SPWAW_SNAPSHOT *dst, FULIST &ful1, FULIST &ful2);
extern SPWAW_ERROR	section01_winspmbt_save_snapshot(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2);

extern SPWAW_ERROR	section08_winspmbt_save_snapshot(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

extern SPWAW_ERROR	section17_winspmbt_save_snapshot(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2);

extern SPWAW_ERROR	section34_winspmbt_save_snapshot(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2);

extern SPWAW_ERROR	section35_winspmbt_detection	(WINSPMBT_SECTION35 *src, FULIST &ful1, FULIST &ful2);
extern SPWAW_ERROR	section35_winspmbt_detection	(GAMEDATA *src, FULIST &ful1, FULIST &ful2);
extern SPWAW_ERROR	section35_winspmbt_save_snapshot(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2);

extern void		section37_winspmbt_prepare	(WINSPMBT_SECTION37 *src);
extern SPWAW_ERROR	section37_winspmbt_save_snapshot(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

extern SPWAW_ERROR	section38_winspmbt_save_snapshot(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

extern SPWAW_ERROR	section39_winspmbt_save_snapshot(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

#endif	/* SECTION_COMMON_WINSPMBT_H */
