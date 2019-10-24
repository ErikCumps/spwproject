/** \file
 * The SPWaW Library - snapshot handling.
 *
 * Copyright (C) 2007-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "snapshot/snapshot.h"
#include "utils/ud.h"
#include "common/internal.h"

static void
snapclean_raw_game (SPWAW_SNAP_GAME_RAW *ptr)
{
	safe_free (ptr->map.data);
	if (cfg.withUD) UD_free (&(ptr->UD));
}

static void
snapclean_raw_oob (SPWAW_SNAP_OOB_RAW *ptr)
{
	DWORD	i;

	if (cfg.withUD) {
		for (i=0; i<ptr->formations.cnt; i++) UD_free (&(ptr->formations.raw[i].UD));
	}
	safe_free (ptr->formations.raw);
	safe_free (ptr->formations.ridx);

	if (cfg.withUD) {
		for (i=0; i<ptr->units.cnt; i++) UD_free (&(ptr->units.raw[i].UD));
	}
	safe_free (ptr->units.raw);
	safe_free (ptr->units.ridx);

	if (cfg.withUD) {
		for (i=0; i<ptr->leaders.cnt; i++) UD_free (&(ptr->leaders.raw[i].UD));
	}
	safe_free (ptr->leaders.raw);
	safe_free (ptr->leaders.ridx);

	if (cfg.withUD) {
		for (i=0; i<ptr->positions.cnt; i++) UD_free (&(ptr->positions.raw[i].UD));
	}
	safe_free (ptr->positions.raw);
	safe_free (ptr->positions.ridx);
}

static void
snapclean_raw (SPWAW_SNAP_RAW *ptr)
{
	snapclean_raw_game (&(ptr->game));
	snapclean_raw_oob (&(ptr->OOBp1));
	snapclean_raw_oob (&(ptr->OOBp2));
}

static void
snapclean_game (SPWAW_SNAP_GAME *ptr)
{
	safe_free (ptr->map.data);
}

static void
snapclean_oob_force (SPWAW_SNAP_OOB_FORCE *ptr, bool freeall)
{
	DWORD	i;

	for (i=0; i<ptr->formations.cnt; i++) {
		if (ptr->formations.list[i].data.ulist) safe_free (ptr->formations.list[i].data.ulist);
	}
	safe_free (ptr->formations.list);

	if (freeall) {
		for (i=0; i<ptr->units.cnt; i++) {
			if (ptr->units.list[i].data.uname) safe_free (ptr->units.list[i].data.uname);
		}
	}
	safe_free (ptr->units.list);

	if (freeall) {
		for (i=0; i<ptr->crews.cnt; i++) {
			if (ptr->crews.list[i].data.uname) safe_free (ptr->crews.list[i].data.uname);
		}
	}
	safe_free (ptr->crews.list);
}

static void
snapclean_oob (SPWAW_SNAP_OOB *ptr)
{
	snapclean_oob_force (&(ptr->battle), true);
	snapclean_oob_force (&(ptr->core), false);
	snapclean_oob_force (&(ptr->support), false);
}

SPWAW_ERROR
snapclean (SPWAW_SNAPSHOT *ptr, STRTAB *stptr)
{
	CNULLARG (ptr);

	snapclean_raw	(&(ptr->raw));
	snapclean_game	(&(ptr->game));
	snapclean_oob	(&(ptr->OOBp1));
	snapclean_oob	(&(ptr->OOBp2));
	if (ptr->freeoobdat) SPWOOB_free (&(ptr->oobdat));
	if (ptr->stab != stptr) {
		STRTAB_free ((STRTAB **)&(ptr->stab));
	}

	clear_ptr (ptr);

	return (SPWERR_OK);
}
