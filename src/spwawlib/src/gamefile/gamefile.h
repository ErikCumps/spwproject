/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEFILE_H
#define	GAMEFILE_H	1

#include <spwawlib_types.h>

typedef struct s_GAMEFILE_FILE {
	char		name[MAX_PATH+1];
	char		description[MAX_PATH+1];
	int		fd;
	FILETIME	date;
} GAMEFILE_FILE;

typedef struct s_GAMEFILE {
	SPWAW_GAME_TYPE	gametype;
	SPWAW_SAVE_TYPE	savetype;
	GAMEFILE_FILE	metadata;
	GAMEFILE_FILE	data;
} GAMEFILE;

typedef struct s_GAMEFILE_META {
	const char *base;
	const char *ext_data;
	const char *ext_metadata;
} GAMEFILE_META;

extern bool	gamefile_info		(SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype, GAMEFILE_META *meta);
extern bool	gamefile_basename	(SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype, char *filename, char *dst, unsigned int len);

extern bool	gamefile_open		(SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEFILE *game);
extern bool	gamefile_create		(SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEFILE *game);
extern bool	gamefile_close		(GAMEFILE *game);

#endif	/* GAMEFILE_H */