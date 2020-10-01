/** \file
 * The SPWaW Library - savegame handling - winSPWW2 game data.
 *
 * Copyright (C) 2019-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"

#include "gamefile/game.h"
#include "gamefile/winspww2/game_winspww2.h"
#include "gamefile/winspww2/cmt_winspww2.h"
#include "gamefile/winspww2/section_common_winspww2.h"
#include "gamefile/winspww2/road_connections_winspww2.h"
#include "gamefile/fulist.h"
#include "common/internal.h"
#include "common/types.h"

typedef struct s_TITLE {
	const char	*ptr;
	DWORD		size;
} TITLE;

static bool
metadata_title (METADATA *metadata, TITLE &title)
{
	bool	ok = true;

	if (metadata->used == sizeof(CMTDATA_WINSPWW2)) {
		CMTDATA_WINSPWW2 *p = (CMTDATA_WINSPWW2 *)metadata->data;
		title.ptr  = p->title;
		title.size = sizeof(p->title);
	} else if (metadata->used == sizeof(CMTDATA_WINSPWW2_ALT)) {
		CMTDATA_WINSPWW2_ALT *p = (CMTDATA_WINSPWW2_ALT *)metadata->data;
		title.ptr  = p->title;
		title.size = sizeof(p->title);
	} else {
		ok = false;
	}
	return (ok);
}

static void
setup_winspww2_info (GAMEINFO *info, GAMEFILE *file, METADATA *metadata, WINSPWW2_SECTION37 *sec37, WINSPWW2_SECTION35 *sec35)
{
	WINSPWW2_SECTION37	*gamedata = sec37;
	WINSPWW2_SECTION35	*formdata = sec35;
	char			*p, *q;
	SPWAW_DATE		date;
	TITLE			title;
	SPWAW_ERROR		rc;
	FULIST			ful1, ful2;
	USHORT			cfcnt;

	if (!info) return;
	clear_ptr (info);

	info->gametype = file->gametype;
	info->savetype = file->savetype;

	section37_winspww2_prepare (gamedata);

	p = strrchr (file->data.name, '\\');
	if (p) {
		q = p; while (*q == '\\') q--; q++;
		*q = '\0';
		_fullpath (info->path, file->data.name, sizeof (info->path) - 1);
		*q = '\\';
		q = p + 1;
	} else {
		_fullpath (info->path, ".", sizeof (info->path) - 1);
		q = file->data.name;
	}
	snprintf (info->file, sizeof (info->file) - 1, "%s", q);
	info->date = file->data.date;

	// winSPWW2 games do not have a battle day and hour!
	SPWAW_set_date (date, gamedata->u.d.data.Ygame + SPWAW_STARTYEAR, gamedata->u.d.data.Mgame);
	snprintf (info->stamp, sizeof (info->stamp) - 1, "%4.4u/%02.2u, turn %u",
		date.year, date.month, gamedata->u.d.data.turn);

	memcpy (info->location, gamedata->u.d.data.location, sizeof (gamedata->u.d.data.location));
	if (metadata_title (metadata, title)) {
		memcpy (info->title, title.ptr, title.size);
	}

	log_disable();
	rc = section35_winspww2_detection (formdata, ful1, ful2);
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
setup_winspww2_info (GAMEINFO *info, GAMEFILE *file, GAMEDATA *game)
{
	if (!info || !file || !game) return;

	setup_winspww2_info (info, file, &(game->metadata), &(GDWINSPWW2(game)->sec37), &(GDWINSPWW2(game)->sec35));
}

bool
game_load_winspww2_info (SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEINFO *info)
{
	GAMEFILE		game;
	GAMEDATA		*data;
	bool			grc = true, rc;
	WINSPWW2_SECTION37	*sec37;
	unsigned long		len37;
	WINSPWW2_SECTION35	*sec35;
	unsigned long		len35;

	if (!sgd || !info) return (false);
	if (sgd->gametype != SPWAW_GAME_TYPE_WINSPWW2) return (false);

	clear_ptr (info);

	if (!gamefile_open (sgd, &game)) {
		ERROR0 ("failed to open gamefiles");
		return (false);
	}

	data = gamedata_new (game.gametype, game.savetype); COOMRET (data, "GAMEDATA", false);

	rc = gamedata_load_metadata (&game, data); grc = grc && rc;
	if (!rc) {
		ERROR0 ("failed to load game metadata");
	}

	rc = gamedata_load_section (&game, 37, (void **)&sec37, &len37); grc = grc && rc;
	if (!rc) {
		ERROR0 ("failed to load section #37 game data");
	}

	rc = gamedata_load_section (&game, 35, (void **)&sec35, &len35); grc = grc && rc;
	if (!rc) {
		ERROR0 ("failed to load section #35 game data");
	}

	setup_winspww2_info (info, &game, &(data->metadata), sec37, sec35);

	if (sec35) safe_free (sec35);
	if (sec37) safe_free (sec37);
	gamefile_close (&game);

	if (!grc) ERROR0 ("failed to load all game info");

	return (grc);
}

SPWAW_ERROR
load_from_winspww2_game (GAMEDATA *src, SPWAW_SNAPSHOT *dst)
{
	SPWAW_ERROR	rc;
	STRTAB		*stab = NULL;
	TITLE		title;
	FULIST		ful1, ful2;

	CNULLARG (src); CNULLARG (dst);
	if (src->gametype != SPWAW_GAME_TYPE_WINSPWW2) {
		RWE (SPWERR_BADGTYPE, "cannot load snapshot from non-winSPWW2 game data");
	} else {
		dst->gametype = SPWAW_GAME_TYPE_WINSPWW2;
	}
	dst->savetype = src->savetype;

	stab = (STRTAB *)(dst->stab);

	if (metadata_title (&(src->metadata), title)) {
		dst->raw.game.meta.title = aznstrstab (title.ptr, title.size, stab);
	}
	// winSPWW2 games do not have map source data

	rc = section37_winspww2_save_snapshot (src, dst, stab);			ROE ("section37_winspww2_save_snapshot()");

	rc = section38_winspww2_save_snapshot (src, dst, stab);			ROE ("section38_winspww2_save_snapshot()");
	rc = section39_winspww2_save_snapshot (src, dst, stab);			ROE ("section39_winspww2_save_snapshot()");
	rc = section08_winspww2_save_snapshot (src, dst, stab);			ROE ("section08_winspww2_save_snapshot()");
	rc = detect_winspww2_road_connections (src, dst);			ROE ("detect_winspww2_road_connections()");

	// winSPWW2 games do not have game credit information

	init_FULIST (ful1);
	init_FULIST (ful2);

	rc = section35_winspww2_detection (src, ful1, ful2);			ROE ("section35_winspww2_detection()");
	rc = section01_winspww2_detection (src, dst, ful1, ful2);		ROE ("section01_winspww2_detection()");
	rc = section35_winspww2_save_snapshot (src, dst, stab, ful1, ful2);	ROE ("section35_winspww2_save_snapshot()");
	rc = section01_winspww2_save_snapshot (src, dst, stab, ful1, ful2);	ROE ("section01_winspww2_save_snapshot()");
	rc = section34_winspww2_save_snapshot (src, dst, stab, ful1, ful2);	ROE ("section34_winspww2_save_snapshot()");
	rc = section17_winspww2_save_snapshot (src, dst, stab, ful1, ful2);	ROE ("section17_winspww2_save_snapshot()");

	return (SPWERR_OK);
}
