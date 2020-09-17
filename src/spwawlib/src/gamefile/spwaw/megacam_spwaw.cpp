/** \file
 * The SPWaW Library - gamefile handling - SPWaW Mega Campaign data.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/spwaw/megacam_spwaw.h"
#include "fileio/fileio.h"
#include "common/internal.h"

#define UNREFERENCED_LOCAL_VARIABLE(V)	(V)

static bool
validate_megacamdata_structure (void)
{
	MEGACAMDATA_SPWAW	*p = NULL;

	/* prevent compiler warnings */
	UNREFERENCED_LOCAL_VARIABLE (p);

	assert (sizeof(p->u.raw) == sizeof(p->u.d));
	return (true);
}
static bool megacamdata_structure_ok = validate_megacamdata_structure();

bool
gamedata_init_spwaw_megacam (METADATA *data)
{
	if (!data) return (false);

	data->savetype = SPWAW_SAVE_TYPE_MEGACAM;
	data->size = sizeof (MEGACAMDATA_SPWAW);
	data->data = (void *)safe_malloc (MEGACAMDATA_SPWAW);
	data->used = 0;

	if (!data->data) {
		clear_ptr (data);
		return (false);
	}

	return (true);
}

void
gamedata_free_spwaw_megacam (METADATA *data)
{
	if (!data) return;

	if (data->data) safe_free (data->data);
	clear_ptr (data);
}

bool
gamedata_load_spwaw_megacam (GAMEFILE *file, METADATA *dst)
{
	MEGACAMDATA_SPWAW	*p;
	bool			rc;
	unsigned int		i;

	if (!file || !dst) return (false);
	if (file->gametype != SPWAW_GAME_TYPE_SPWAW) return (false);
	if (file->savetype != SPWAW_SAVE_TYPE_MEGACAM) return (false);
	if (dst->savetype != SPWAW_SAVE_TYPE_MEGACAM) return (false);
	if (!dst->size || !dst->data) return (false);

	p = (MEGACAMDATA_SPWAW *)dst->data; dst->used = sizeof (MEGACAMDATA_SPWAW);
	rc = bread (file->metadata.fd, (char *)p, dst->used, false);

	if (rc) {
		/* Fix unprintable characters */
		for (i = 0; i < sizeof (p->u.d.campaign); i++) {
			if (p->u.d.campaign[i] != '\0' && !isprint(p->u.d.campaign[i]))
				p->u.d.campaign[i] = ' ';
		}

		/* Fix unprintable characters */
		for (i = 0; i < sizeof (p->u.d.title); i++) {
			if (p->u.d.title[i] != '\0' && !isprint(p->u.d.title[i]))
				p->u.d.title[i] = ' ';
		}
	} else {
		memset (dst->data, 0, dst->size);
	}

	return (rc);
}

bool
gamedata_save_spwaw_megacam (METADATA *src, GAMEFILE *file)
{
	MEGACAMDATA_SPWAW	*p;

	if (!src || !file) return (false);
	if (src->savetype != SPWAW_SAVE_TYPE_MEGACAM) return (false);
	if (file->gametype != SPWAW_GAME_TYPE_SPWAW) return (false);
	if (file->savetype != SPWAW_SAVE_TYPE_MEGACAM) return (false);
	if (!src->size || !src->data) return (false);

	p = (MEGACAMDATA_SPWAW *)src;
	return (bwrite (file->metadata.fd, (char *)p, src->used));
}