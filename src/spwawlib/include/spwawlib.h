/** \file
 * The SPWaW Library - main API header file.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SPWAW_LIB_H
#define	SPWAW_LIB_H	1

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include <spwawlib_api.h>
#include <spwawlib_spwoob.h>
#include <spwawlib_savelist.h>
#include <spwawlib_snapshot.h>
#include <spwawlib_snaplist.h>
#include <spwawlib_dossier.h>
#include <spwawlib_dosslist.h>

extern SPWAWLIB_API SPWAW_ERROR		SPWAW_init	(const char *oobdir, bool withUD);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_recfg	(const char *oobdir, bool withUD);
extern SPWAWLIB_API void		SPWAW_shutdown	(void);

#endif	/* SPWAW_LIB_H */
