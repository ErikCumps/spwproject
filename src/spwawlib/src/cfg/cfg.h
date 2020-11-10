/** \file
 * The SPWaW Library - internal runtime configuration handling.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_CONFIG_H
#define	INTERNAL_CONFIG_H	1

#include <spwawlib.h>
#include <spwawlib_types.h>
#include "spwoob/spwoob.h"

typedef struct s_OOBENTRY {
	SPWAW_GAME_TYPE	gametype;
	const char	*oobdir;
	SPWOOB		*oobptr;
	FILETIME	dirdate;
} OOBENTRY;

typedef struct s_CFG {
	bool		init;
	unsigned int	oob_len;
	unsigned int	oob_cnt;
	OOBENTRY	*oob_lst;
	bool		withUD;
} CFG;

extern CFG	cfg;

extern SPWAW_ERROR	cfg_set		(int cnt, SPWAW_OOBCFG *list, bool withUD);
extern void		cfg_free	(void);
extern SPWAW_ERROR	cfg_add_oobdir	(const char *oobdir, SPWAW_GAME_TYPE gametype);
extern SPWOOB *		cfg_oobptr	(const char *oobdir, SPWAW_GAME_TYPE gametype);

#endif	/* INTERNAL_CONFIG_H */
