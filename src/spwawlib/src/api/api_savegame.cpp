/** \file
 * The SPWaW Library - savegame API implementation.
 *
 * Copyright (C) 2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_savegame.h>
#include "gamefile/gamefile.h"
#include "common/internal.h"

/*! Creates a new, empty, savegame content structure */
SPWAWLIB_API SPWAW_ERROR
SPWAW_savegame_new (SPWAW_SAVEGAME **game)
{
	SPWAW_SAVEGAME	*p = NULL;
	SECMAP		*map;
	int		i;
	
	CSPWINIT;
	CNULLARG (game);
	*game = NULL;

	p = safe_malloc (SPWAW_SAVEGAME); COOM (p, "SPWAW_SAVEGAME");

	map = gamedata_secmap();
	//for (i=0; i<SPWAW_SECTION_COUNT; i++) {
	for (i=0; i<SPWW2_SECTION_COUNT; i++) {
		p->sections[i].idx = map[i].idx;
	}

	*game = p;

	return (SPWERR_OK);
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
		//for (int i=0; i<SPWAW_SECTION_COUNT; i++) {
		for (int i=0; i<SPWW2_SECTION_COUNT; i++) {
			if (p->sections[i].data) safe_free (p->sections[i].data);
		}

		safe_free (p);
	}

	return (SPWERR_OK);
}

/*! Creates a new savegame content structure from an existing savegame */
SPWAWLIB_API SPWAW_ERROR
SPWAW_savegame_load (const char *dir, int id, SPWAW_SAVEGAME **game)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_SAVEGAME	*p;
	GAMEDATA	*data;
	int		i;

	CSPWINIT;
	CNULLARG (dir); CNULLARG (game);
	*game = NULL;

	rc = SPWAW_savegame_new (&p);
	ERRORGOTO ("SPWAW_savegame_new()", handle_error);

	data = game_load_full (dir, id, NULL);
	if (!data) FAILGOTO (SPWERR_BADSAVEGAME, "game_load_full()", handle_error);

	p->comment.size = sizeof (data->cmt);

	p->comment.data = safe_smalloc (char, p->comment.size);
	COOMGOTO (p->comment.data, "SPWAW_SAVEGAME comment data", handle_error);
	memcpy (p->comment.data, &(data->cmt), p->comment.size);

	//for (i=0; i<SPWAW_SECTION_COUNT; i++) {
	for (i=0; i<SPWW2_SECTION_COUNT; i++) {
		p->sections[i].idx = data->MAP[i].idx;
		p->sections[i].size = data->MAP[i].size;

		p->sections[i].data = safe_smalloc (char, p->sections[i].size);
		COOMGOTO (p->sections[i].data, "SPWAW_SAVEGAME section data", handle_error);
		memcpy (p->sections[i].data, data->MAP[i].ptr, p->sections[i].size);
	}

	game_free (&data);

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

	data = game_new();
	COOMGOTO (data, "GAMEDATA", handle_error);

	if (!p->comment.data)
		FAILGOTO (SPWERR_FAILED, "missing comment data in SPWAW_SAVEGAME", handle_error);
	if (p->comment.size != sizeof(data->cmt))
		FAILGOTO (SPWERR_FAILED, "invalid comment data in SPWAW_SAVEGAME", handle_error);
	memcpy (&(data->cmt), p->comment.data, p->comment.size);

	//for (i=0; i<SPWAW_SECTION_COUNT; i++) {
	for (i=0; i<SPWW2_SECTION_COUNT; i++) {
		SPWAW_SAVEGAME_SECTION *sp = NULL;
		//for (j=0; j<SPWAW_SECTION_COUNT; j++) {
		for (j=0; j<SPWW2_SECTION_COUNT; j++) {
			if (p->sections[j].idx == data->MAP[i].idx) {
				sp = &(p->sections[j]);
				break;
			}
		}
		if (!sp)
			FAILGOTO (SPWERR_FAILED, "missing section data in SPWAW_SAVEGAME", handle_error);

		if (!sp->data)
			FAILGOTO (SPWERR_FAILED, "missing section data in SPWAW_SAVEGAME", handle_error);
		if (sp->size != data->MAP[i].size)
			FAILGOTO (SPWERR_FAILED, "invalid section data in SPWAW_SAVEGAME", handle_error);
		memcpy (data->MAP[i].ptr, sp->data, sp->size);
	}

	if (!game_save_full (data, dir, id))
		FAILGOTO (SPWERR_FAILED, "failed to create savegame", handle_error);

	game_free (&data);
	return (SPWERR_OK);

handle_error:
	game_free (&data);
	return (rc);
}

