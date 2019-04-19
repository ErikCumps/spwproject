/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEFILE_H
#define	GAMEFILE_H	1

#include <spwawlib_types.h>

typedef struct s_GAMEFILE {
	SPWAW_GAME_TYPE	gametype;
	char		*cmt_name;
	int		cmt_fd;
	FILETIME	cmt_date;
	char		*dat_name;
	int		dat_fd;
	FILETIME	dat_date;
} GAMEFILE;

extern bool	gamefile_basename	(SPWAW_GAME_TYPE type, const char **base);

extern bool	gamefile_open		(SPWAW_GAME_TYPE gametype, const char *dir, int id, GAMEFILE *game);
extern bool	gamefile_create		(SPWAW_GAME_TYPE gametype, const char *dir, int id, GAMEFILE *game);
extern bool	gamefile_close		(GAMEFILE *game);

#endif	/* GAMEFILE_H */