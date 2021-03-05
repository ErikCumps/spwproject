/** \file
 * The SPWaW Library - snapshot handling.
 *
 * Copyright (C) 2007-2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "snapshot/snapshot.h"
#include "gamefile/spwaw/game_spwaw.h"
#include "gamefile/winspww2/game_winspww2.h"
#include "gamefile/winspmbt/game_winspmbt.h"
#include "common/internal.h"

SPWAW_ERROR
load_from_game (GAMEDATA *src, SPWAW_SNAPSHOT *dst)
{
	CNULLARG (src); CNULLARG (dst);

	switch (src->gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			return (load_from_spwaw_game (src, dst));
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			return (load_from_winspww2_game (src, dst));
			break;
		case SPWAW_GAME_TYPE_WINSPMBT:
			return (load_from_winspmbt_game (src, dst));
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			RWE (SPWERR_FAILED, "unsupported game type");
			break;
	}
}

// Checks unit record ID
bool
check_unitid (DWORD urid, SPWAW_SNAP_OOB_URAW *ptr, SPWAW_SNAP_OOB_UELRAW **udata)
{
	DWORD	i;

	if (udata) *udata = NULL;

	if (!ptr) return (false);

	for (i=0; i<ptr->cnt; i++)
		if (urid == ptr->raw[i].RID) {
			if (udata) *udata = &(ptr->raw[i]);
			return (true);
		}

	return (false);
}

// Checks formation record ID
bool
check_formationid (DWORD frid, SPWAW_SNAP_OOB_FRAW *ptr, USHORT *fid, SPWAW_SNAP_OOB_FELRAW **fdata)
{
	DWORD	i;

	if (fid) *fid = 0;
	if (fdata) *fdata = NULL;

	if (!ptr) return (false);

	for (i=0; i<ptr->cnt; i++)
	{
		if (frid == ptr->raw[i].RID) {
			if (fid) *fid = ptr->raw[i].FID;
			if (fdata) *fdata = &(ptr->raw[i]);
			return (true);
		}
	}

	return (false);
}

// Obtain unit data from unit record ID
SPWAW_SNAP_OOB_UEL *
unitbyid (DWORD urid, SPWAW_SNAP_OOB_FORCE *ptr)
{
	SPWAW_SNAP_OOB_UEL	*p = NULL;
	DWORD			i;

	for (i=0; i<ptr->units.cnt; i++) {
		if (ptr->units.list[i].data.RID == urid) {
			p = &(ptr->units.list[i]);
			break;
		}
	}
	return (p);
}

// Obtain crew data from unit record ID
SPWAW_SNAP_OOB_UEL *
crewbyid (DWORD urid, SPWAW_SNAP_OOB_FORCE *ptr)
{
	SPWAW_SNAP_OOB_UEL	*p = NULL;
	DWORD			i;

	for (i=0; i<ptr->crews.cnt; i++) {
		if (ptr->crews.list[i].data.RID == urid) {
			p = &(ptr->crews.list[i]);
			break;
		}
	}
	return (p);
}

// Obtain either unit or crew data from unit record ID
SPWAW_SNAP_OOB_UEL *
unitorcrewbyid (DWORD urid, SPWAW_SNAP_OOB_FORCE *ptr)
{
	SPWAW_SNAP_OOB_UEL	*p = NULL;

	p = unitbyid (urid, ptr);
	if (!p) p = crewbyid (urid, ptr);

	return (p);
}

SPWAW_SNAP_OOB_FEL *
formationbyid (DWORD frid, SPWAW_SNAP_OOB_FORCE *ptr)
{
	SPWAW_SNAP_OOB_FEL	*p = NULL;
	DWORD			i;

	for (i=0; i<ptr->formations.cnt; i++) {
		if (ptr->formations.list[i].data.RID == frid) {
			p = &(ptr->formations.list[i]);
			break;
		}
	}
	return (p);
}
