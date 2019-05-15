/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION_COMMON_SPWAW_H
#define	SECTION_COMMON_SPWAW_H	1

#include <spwawlib_api.h>
#include <spwawlib_snapshot.h>
#include "gamefile/gamedata.h"
#include "gamefile/spwaw/gamedata_spwaw.h"
#include "gamefile/fulist.h"
#include "strtab/strtab.h"

extern SPWAW_ERROR	section01_spwaw_detection	(GAMEDATA *src, SPWAW_SNAPSHOT *dst, FULIST &ful1, FULIST &ful2);
extern SPWAW_ERROR	section01_spwaw_save_snapshot	(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2);

extern SPWAW_ERROR	section08_spwaw_save_snapshot	(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

extern SPWAW_ERROR	section14_spwaw_save_snapshot	(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

extern SPWAW_ERROR	section17_spwaw_save_snapshot	(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2);

extern SPWAW_ERROR	section26_spwaw_save_snapshot	(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

extern SPWAW_ERROR	section34_spwaw_save_snapshot	(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2);

extern SPWAW_ERROR	section35_spwaw_detection	(SECTION35 *src, FULIST &ful1, FULIST &ful2);
extern SPWAW_ERROR	section35_spwaw_detection	(GAMEDATA *src, FULIST &ful1, FULIST &ful2);
extern SPWAW_ERROR	section35_spwaw_save_snapshot	(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2);

extern void		section37_spwaw_prepare		(SECTION37 *src);
extern SPWAW_ERROR	section37_spwaw_save_snapshot	(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

extern SPWAW_ERROR	section38_spwaw_save_snapshot	(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

extern SPWAW_ERROR	section39_spwaw_save_snapshot	(GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

#endif	/* SECTION_COMMON_SPWAW_H */
