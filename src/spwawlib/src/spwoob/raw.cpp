/** \file
 * The SPWaW Library - SPWaW OOB handling - raw data handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "spwoob/spwoob.h"
#include "spwoob/raw.h"
#include "spwoob/raw_spwaw_api.h"
#include "common/internal.h"

SPWAW_ERROR
spwoob_load_raw_files (SPWOOB *oob)
{
	switch (oob->gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			return (spwoob_load_raw_spwaw_files (oob));
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			RWE (SPWERR_FAILED, "winSPWW2 game type not yet supported");
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			RWE (SPWERR_FAILED, "unsupported game type");
			break;
	}
}

SPWAW_ERROR
spwoob_load_raw_data (SPWOOB_DATA *dst)
{
	switch (dst->spwoob->gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			return (spwoob_load_raw_spwaw_data (dst));
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			RWE (SPWERR_FAILED, "winSPWW2 game type not yet supported");
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			RWE (SPWERR_FAILED, "unsupported game type");
			break;
	}
}

void
spwoob_dump_raw_data (SPWAW_GAME_TYPE gametype, void *rdata, BYTE id, char *base)
{
	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			spwoob_dump_raw_spwaw_data (rdata, id, base);
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			ERROR0 ("winSPWW2 game type not yet supported");
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			ERROR0 ("unsupported game type");
			break;
	}
}
