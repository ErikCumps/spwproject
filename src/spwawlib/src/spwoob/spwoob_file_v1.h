/** \file
 * The SPWaW Library - SPWaW OOB handling - backwards comaptibility with the V1 SPWOOB.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_SPWOOBFILE_V1_H
#define	INTERNAL_SPWOOBFILE_V1_H	1

#include <spwawlib_api.h>
#include <spwawlib_types.h>
#include <spwawlib_defines.h>
#include <ad_hash.h>
#include "spwoob/spwoob_file.h"

#define	SPWOOB_VERSION_V1	1

typedef struct s_SPWOOB_HEADER_V1 {
	BYTE	cnt;			/* OOB list count						*/
} SPWOOB_HEADER_V1;

extern SPWAW_ERROR	spwoob_load_v1_header	(int fd, SPWOOB_HEADER *hdr);

#endif	/* INTERNAL_SPWOOBFILE_V1_H */
