/** \file
 * The SPWaW Library - savegame handling - winSPMBT game data.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"

#include "gamefile/game.h"
#include "gamefile/winspmbt/game_winspmbt.h"
#include "gamefile/winspmbt/cmt_winspmbt.h"
#include "gamefile/winspmbt/section_common_winspmbt.h"
#include "gamefile/winspmbt/road_connections_winspmbt.h"
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

	if (metadata->used == sizeof(CMTDATA_WINSPMBT)) {
		CMTDATA_WINSPMBT *p = (CMTDATA_WINSPMBT *)metadata->data;
		title.ptr  = p->title;
		title.size = sizeof(p->title);
	} else if (metadata->used == sizeof(CMTDATA_WINSPMBT_ALT)) {
		CMTDATA_WINSPMBT_ALT *p = (CMTDATA_WINSPMBT_ALT *)metadata->data;
		title.ptr  = p->title;
		title.size = sizeof(p->title);
	} else {
		ok = false;
	}
	return (ok);
}

static void
setup_winspmbt_info (SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEINFO *info, GAMEFILE *file, METADATA *metadata, WINSPMBT_SECTION37 *sec37, WINSPMBT_SECTION35 *sec35)
{
	WINSPMBT_SECTION37	*gamedata = sec37;
	WINSPMBT_SECTION35	*formdata = sec35;
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

	snprintf (info->oobdir, sizeof (info->oobdir) - 1, "%s", sgd->oobdir);

	section37_winspmbt_prepare (gamedata);

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

	// winSPMBT games do not have a battle day and hour!
	SPWAW_set_date (date, gamedata->u.d.data.Ygame + SPWAW_STARTYEAR, gamedata->u.d.data.Mgame);
	snprintf (info->stamp, sizeof (info->stamp) - 1, "%4.4u/%02.2u, turn %u",
		date.year, date.month, gamedata->u.d.data.turn);

	memcpy (info->location, gamedata->u.d.data.location, sizeof (gamedata->u.d.data.location));
	if (metadata_title (metadata, title)) {
		memcpy (info->title, title.ptr, title.size);
	}

	log_disable();
	rc = section35_winspmbt_detection (formdata, ful1, ful2);
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
setup_winspmbt_info (SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEINFO *info, GAMEFILE *file, GAMEDATA *game)
{
	if (!sgd || !info || !file || !game) return;

	setup_winspmbt_info (sgd, info, file, &(game->metadata), &(GDWINSPMBT(game)->sec37), &(GDWINSPMBT(game)->sec35));
}

bool
game_load_winspmbt_info (SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEINFO *info)
{
	GAMEFILE		game;
	GAMEDATA		*data;
	bool			grc = true, rc;
	WINSPMBT_SECTION37	*sec37;
	unsigned long		len37;
	WINSPMBT_SECTION35	*sec35;
	unsigned long		len35;

	if (!sgd || !info) return (false);
	if (sgd->gametype != SPWAW_GAME_TYPE_WINSPMBT) return (false);

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

	setup_winspmbt_info (sgd, info, &game, &(data->metadata), sec37, sec35);

	if (sec35) safe_free (sec35);
	if (sec37) safe_free (sec37);
	gamedata_free (&data);
	gamefile_close (&game);

	if (!grc) ERROR0 ("failed to load all game info");

	return (grc);
}

SPWAW_ERROR
load_from_winspmbt_game (GAMEDATA *src, SPWAW_SNAPSHOT *dst)
{
	SPWAW_ERROR	rc;
	STRTAB		*stab = NULL;
	TITLE		title;
	FULIST		ful1, ful2;

	CNULLARG (src); CNULLARG (dst);
	if (src->gametype != SPWAW_GAME_TYPE_WINSPMBT) {
		RWE (SPWERR_BADGTYPE, "cannot load snapshot from non-winSPMBT game data");
	} else {
		dst->gametype = SPWAW_GAME_TYPE_WINSPMBT;
	}
	dst->savetype = src->savetype;

	stab = (STRTAB *)(dst->stab);

	if (metadata_title (&(src->metadata), title)) {
		dst->raw.game.meta.title = aznstrstab (title.ptr, title.size, stab);
	}
	// winSPMBT games do not have map source data

	rc = section37_winspmbt_save_snapshot (src, dst, stab);			ROE ("section37_winspmbt_save_snapshot()");

	rc = section38_winspmbt_save_snapshot (src, dst, stab);			ROE ("section38_winspmbt_save_snapshot()");
	rc = section39_winspmbt_save_snapshot (src, dst, stab);			ROE ("section39_winspmbt_save_snapshot()");
	rc = section08_winspmbt_save_snapshot (src, dst, stab);			ROE ("section08_winspmbt_save_snapshot()");
	rc = detect_winspmbt_road_connections (src, dst);			ROE ("detect_winspmbt_road_connections()");

	// winSPMBT games do not have game credit information

	init_FULIST (ful1);
	init_FULIST (ful2);

	rc = section35_winspmbt_detection (src, ful1, ful2);			ROE ("section35_winspmbt_detection()");
	rc = section01_winspmbt_detection (src, dst, ful1, ful2);		ROE ("section01_winspmbt_detection()");
	rc = section35_winspmbt_save_snapshot (src, dst, stab, ful1, ful2);	ROE ("section35_winspmbt_save_snapshot()");
	rc = section01_winspmbt_save_snapshot (src, dst, stab, ful1, ful2);	ROE ("section01_winspmbt_save_snapshot()");
	rc = section34_winspmbt_save_snapshot (src, dst, stab, ful1, ful2);	ROE ("section34_winspmbt_save_snapshot()");
	rc = section17_winspmbt_save_snapshot (src, dst, stab, ful1, ful2);	ROE ("section17_winspmbt_save_snapshot()");

	return (SPWERR_OK);
}
