/** \file
 * The SPWaW Library - gamefile handling - winSPWW2 comment data.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/winspww2/cmt_winspww2.h"
#include "gamefile/winspww2/defines_winspww2.h"
#include "fileio/fileio.h"
#include "common/internal.h"

typedef struct s_CMTDATA_WINSPWW2 {
	char	title[WINSPWW2_AZSTITLE];	/* Savegame title		*/
} CMTDATA_WINSPWW2;

typedef struct s_CMTDATA_WINSPWW2_ALT {
	char	title[WINSPWW2_AZSTITLE_ALT];	/* Savegame title (alternative)	*/
	/* More data follows, but its meaning is not yet discovered... */
} CMTDATA_WINSPWW2_ALT;

template <typename T> static bool
load_cmt (int fd, T &cmt, char **tptr, unsigned int *tlen)
{
	bool	rc;

	*tptr = NULL; *tlen = 0;

	bseekset (fd, 0);
	rc = bread (fd, (char *)&cmt, sizeof (cmt), true);
	if (rc) {
		*tptr = cmt.title;
		*tlen = sizeof(cmt.title);
	}

	return (rc);
}

bool
gamedata_load_winspww2_cmt (GAMEFILE *file, CMTDATA *dst)
{
	CMTDATA_WINSPWW2	cmt;
	CMTDATA_WINSPWW2_ALT	cmt_alt;
	char			*tptr;
	unsigned int		tlen;
	bool			rc;
	unsigned int		todo, i;

	if (!file || !dst) return (false);

	clear_ptr (dst);

	rc = load_cmt (file->cmt_fd, cmt, &tptr, &tlen);
	if (!rc) {
		rc = load_cmt (file->cmt_fd, cmt_alt, &tptr, &tlen);
	}

	if (rc) {
		/* Fix unprintable characters */
		todo = sizeof (dst->title); if (tlen < todo) todo = tlen;
		for (i = 0; i < todo; i++) {
			if (tptr[i] != '\0' && !isprint(tptr[i]))
				dst->title[i] = ' ';
			else
				dst->title[i] = tptr[i];
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
