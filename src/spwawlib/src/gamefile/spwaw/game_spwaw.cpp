/** \file
 * The SPWaW Library - savegame handling - SPWaW game data.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/game.h"
#include "gamefile/spwaw/game_spwaw.h"
#include "gamefile/spwaw/cmt_spwaw.h"
#include "gamefile/spwaw/megacam_spwaw.h"
#include "gamefile/spwaw/section_common_spwaw.h"
#include "gamefile/spwaw/road_connections_spwaw.h"
#include "gamefile/fulist.h"
#include "common/internal.h"
#include "common/types.h"

typedef struct s_STRING {
	const char	*ptr;
	DWORD		size;
} STRING;

static bool
metadata_title (METADATA *metadata, STRING &title)
{
	switch (metadata->savetype) {
		case SPWAW_SAVE_TYPE_REGULAR:
		default:
			{
				CMTDATA_SPWAW *p = (CMTDATA_SPWAW *)metadata->data;
				title.ptr  = p->title;
				title.size = sizeof(p->title);
			}
			break;
		case SPWAW_SAVE_TYPE_MEGACAM:
			{
				MEGACAMDATA_SPWAW *p = (MEGACAMDATA_SPWAW *)metadata->data;
				title.ptr  = p->u.d.title;
				title.size = sizeof(p->u.d.title);
			}
			break;
	}

	return (true);
}

static bool
metadata_mapsrc (METADATA *metadata, STRING &mapsrc)
{
	switch (metadata->savetype) {
		case SPWAW_SAVE_TYPE_REGULAR:
		default:
			{
				CMTDATA_SPWAW *p = (CMTDATA_SPWAW *)metadata->data;
				mapsrc.ptr  = p->mapsrc;
				mapsrc.size = sizeof(p->mapsrc);
			}
			break;
		case SPWAW_SAVE_TYPE_MEGACAM:
			{
				return (false);
			}
			break;
	}

	return (true);
}

static MEGACAM_COREFLAGS *
metadata_coreflags (METADATA *metadata)
{
	MEGACAM_COREFLAGS *cfp = NULL;

	switch (metadata->savetype) {
		case SPWAW_SAVE_TYPE_REGULAR:
		default:
			break;
		case SPWAW_SAVE_TYPE_MEGACAM:
			{
				MEGACAMDATA_SPWAW *p = (MEGACAMDATA_SPWAW *)metadata->data;
				cfp = (&(p->u.d.coreflags));
			}
			break;
	}

	return (cfp);
}

static void
setup_spwaw_info (GAMEINFO *info, GAMEFILE *file, METADATA *metadata, SPWAW_SECTION37 *sec37, SPWAW_SECTION35 *sec35, SPWAW_SECTION01 *sec01, SPWAW_SECTION17 *sec17)
{
	SPWAW_SECTION37	*gamedata = sec37;
	SPWAW_SECTION35	*formdata = sec35;
	SPWAW_SECTION01 *unitdata = sec01;
	SPWAW_SECTION17 *posdata  = sec17;
	char		*p, *q;
	SPWAW_DATE	base, date;
	SPWAW_PERIOD	add;
	STRING		title;
	SPWAW_ERROR	rc;
	FULIST		ful1, ful2;
	USHORT		cfcnt;

	if (!info) return;
	clear_ptr (info);

	info->gametype = file->gametype;
	info->savetype = file->savetype;

	section37_spwaw_prepare (gamedata);

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
	if (metadata_title (metadata, title)) {
		memcpy (info->title, title.ptr, title.size);
	}

	log_disable();
	rc = section35_spwaw_detection (formdata, ful1, ful2);
	log_enable();
	if (SPWAW_HAS_ERROR(rc)) {
		ERROR1 ("failed to detect formations: %s", SPWAW_errstr(rc));
		init_FLIST (ful1.fl); init_FLIST (ful2.fl);
	}

	log_disable();
	rc = section01_spwaw_detection (unitdata, posdata, cfg_oobptr(file->gametype), date, metadata_coreflags(metadata), ful1, ful2);
	log_enable();
	if (SPWAW_HAS_ERROR(rc)) {
		ERROR1 ("failed to detect units: %s", SPWAW_errstr(rc));
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
	if (cfcnt != 0) {
		if ((info->gametype == SPWAW_GAME_TYPE_SPWAW) && (info->savetype == SPWAW_SAVE_TYPE_MEGACAM)) {
			info->type = SPWAW_MEGACAM_BATTLE;
		} else {
			info->type = SPWAW_CAMPAIGN_BATTLE;
		}
	} else {
		info->type = SPWAW_STDALONE_BATTLE;
	}

	log ("setup_info: fcnt=%u, cfcnt=%u\n", ful1.fl.cnt, cfcnt);
}

void
setup_spwaw_info (GAMEINFO *info, GAMEFILE *file, GAMEDATA *game)
{
	if (!info || !file || !game) return;

	setup_spwaw_info (info, file, &(game->metadata), &(GDSPWAW(game)->sec37), &(GDSPWAW(game)->sec35), &(GDSPWAW(game)->sec01), &(GDSPWAW(game)->sec17));
}

bool
game_load_spwaw_info (SPWAW_SAVEGAME_DESCRIPTOR *sgd, GAMEINFO *info)
{
	GAMEFILE	game;
	GAMEDATA	*data;
	bool		grc = true, rc;
	SPWAW_SECTION37	*sec37;
	unsigned long	len37;
	SPWAW_SECTION35	*sec35;
	unsigned long	len35;
	SPWAW_SECTION01	*sec01;
	unsigned long	len01;
	SPWAW_SECTION17	*sec17;
	unsigned long	len17;

	if (!sgd || !info) return (false);
	if (sgd->gametype != SPWAW_GAME_TYPE_SPWAW) return (false);

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

	rc = gamedata_load_section (&game, 01, (void **)&sec01, &len01); grc = grc && rc;
	if (!rc) {
		ERROR0 ("failed to load section #01 game data");
	}

	rc = gamedata_load_section (&game, 17, (void **)&sec17, &len17); grc = grc && rc;
	if (!rc) {
		ERROR0 ("failed to load section #17 game data");
	}

	setup_spwaw_info (info, &game, &(data->metadata), sec37, sec35, sec01, sec17);

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
	STRING		str;
	FULIST		ful1, ful2;

	CNULLARG (src); CNULLARG (dst);
	if (src->gametype != SPWAW_GAME_TYPE_SPWAW) {
		RWE (SPWERR_BADGTYPE, "cannot load snapshot from non-SP:WaW game data");
	} else {
		dst->gametype = SPWAW_GAME_TYPE_SPWAW;
	}
	dst->savetype = src->savetype;

	stab = (STRTAB *)(dst->stab);

	if (metadata_title (&(src->metadata), str)) {
		dst->raw.game.meta.title = aznstrstab (str.ptr, str.size, stab);
	}
	if (metadata_mapsrc (&(src->metadata), str)) {
		dst->raw.game.meta.mapsrc = aznstrstab (str.ptr, str.size, stab);
	}

	rc = section37_spwaw_save_snapshot (src, dst, stab);		ROE ("section37_spwaw_save_snapshot()");

	rc = section38_spwaw_save_snapshot (src, dst, stab);		ROE ("section38_spwaw_save_snapshot()");
	rc = section39_spwaw_save_snapshot (src, dst, stab);		ROE ("section39_spwaw_save_snapshot()");
	rc = section08_spwaw_save_snapshot (src, dst, stab);		ROE ("section08_spwaw_save_snapshot()");
	rc = detect_spwaw_road_connections (src, dst);			ROE ("detect_spwaw_road_connections()");

	rc = section14_spwaw_save_snapshot (src, dst, stab);		ROE ("section14_spwaw_save_snapshot()");

	init_FULIST (ful1);
	init_FULIST (ful2);

	rc = section35_spwaw_detection (src, ful1, ful2);		ROE ("section35_spwaw_detection()");
	rc = section01_spwaw_detection (src, dst,  metadata_coreflags(&(src->metadata)), ful1, ful2);
									ROE ("section01_spwaw_detection()");
	rc = section35_spwaw_save_snapshot (src, dst, stab, ful1, ful2);ROE ("section35_spwaw_save_snapshot()");
	rc = section01_spwaw_save_snapshot (src, dst, stab, ful1, ful2);ROE ("section01_spwaw_save_snapshot()");
	rc = section34_spwaw_save_snapshot (src, dst, stab, ful1, ful2);ROE ("section34_spwaw_save_snapshot()");
	rc = section17_spwaw_save_snapshot (src, dst, stab, ful1, ful2);ROE ("section17_spwaw_save_snapshot()");

	rc = section26_spwaw_save_snapshot (src, dst, stab);		ROE ("section26_spwaw_save_snapshot()");

	return (SPWERR_OK);
}
