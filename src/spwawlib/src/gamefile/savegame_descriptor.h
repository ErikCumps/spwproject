/** \file
 * The SPWaW Library - savegame descriptor.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SAVEGAME_DESCRIPTOR_H
#define	SAVEGAME_DESCRIPTOR_H	1

#include <spwawlib_types.h>
#include "strtab/strtab.h"

extern	SPWAW_ERROR	savegame_descriptor_init (SPWAW_SAVEGAME_DESCRIPTOR &sgd, SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype, const char *oobdir, const char *path, unsigned int id, STRTAB *stab = NULL);
extern	SPWAW_ERROR	savegame_descriptor_init (SPWAW_SAVEGAME_DESCRIPTOR &sgd, SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype, const char *oobdir, const char *path, const char * id, STRTAB *stab = NULL);
extern	SPWAW_ERROR	savegame_descriptor_init (SPWAW_SAVEGAME_DESCRIPTOR &sgd, SPWAW_SAVEGAME_DESCRIPTOR &src, STRTAB *stab = NULL);
extern	SPWAW_ERROR	savegame_descriptor_init (SPWAW_SAVEGAME_DESCRIPTOR &sgd, ULONG gametype, ULONG savetype, ULONG oobdir, ULONG path, ULONG id, STRTAB *stab);
extern	void		savegame_descriptor_clear (SPWAW_SAVEGAME_DESCRIPTOR &sgd, STRTAB *stab = NULL);

#endif	/* SAVEGAME_DESCRIPTOR_H */
