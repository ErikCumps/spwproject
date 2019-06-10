/** \file
 * The SPWaW Library - main API header file.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 *

 * \defgroup API The SPWaW Library API
 * @{
 */

/**
 * \mainpage
 *
 * The SPWaW Library API is documented here: \ref API
 */

#ifndef	SPWAW_LIB_H
#define	SPWAW_LIB_H	1

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include <spwawlib_api.h>
#include <spwawlib_types.h>
#include <spwawlib_spwoob.h>
#include <spwawlib_savelist.h>
#include <spwawlib_snapshot.h>
#include <spwawlib_snaplist.h>
#include <spwawlib_dossier.h>
#include <spwawlib_dosslist.h>
#include <spwawlib_savegame.h>

typedef struct s_SPWAW_OOBCFG {
	SPWAW_GAME_TYPE	gametype;
	const char	*oobdir;
} SPWAW_OOBCFG;

extern SPWAWLIB_API SPWAW_ERROR		SPWAW_init	(SPWAW_OOBCFG *list, int cnt, bool withUD);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_recfg	(SPWAW_OOBCFG *list, int cnt, bool withUD);
extern SPWAWLIB_API void		SPWAW_shutdown	(void);

#endif	/* SPWAW_LIB_H */

/** @} */
