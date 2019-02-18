/** \file
 * The SPWaW Library - SPWaW OOB handling - raw SPWaW data handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	RAW_SPWAW_API_H
#define	RAW_SPWAW_API_H	1

#include <spwawlib_spwoob.h>

extern SPWAW_ERROR	spwoob_load_raw_spwaw_files	(SPWOOB *oob);
extern SPWAW_ERROR	spwoob_load_raw_spwaw_data	(SPWOOB_DATA *dst);
extern void		spwoob_dump_raw_spwaw_data	(void *rdata, BYTE id, char *base);

#endif	/* RAW_SPWAW_API_H */
