/** \file
 * The SPWaW Library - snapshot handling - backwards compatibility with the V13 snapshot.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "snapshot/snapfile_v13.h"
#include "snapshot/index.h"
#include "fileio/fileio.h"
#include "common/internal.h"
#include "common/types.h"

#define	copyOU(name)	uel->##name = uel_v13.##name

SPWAW_ERROR
snapshot_load_v13_oob_uel (SBR *sbr, SNAP_OOB_UEL *uel)
{
	SNAP_OOB_UEL_V13	uel_v13;

	CNULLARG (sbr); CNULLARG (uel);

	if (sbread (sbr, (char *)&uel_v13, sizeof (SNAP_OOB_UEL_V13)) != sizeof (SNAP_OOB_UEL_V13))
			RWE (SPWERR_FRFAILED, "sbread(v13 unit data)");

	/* The V13 snapshot unit element lacks the electronic warfare value. */
	uel->ew = 0;

	copyOU (dutype);
	copyOU (RID); copyOU (FRID); copyOU (FMID); copyOU (FSID);
	copyOU (name);
	copyOU (classID); copyOU (OOB); copyOU (OOBrid);
	copyOU (size); copyOU (cost); copyOU (survive); copyOU (leader);
	copyOU (exp); copyOU (mor); copyOU (sup); copyOU (status); copyOU (entr);
	copyOU (aband);
	copyOU (smkdev); copyOU (smkammo); copyOU (crew);
	copyOU (range); copyOU (stance_x); copyOU (stance_y);
	copyOU (loader); copyOU (load_cap); copyOU (load_cost);
	copyOU (contact); copyOU (rof); copyOU (tgt); copyOU (rf); copyOU (fc); copyOU (iv);
	copyOU (swim); copyOU (men); copyOU (men_ori); copyOU (speed); copyOU (moves);
	copyOU (damage); copyOU (movdir); copyOU (shtdir); copyOU (target); copyOU (UTGidx);
//	copyOU (SPECIAL_OU); copyOU (SPECIAL[0]); copyOU (SPECIAL[1]);
//	copyOU (SPECIAL[2]); copyOU (SPECIAL[3]); copyOU (SPECIAL[4]);

	return (SPWERR_OK);
}

