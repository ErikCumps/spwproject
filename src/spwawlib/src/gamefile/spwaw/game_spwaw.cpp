/** \file
 * The SPWaW Library - savegame handling - SPWaW game data.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/spwaw/game_spwaw.h"
#include "gamefile/spwaw/section_common_spwaw.h"
#include "gamefile/fulist.h"
#include "common/internal.h"
#include "common/types.h"

static void
setup_spwaw_info (GAMEINFO *info, char *filename, FILETIME filedate, SECTION37 *sec37, CMTDATA *gamecmt, SECTION35 *sec35)
{
	SECTION37	*gamedata = sec37;
	SECTION35	*formdata = sec35;
	char		*p, *q;
	SPWAW_DATE	base, date;
	SPWAW_PERIOD	add;
	SPWAW_ERROR	rc;
	FULIST		ful1, ful2;
	USHORT		cfcnt;

	if (!info) return;
	clear_ptr (info);

	info->gametype = SPWAW_GAME_TYPE_SPWAW;

	section37_spwaw_prepare (gamedata);

	p = strrchr (filename, '\\');
	if (p) {
		*p = '\0';
		_fullpath (info->path, filename, sizeof (info->path) - 1);
		*p = '\\';
		q = p + 1;
	} else {
		_fullpath (info->path, ".", sizeof (info->path) - 1);
		q = filename;
	}
	snprintf (info->file, sizeof (info->file) - 1, "%s", q);
	info->date = filedate;

	SPWAW_set_date (base,
		gamedata->u.d.data.Ygame + SPWAW_STARTYEAR,
		gamedata->u.d.data.Mgame,
		gamedata->u.d.data.Dgame,
		gamedata->u.d.data.Hgame
	);
	add.stamp  = gamedata->u.d.data.turn * SPWAW_MINSPERTURN;
	SPWAW_date_add (&base, &add, &date);
	snprintf (info->stamp, sizeof (info->stamp) - 1, "%4.4u/%02.2u/%02.2u %02.2u:%02.2u, turn %u",
		date.year, date.month, date.day, date.hour, date.minute, gamedata->u.d.data.turn);

	memcpy (info->location, gamedata->u.d.data.location, sizeof (gamedata->u.d.data.location));
	memcpy (info->comment, gamecmt->title, sizeof (gamecmt->title));

	log_disable();
	rc = section35_spwaw_detection (formdata, ful1, ful2);
	log_enable();
	if (SPWAW_HAS_ERROR(rc)) {
		ERROR1 ("failed to detect formations: %s", SPWAW_errstr(rc));
		init_FLIST (ful1.fl); init_FLIST (ful2.fl);
	}

	cfcnt=0;
	if (ful1.fl.cnt) {
		FEL *p = ful1.fl.head;
		while (p) {
			if (p->d.status == F_CORE) cfcnt++;
			p = p->l.next;
		}
	}
	info->type = (cfcnt != 0) ? SPWAW_CAMPAIGN_BATTLE : SPWAW_STDALONE_BATTLE;

	log ("setup_info: fcnt=%u, cfcnt=%u\n", ful1.fl.cnt, cfcnt);
}

void
setup_spwaw_info (GAMEINFO *info, GAMEFILE *file, GAMEDATA *game)
{
	if (!info || !file || !game) return;

	setup_spwaw_info (info, file->dat_name, file->dat_date, &(GDSPWAW(game)->sec37), &(game->cmt), &(GDSPWAW(game)->sec35));
}

bool
game_load_spwaw_info (const char *dir, unsigned int id, GAMEINFO *info)
{
	GAMEFILE	game;
	bool		grc = true, rc;
	SECTION37	*sec37;
	unsigned long	len37;
	SECTION35	*sec35;
	unsigned long	len35;
	CMTDATA		cmt;

	if (!info) return (false);
	clear_ptr (info);

	if (!gamefile_open (SPWAW_GAME_TYPE_SPWAW, dir, id, &game)) {
		ERROR0 ("failed to open gamefiles");
		return (false);
	}

	rc = gamedata_load_section (&game, 37, (void **)&sec37, &len37); grc = grc && rc;
	if (!rc) {
		ERROR0 ("failed to load section #37 game data");
	}

	rc = gamedata_load_section (&game, 35, (void **)&sec35, &len35); grc = grc && rc;
	if (!rc) {
		ERROR0 ("failed to load section #35 game data");
	}

	rc = gamedata_load_cmt (&game, &cmt); grc = grc && rc;
	if (!rc) {
		ERROR0 ("failed to load game comment");
		memset (&cmt, 0, sizeof (cmt));
	}

	setup_spwaw_info (info, game.dat_name, game.dat_date, sec37, &cmt, sec35);

	if (sec35) safe_free (sec35);
	if (sec37) safe_free (sec37);
	gamefile_close (&game);

	if (!grc) ERROR0 ("failed to load all game info");

	return (grc);
}

SPWAW_ERROR
load_from_spwaw_game (GAMEDATA *src, SPWAW_SNAPSHOT *dst)
{
	SPWAW_ERROR	rc;
	STRTAB		*stab = NULL;
	FULIST		ful1, ful2;

	CNULLARG (src); CNULLARG (dst);
	if (src->gametype != SPWAW_GAME_TYPE_SPWAW) {
		RWE (SPWERR_BADGTYPE, "cannot load snapshot from non-SP:WaW game data");
	} else {
		dst->gametype = SPWAW_GAME_TYPE_SPWAW;
	}

	stab = (STRTAB *)(dst->stab);

	dst->raw.game.cmt.title = azstrstab (src->cmt.title, stab);
	dst->raw.game.cmt.mapsrc = azstrstab (src->cmt.mapsrc, stab);

	rc = section38_spwaw_save_snapshot (src, dst, stab);		ROE ("section38_spwaw_save_snapshot()");
	rc = section39_spwaw_save_snapshot (src, dst, stab);		ROE ("section39_spwaw_save_snapshot()");
	rc = section37_spwaw_save_snapshot (src, dst, stab);		ROE ("section37_spwaw_save_snapshot()");
	rc = section08_spwaw_save_snapshot (src, dst, stab);		ROE ("section08_spwaw_save_snapshot()");
	rc = section14_spwaw_save_snapshot (src, dst, stab);		ROE ("section14_spwaw_save_snapshot()");

	init_FULIST (ful1);
	init_FULIST (ful2);

	rc = section35_spwaw_detection (src, ful1, ful2);		ROE ("section35_spwaw_detection()");
	rc = section01_spwaw_detection (src, dst, ful1, ful2);		ROE ("section01_spwaw_detection()");
	rc = section35_spwaw_save_snapshot (src, dst, stab, ful1, ful2);ROE ("section35_spwaw_save_snapshot()");
	rc = section01_spwaw_save_snapshot (src, dst, stab, ful1, ful2);ROE ("section01_spwaw_save_snapshot()");
	rc = section34_spwaw_save_snapshot (src, dst, stab, ful1, ful2);ROE ("section34_spwaw_save_snapshot()");
	rc = section17_spwaw_save_snapshot (src, dst, stab, ful1, ful2);ROE ("section17_spwaw_save_snapshot()");

	rc = section26_spwaw_save_snapshot (src, dst, stab);		ROE ("section26_spwaw_save_snapshot()");

	return (SPWERR_OK);
}
