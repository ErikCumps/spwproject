/** \file
 * The SPWaW Library - gamefile handling - winSPWW2 comment data.
 *
 * Copyright (C) 2019-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/winspww2/cmt_winspww2.h"
#include "fileio/fileio.h"
#include "common/internal.h"

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
	} else {
		clear_ptr (&cmt);
	}

	return (rc);
}

bool
gamedata_init_winspww2_cmt (METADATA *data)
{
	if (!data) return (false);

	data->savetype = SPWAW_SAVE_TYPE_REGULAR;
	data->size = sizeof (CMTDATA_WINSPWW2);
	data->data = (void *)safe_malloc (CMTDATA_WINSPWW2);
	data->used = 0;

	if (!data->data) {
		clear_ptr (data);
		return (false);
	}

	return (true);
}

void
gamedata_free_winspww2_cmt (METADATA *data)
{
	if (!data) return;

	if (data->data) safe_free (data->data);
	clear_ptr (data);
}

static bool
gamedata_load_winspww2_cmt_reg (CMTDATA_WINSPWW2 src, METADATA *dst)
{
	CMTDATA_WINSPWW2	*p;
	unsigned int		i;

	if (dst->size < sizeof(CMTDATA_WINSPWW2)) return (false);

	p = (CMTDATA_WINSPWW2 *)dst->data;
	dst->used = sizeof(CMTDATA_WINSPWW2);

	/* Fix unprintable characters */
	for (i = 0; i < sizeof(p->title); i++) {
		if (src.title[i] != '\0' && !isprint(src.title[i]))
			p->title[i] = ' ';
		else
			p->title[i] = src.title[i];
	}

	return (true);
}

static bool
gamedata_load_winspww2_cmt_alt (CMTDATA_WINSPWW2_ALT src, METADATA *dst)
{
	CMTDATA_WINSPWW2_ALT	*p;
	unsigned int		i;

	if (dst->size < sizeof(CMTDATA_WINSPWW2)) return (false);

	p = (CMTDATA_WINSPWW2_ALT *)dst->data;
	dst->used = sizeof(CMTDATA_WINSPWW2_ALT);

	/* Fix unprintable characters */
	for (i = 0; i < sizeof(p->title); i++) {
		if (src.title[i] != '\0' && !isprint(src.title[i]))
			p->title[i] = ' ';
		else
			p->title[i] = src.title[i];
	}

	return (true);
}

bool
gamedata_load_winspww2_cmt (GAMEFILE *file, METADATA *dst)
{
	CMTDATA_WINSPWW2	cmt;
	CMTDATA_WINSPWW2_ALT	cmt_alt;
	char			*tptr;
	unsigned int		tlen;
	bool			rc;

	if (!file || !dst) return (false);
	if (file->gametype != SPWAW_GAME_TYPE_WINSPWW2) return (false);
	if (file->savetype != SPWAW_SAVE_TYPE_REGULAR) return (false);
	if (dst->savetype != SPWAW_SAVE_TYPE_REGULAR) return (false);
	if (!dst->size || !dst->data) return (false);

	memset (dst->data, 0, dst->size); dst->used = 0;

	rc = load_cmt (file->metadata.fd, cmt, &tptr, &tlen);
	if (rc) {
		return (gamedata_load_winspww2_cmt_reg (cmt, dst));
	}

	rc = load_cmt (file->metadata.fd, cmt_alt, &tptr, &tlen);
	if (rc) {
		return (gamedata_load_winspww2_cmt_alt (cmt_alt, dst));
	}

	return (rc);
}

static bool
gamedata_save_winspww2_cmt_reg (METADATA *src, GAMEFILE *file)
{
	CMTDATA_WINSPWW2	*p;

	p = (CMTDATA_WINSPWW2 *)src->data;

	return (bwrite (file->metadata.fd, (char *)p, src->used));
}

static bool
gamedata_save_winspww2_cmt_alt (METADATA *src, GAMEFILE *file)
{
	CMTDATA_WINSPWW2_ALT	*p;

	p = (CMTDATA_WINSPWW2_ALT *)src->data;

	return (bwrite (file->metadata.fd, (char *)p, src->used));
}

bool
gamedata_save_winspww2_cmt (METADATA *src, GAMEFILE *file)
{
	if (!src || !file) return (false);
	if (src->savetype != SPWAW_SAVE_TYPE_REGULAR) return (false);
	if (file->gametype != SPWAW_GAME_TYPE_SPWAW) return (false);
	if (file->savetype != SPWAW_SAVE_TYPE_REGULAR) return (false);
	if (!src->size || !src->data) return (false);

	if (src->used == sizeof(CMTDATA_WINSPWW2)) {
		return (gamedata_save_winspww2_cmt_reg (src, file));
	}

	if (src->used == sizeof(CMTDATA_WINSPWW2_ALT)) {
		return (gamedata_save_winspww2_cmt_alt (src, file));
	}

	return (false);
}
