/** \file
 * The SPWaW Library - gamefile handling - SPWaW comment data.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/spwaw/cmt_spwaw.h"
#include "fileio/fileio.h"
#include "common/internal.h"

typedef struct s_CMTDATA_SPWAW {
	char	title[SPWAW_AZSCMTTITLE];	/* Savegame title	*/
	char	mapsrc[SPWAW_AZSCMTMAPSRC];	/* Savegame map source	*/
} CMTDATA_SPWAW;

bool
gamedata_load_spwaw_cmt (GAMEFILE *file, CMTDATA *dst)
{
	CMTDATA_SPWAW	cmt;
	bool		rc;
	unsigned int	todo, i;

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

		/* Fix unprintable characters */
		todo = sizeof (dst->mapsrc); if (sizeof(cmt.mapsrc) < todo) todo = sizeof(cmt.mapsrc);
		for (i = 0; i < todo; i++) {
			if (cmt.mapsrc[i] != '\0' && !isprint(cmt.mapsrc[i])) 
				dst->mapsrc[i] = ' ';
			else
				dst->mapsrc[i] = cmt.mapsrc[i];
		}
	}

	return (rc);
}

bool
gamedata_save_spwaw_cmt (CMTDATA *src, GAMEFILE *file)
{
	CMTDATA_SPWAW	cmt;
	unsigned int	todo;

	if (!src || !file) return (false);

	memset (&cmt, 0, sizeof(cmt));

	todo = sizeof(cmt.title); if (sizeof(src->title) < todo) todo = sizeof(src->title);
	memcpy (cmt.title, src->title, todo);

	todo = sizeof(cmt.mapsrc); if (sizeof(src->mapsrc) < todo) todo = sizeof(src->mapsrc);
	memcpy (cmt.mapsrc, src->mapsrc, todo);

	return (bwrite (file->cmt_fd, (char *)&cmt, sizeof (cmt)));
}
