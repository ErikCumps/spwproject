/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamedata.h"
#include "common/internal.h"

SPWAW_ERROR
section26_spwaw_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB * /*stab*/)
{
	CNULLARG (src); CNULLARG (dst);

	return (SPWERR_OK);
}
