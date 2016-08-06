/** \file
 * The SPWaW Library - internal runtime configuration handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_CONFIG_H
#define	INTERNAL_CONFIG_H	1

#include "spwoob/spwoob.h"

typedef struct s_CFG {
	bool	init;
	char	*oobdir;
	SPWOOB	*oobptr;
	bool	withUD;
} CFG;

extern CFG	cfg;

extern SPWAW_ERROR	cfg_set		(const char *oobdir, bool withUD);
extern void		cfg_free	(void);

#endif	/* INTERNAL_CONFIG_H */
