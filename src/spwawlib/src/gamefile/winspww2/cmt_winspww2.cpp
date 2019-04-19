/** \file
 * The SPWaW Library - gamefile handling - winSPWW2 comment data.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/winspww2/cmt_winspww2.h"
#include "fileio/fileio.h"
#include "common/internal.h"

typedef struct s_CMTDATA_WINSPWW2 {
	char	title[SPWAW_AZSCMTTITLE];	/* Savegame title	*/
} CMTDATA_WINSPWW2;

bool
gamedata_load_winspww2_cmt (GAMEFILE *file, CMTDATA *dst)
{
	CMTDATA_WINSPWW2	cmt;
	bool			rc;
	unsigned int		todo, i;

	if (!file || !dst) return (false);

	clear_ptr (dst);

	rc = bread (file->cmt_fd, (char *)&cmt, sizeof (cmt), false);

	if (rc) {
		/* Fix unprintable characters */
		todo = sizeof (dst->title); if (sizeof(cmt.title) < todo) todo = sizeof(cmt.title);
		for (i = 0; i < todo; i++) {
			if (cmt.title[i] != '\0' && !isprint(cmt.title[i]))
				dst->title[i] = ' ';
			else
				dst->title[i] = cmt.title[i];
		}
	}

	return (rc);
}

bool
gamedata_save_winspww2_cmt (CMTDATA *src, GAMEFILE *file)
{
	CMTDATA_WINSPWW2	cmt;
	unsigned int		todo;

	if (!src || !file) return (false);

	memset (&cmt, 0, sizeof(cmt));

	todo = sizeof(cmt.title); if (sizeof(src->title) < todo) todo = sizeof(src->title);
	memcpy (cmt.title, src->title, todo);

	return (bwrite (file->cmt_fd, (char *)&cmt, sizeof (cmt)));
}
