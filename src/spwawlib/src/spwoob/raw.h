/** \file
 * The SPWaW Library - SPWaW OOB handling - raw data handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	RAW_H
#define	RAW_H	1

#include <spwawlib_spwoob.h>
#include <spwawlib_types.h>

extern SPWAW_ERROR	spwoob_load_raw_files	(SPWOOB *oob);
extern SPWAW_ERROR	spwoob_load_raw_data	(SPWOOB_DATA *dst);
extern void		spwoob_dump_raw_data	(SPWAW_GAME_TYPE gametype, void *rdata, BYTE id, char *base);

#endif	/* RAW_H */
