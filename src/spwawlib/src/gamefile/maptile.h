/** \file
 * The SPWaW Library - gamefile handling - map tiles.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MAPTILE_H
#define	MAPTILE_H	1

//#include "common.h"
//#include "common/sizes.h"
//#include "strtab/strtab.h"

/* Common savegame map icon data */
typedef struct s_MAP_ICON {
	BYTE	file;	/* File ID (TerIIIzJ.shp)	*/
	BYTE	icon;	/* Icon ID in SHP file		*/
} MAP_ICON;

#endif	/* MAPTILE_H */
