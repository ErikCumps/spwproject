/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/gamefile.h"
#include "fileio/fileio.h"
#include "common/internal.h"

bool
gamedata_load_cmt (GAMEFILE *file, CMTDATA *dst)
{
	if (!file || !dst) return (false);

	clear_ptr (dst);

	return (bread (file->cmt_fd, (char *)dst, sizeof (*dst), false));
}
