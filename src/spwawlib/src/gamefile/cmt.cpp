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