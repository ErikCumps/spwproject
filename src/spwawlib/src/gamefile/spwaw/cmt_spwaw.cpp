/** \file
 * The SPWaW Library - gamefile handling - SPWaW comment data.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/spwaw/cmt_spwaw.h"
#include "fileio/fileio.h"
#include "common/internal.h"

bool
gamedata_init_spwaw_cmt (METADATA *data)
{
	if (!data) return (false);

	data->savetype = SPWAW_SAVE_TYPE_REGULAR;
	data->size = sizeof (CMTDATA_SPWAW);
	data->data = (void *)safe_malloc (CMTDATA_SPWAW);
	data->used = 0;

	if (!data->data) {
		clear_ptr (data);
		return (false);
	}

	return (true);
}

void
gamedata_free_spwaw_cmt (METADATA *data)
{
	if (!data) return;

	if (data->data) safe_free (data->data);
	clear_ptr (data);
}

bool
gamedata_load_spwaw_cmt (GAMEFILE *file, METADATA *dst)
{
	CMTDATA_SPWAW	*p;
	bool		rc;
	unsigned int	i;

	if (!file || !dst) return (false);
	if (file->gametype != SPWAW_GAME_TYPE_SPWAW) return (false);
	if (file->savetype != SPWAW_SAVE_TYPE_REGULAR) return (false);
	if (dst->savetype != SPWAW_SAVE_TYPE_REGULAR) return (false);
	if (!dst->size || !dst->data) return (false);

	p = (CMTDATA_SPWAW *)dst->data; dst->used = sizeof (CMTDATA_SPWAW);
	rc = bread (file->metadata.fd, (char *)p, dst->used, false);

	if (rc) {
		/* Fix unprintable characters */
		for (i = 0; i < sizeof (p->title); i++) {
			if (p->title[i] != '\0' && !isprint(p->title[i]))
				p->title[i] = ' ';
		}

		/* Fix unprintable characters */
		for (i = 0; i < sizeof (p->mapsrc); i++) {
			if (p->mapsrc[i] != '\0' && !isprint(p->mapsrc[i]))
				p->mapsrc[i] = ' ';
		}
	} else {
		memset (dst->data, 0, dst->size);
	}

	return (rc);
}

bool
gamedata_save_spwaw_cmt (METADATA *src, GAMEFILE *file)
{
	CMTDATA_SPWAW	*p;

	if (!src || !file) return (false);
	if (src->savetype != SPWAW_SAVE_TYPE_REGULAR) return (false);
	if (file->gametype != SPWAW_GAME_TYPE_SPWAW) return (false);
	if (file->savetype != SPWAW_SAVE_TYPE_REGULAR) return (false);
	if (!src->size || !src->data) return (false);

	p = (CMTDATA_SPWAW *)src;
	return (bwrite (file->metadata.fd, (char *)p, src->used));
}
