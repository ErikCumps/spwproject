/** \file
 * The SPWaW Library - internal runtime configuration handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_CONFIG_H
#define	INTERNAL_CONFIG_H	1

#include <spwawlib.h>
#include <spwawlib_types.h>
#include "spwoob/spwoob.h"

typedef struct s_OOBCFG {
	SPWAW_GAME_TYPE	gametype;
	const char	*oobdir;
	SPWOOB		*oobptr;
} OOBCFG;

typedef struct s_CFG {
	bool		init;
	int		oob_cnt;
	OOBCFG		*oob_lst;
	bool		withUD;
} CFG;

extern CFG	cfg;

extern SPWAW_ERROR	cfg_set		(int cnt, SPWAW_OOBCFG *list, bool withUD);
extern const char *	cfg_oobdir	(SPWAW_GAME_TYPE gametype);
extern SPWOOB *		cfg_oobptr	(SPWAW_GAME_TYPE gametype);
extern void		cfg_free	(void);

#endif	/* INTERNAL_CONFIG_H */
