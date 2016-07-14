/** \file
 * Slay's Library - library options handling.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef SL_INTERNAL_OPTS_H
#define SL_INTERNAL_OPTS_H	1

#include <SL/opt.h>

extern SLOPT	SL_OPTIONS;

extern SL_SINIT	SLOPT_init	(SLOPT *options);
extern void	SLOPT_shutdown	(void);


#endif /* SL_INTERNAL_OPTS_H */
