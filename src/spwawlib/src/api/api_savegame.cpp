/** \file
 * The SPWaW Library - savegame API implementation.
 *
 * Copyright (C) 2016-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_savegame.h>
#include "gamefile/game.h"
#include "common/internal.h"

/*! Creates a new, empty, savegame content structure */
SPWAWLIB_API SPWAW_ERROR
SPWAW_savegame_new (SPWAW_GAME_TYPE gametype, SPWAW_SAVEGAME **game)
{
	SPWAW_ERROR	rc;
	SPWAW_SAVEGAME	*p = NULL;
	SECMAP		*map;
	int		i;

	CSPWINIT;
	CNULLARG (game);
	*game = NULL;

	map = gamedata_SECMAP(gametype);

	p = safe_malloc (SPWAW_SAVEGAME); COOM (p, "SPWAW_SAVEGAME");

	p->gametype = gametype;
	p->seccnt = map->cnt;
	p->seclst = safe_nmalloc (SPWAW_SAVEGAME_SECTION, p->seccnt); COOMGOTO (p->seclst, "SPWAW_SAVEGAME_SECTION list", handle_error);

	for (i=0; i<p->seccnt; i++) {
		p->seclst[i].idx = map->list[i].idx;
	}

	*game = p;

	return (SPWERR_OK);

handle_error:
	return (rc);
}

/*! Frees a savegame content structure */
SPWAWLIB_API SPWAW_ERROR
SPWAW_savegame_free (SPWAW_SAVEGAME **game)
{
	SPWAW_SAVEGAME	*p;

	CSPWINIT;
	CNULLARG (game);

	p = *game; *game = NULL;
	if (p) {
		if (p->comment.data) safe_free (p->comment.data);
		if (p->seclst) {
			for (int i=0; i<p->seccnt; i++) {
				if (p->seclst[i].data) safe_free (p->seclst[i].data);
			}
			safe_free (p->seclst);
		}

		safe_free (p);
	}

	return (SPWERR_OK);
}

/*! Creates a new savegame content structure from an existing savegame */
SPWAWLIB_API SPWAW_ERROR
SPWAW_savegame_load (SPWAW_GAME_TYPE gametype, const char *dir, int id, SPWAW_SAVEGAME **game)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_SAVEGAME	*p;
	GAMEDATA	*data;
	int		i;

	CSPWINIT;
	CNULLARG (dir); CNULLARG (game);
	*game = NULL;

	rc = SPWAW_savegame_new (gametype, &p);
	ERRORGOTO ("SPWAW_savegame_new()", handle_error);

	data = game_load_full (gametype, dir, id, NULL);
	if (!data) FAILGOTO (SPWERR_BADSAVEGAME, "game_load_full()", handle_error);

	p->comment.size = sizeof (data->cmt);

	p->comment.data = safe_smalloc (char, p->comment.size);
	COOMGOTO (p->comment.data, "SPWAW_SAVEGAME comment data", handle_error);
	memcpy (p->comment.data, &(data->cmt), p->comment.size);

	for (i=0; i<p->seccnt; i++) {
		p->seclst[i].idx = data->map.list[i].idx;
		p->seclst[i].size = data->map.list[i].size;

		p->seclst[i].data = safe_smalloc (char, p->seclst[i].size);
		COOMGOTO (p->seclst[i].data, "SPWAW_SAVEGAME section data", handle_error);
		memcpy (p->seclst[i].data, data->map.list[i].ptr, p->seclst[i].size);
	}

	gamedata_free (&data);

	*game = p;
	return (SPWERR_OK);

handle_error:
	SPWAW_savegame_free (&p);
	return (rc);
}

/*! Creates a new savegame from an existing savegame content structure */
SPWAWLIB_API SPWAW_ERROR
SPWAW_savegame_save (SPWAW_SAVEGAME **game, const char *dir, int id)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_SAVEGAME	*p;
	GAMEDATA	*data;
	int		i, j;

	CSPWINIT;
	CNULLARG (game); CNULLARG (*game);  CNULLARG (dir);
	p = *game;

	data = gamedata_new(p->gametype);
	COOMGOTO (data, "GAMEDATA", handle_error);

	if (!p->comment.data)
		FAILGOTO (SPWERR_FAILED, "missing comment data in SPWAW_SAVEGAME", handle_error);
	if (p->comment.size != sizeof(data->cmt))
		FAILGOTO (SPWERR_FAILED, "invalid comment data in SPWAW_SAVEGAME", handle_error);
	memcpy (&(data->cmt), p->comment.data, p->comment.size);

	for (i=0; i<data->map.cnt; i++) {
		SPWAW_SAVEGAME_SECTION *sp = NULL;
		for (j=0; j<p->seccnt; j++) {
			if (p->seclst[j].idx == data->map.list[i].idx) {
				sp = &(p->seclst[j]);
				break;
			}
		}
		if (!sp)
			FAILGOTO (SPWERR_FAILED, "missing section In SPWAW_SAVEGAME", handle_error);

		if (!sp->data)
			if (data->map.list[i].optional) {
				// ignore missing optional sections
				continue;
			} else {
				FAILGOTO (SPWERR_FAILED, "missing section data in SPWAW_SAVEGAME", handle_error);
			}

		if (data->map.list[i].size) {
			if (sp->size != data->map.list[i].size)
				FAILGOTO (SPWERR_FAILED, "invalid section data in SPWAW_SAVEGAME", handle_error);
			memcpy (data->map.list[i].ptr, sp->data, sp->size);
		} else {
			data->map.list[i].ptr = sp->data;
			data->map.list[i].size = sp->size;
			data->map.list[i].freeme = false;
		}
	}

	if (!game_save_full (data, dir, id))
		FAILGOTO (SPWERR_FAILED, "failed to create savegame", handle_error);

	gamedata_free (&data);
	return (SPWERR_OK);

handle_error:
	gamedata_free (&data);
	return (rc);
}

