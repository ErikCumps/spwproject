/** \file
 * The SPWaW Library - winSPMBT road connection handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	ROAD_CONNECTIONS_WINSPMBT_H
#define	ROAD_CONNECTIONS_WINSPMBT_H	1

#include <spwawlib_snapshot.h>
#include "gamefile/gamedata.h"

extern SPWAW_ERROR	detect_winspmbt_road_connections	(GAMEDATA *src, SPWAW_SNAPSHOT *dst);

#endif	/* ROAD_CONNECTIONS_WINSPMBT_H */
