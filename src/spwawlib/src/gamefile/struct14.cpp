/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamefile.h"
#include "common/internal.h"

SPWAW_ERROR
sec14_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB * /*stab*/)
{
	CREDIT			*data;
	SPWAW_SNAP_BATTLE_RAW	*bp;

	CNULLARG (src); CNULLARG (dst);

	data = &(src->sec14.u.d.data);
	bp   = &(dst->raw.game.battle);

	bp->credit = data->amount;

	return (SPWERR_OK);
}
