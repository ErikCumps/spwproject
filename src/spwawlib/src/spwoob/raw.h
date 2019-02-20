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

/* External API */
extern SPWAW_ERROR	spwoob_load_raw_files	(SPWOOB *oob);
extern SPWAW_ERROR	spwoob_load_raw_data	(SPWOOB_DATA *dst);
extern void		spwoob_dump_raw_data	(SPWAW_GAME_TYPE gametype, void *rdata, BYTE id, char *base);

/* Internal API */
#define	SPWOOB_BADOOBID	((BYTE)-1)

typedef SPWAW_ERROR (*load_raw_data_cb)(SPWOOB_DATA *dst);
typedef BYTE (*name2id_cb) (const char *name);

extern SPWAW_ERROR	spwoob_load_raw_files_core (SPWOOB *oob, char *pattern, name2id_cb name2id, ULONG rsize, load_raw_data_cb load);

#endif	/* RAW_H */
