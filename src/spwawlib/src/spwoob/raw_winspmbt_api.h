/** \file
 * The SPWaW Library - SPWaW OOB handling - raw winSPMBT data handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	RAW_WINSPMBT_API_H
#define	RAW_WINSPMBT_API_H	1

#include <spwawlib_spwoob.h>

extern SPWAW_ERROR	spwoob_load_raw_winspmbt_files	(SPWOOB *oob);
extern SPWAW_ERROR	spwoob_load_raw_winspmbt_data	(SPWOOB_DATA *dst);
extern void		spwoob_dump_raw_winspmbt_data	(void *rdata, BYTE id, char *base);

#endif	/* RAW_WINSPMBT_API_H */
