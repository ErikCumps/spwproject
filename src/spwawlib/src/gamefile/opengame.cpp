/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/gamefile.h"
#include "utils/log.h"
#include "common/internal.h"
#include "common/types.h"

#define	PATHMAX	256

static SECMAP	MAP[SPWAW_SECTION_COUNT] = {
	{  0,	(void *)offsetof (GAMEDATA, sec00),	SIZESEC00,	false	},
	{  1,	(void *)offsetof (GAMEDATA, sec01),	SIZESEC01,	true	},
	{  2,	(void *)offsetof (GAMEDATA, sec02),	SIZESEC02,	false	},
	{  3,	(void *)offsetof (GAMEDATA, sec03),	SIZESEC03,	true	},
	{  4,	(void *)offsetof (GAMEDATA, sec04),	SIZESEC04,	true	},
	{  5,	(void *)offsetof (GAMEDATA, sec05),	SIZESEC05,	true	},
	{  6,	(void *)offsetof (GAMEDATA, sec06),	SIZESEC06,	true	},
	{  7,	(void *)offsetof (GAMEDATA, sec07),	SIZESEC07,	true	},
	{  8,	(void *)offsetof (GAMEDATA, sec08),	SIZESEC08,	true	},
	{  9,	(void *)offsetof (GAMEDATA, sec09),	SIZESEC09,	true	},
	{ 10,	(void *)offsetof (GAMEDATA, sec10),	SIZESEC10,	false	},
	{ 11,	(void *)offsetof (GAMEDATA, sec11),	SIZESEC11,	true	},
	{ 12,	(void *)offsetof (GAMEDATA, sec12),	SIZESEC12,	false	},
	{ 13,	(void *)offsetof (GAMEDATA, sec13),	SIZESEC13,	false	},
	{ 14,	(void *)offsetof (GAMEDATA, sec14),	SIZESEC14,	false	},
	{ 15,	(void *)offsetof (GAMEDATA, sec15),	SIZESEC15,	false	},
	{ 16,	(void *)offsetof (GAMEDATA, sec16),	SIZESEC16,	false	},
	{ 17,	(void *)offsetof (GAMEDATA, sec17),	SIZESEC17,	true	},
	{ 18,	(void *)offsetof (GAMEDATA, sec18),	SIZESEC18,	true	},
	{ 19,	(void *)offsetof (GAMEDATA, sec19),	SIZESEC19,	true	},
	{ 20,	(void *)offsetof (GAMEDATA, sec20),	SIZESEC20,	false	},
	{ 21,	(void *)offsetof (GAMEDATA, sec21),	SIZESEC21,	false	},
	{ 22,	(void *)offsetof (GAMEDATA, sec22),	SIZESEC22,	false	},
	{ 23,	(void *)offsetof (GAMEDATA, sec23),	SIZESEC23,	true	},
	{ 24,	(void *)offsetof (GAMEDATA, sec24),	SIZESEC24,	true	},
	{ 25,	(void *)offsetof (GAMEDATA, sec25),	SIZESEC25,	true	},
	{ 26,	(void *)offsetof (GAMEDATA, sec26),	SIZESEC26,	true	},
	{ 27,	(void *)offsetof (GAMEDATA, sec27),	SIZESEC27,	false	},
	{ 28,	(void *)offsetof (GAMEDATA, sec28),	SIZESEC28,	true	},
	{ 29,	(void *)offsetof (GAMEDATA, sec29),	SIZESEC29,	true	},
	{ 30,	(void *)offsetof (GAMEDATA, sec30),	SIZESEC30,	true	},
	{ 31,	(void *)offsetof (GAMEDATA, sec31),	SIZESEC31,	true	},
	{ 32,	(void *)offsetof (GAMEDATA, sec32),	SIZESEC32,	true	},
	{ 33,	(void *)offsetof (GAMEDATA, sec33),	SIZESEC33,	true	},
	{ 34,	(void *)offsetof (GAMEDATA, sec34),	SIZESEC34,	true	},
	{ 35,	(void *)offsetof (GAMEDATA, sec35),	SIZESEC35,	true	},
	{ 36,	(void *)offsetof (GAMEDATA, sec36),	SIZESEC36,	true	},
	{ 37,	(void *)offsetof (GAMEDATA, sec37),	SIZESEC37,	true	},
	{ 38,	(void *)offsetof (GAMEDATA, sec38),	SIZESEC38,	false	},
	{ 39,	(void *)offsetof (GAMEDATA, sec39),	SIZESEC39,	false	},
	{ 40,	(void *)offsetof (GAMEDATA, sec40),	SIZESEC40,	true	},
	{ 41,	(void *)offsetof (GAMEDATA, sec41),	SIZESEC41,	true	},
	{ 42,	(void *)offsetof (GAMEDATA, sec42),	SIZESEC42,	false	},
	{ 43,	(void *)offsetof (GAMEDATA, sec43),	SIZESEC43,	false	},
	{ 44,	(void *)offsetof (GAMEDATA, sec44),	SIZESEC44,	true	},
	{ 45,	(void *)offsetof (GAMEDATA, sec45),	SIZESEC45,	false	},
	{ 46,	(void *)offsetof (GAMEDATA, sec46),	SIZESEC46,	true	},
	{ 47,	(void *)offsetof (GAMEDATA, sec47),	SIZESEC47,	true	},
	{ 51,	(void *)offsetof (GAMEDATA, sec51),	SIZESEC51,	false	},
	{ 48,	(void *)offsetof (GAMEDATA, sec48),	SIZESEC48,	true	},
};

SECMAP *
gamedata_secmap (void)
{
	return (MAP);
}

GAMEDATA *
game_new (void)
{
	GAMEDATA	*ptr;
	int		i;

	ptr = safe_malloc (GAMEDATA);
	if (!ptr) return (NULL);

	clear_ptr (ptr);
	memcpy (ptr->MAP, &MAP, sizeof (MAP));

	for (i=0; i<SPWAW_SECTION_COUNT; i++) {
		DWORD p = (DWORD)ptr->MAP[i].ptr;
		p += (DWORD)ptr;
		ptr->MAP[i].ptr = (void *)p;
	}


	return (ptr);
}

SECMAP *
gamedata_section (SECMAP *map, int idx)
{
	SECMAP	*sp = NULL;
	int	i;

	CWVNULL (map, NULL);

	for (i=0; i<SPWAW_SECTION_COUNT; i++) {
		if (map[i].idx == idx) {
			sp = &(map[i]);
			break;
		}
	}
	return (sp);
}

static void
freegame (GAMEDATA **ptr)
{
	if (!ptr) return;
	if (*ptr) safe_free (*ptr);
}

static bool
open_gamefiles (const char *dir, int id, GAMEFILE *game)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	char		name[PATHMAX+1];
	HANDLE		hfd;

	if (!dir || !game) return (false);

	clear_ptr (game);
	game->cmt_fd = game->dat_fd = -1;

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "%s\\save%03.3d.cmt", dir, id);
	game->cmt_name = strdup (name); COOMGOTO (game->cmt_name, "game cmt filename", error);

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "%s\\save%03.3d.dat", dir, id);
	game->dat_name = strdup (name); COOMGOTO (game->cmt_name, "game dat filename", error);

	game->cmt_fd = open (game->cmt_name, O_RDONLY|O_BINARY);
	if (game->cmt_fd == -1) {
		ERROR1 ("failed to open game cmt file \"%s\" for reading", game->cmt_name);
		goto error;
	}
	log ("open_gamefiles: comment: name=\"%s\", fd=%d\n", game->cmt_name, game->cmt_fd);

	hfd = (void *)_get_osfhandle (game->cmt_fd);
	if (hfd != INVALID_HANDLE_VALUE) GetFileTime (hfd, NULL, NULL, &(game->cmt_date));

	game->dat_fd = open (game->dat_name, O_RDONLY|O_BINARY);
	if (game->dat_fd == -1) {
		ERROR1 ("failed to open game dat file \"%s\" for reading", game->dat_name);
		goto error;
	}
	log ("open_gamefiles: data: name=\"%s\", fd=%d\n", game->dat_name, game->dat_fd);

	hfd = (void *)_get_osfhandle (game->dat_fd);
	if (hfd != INVALID_HANDLE_VALUE)
		GetFileTime (hfd, NULL, NULL, &(game->dat_date));

	return (true);

error:
	if (game->dat_fd != -1) close (game->dat_fd);
	if (game->cmt_fd != -1) close (game->cmt_fd);
	if (game->dat_name) free (game->dat_name);
	if (game->cmt_name) free (game->cmt_name);
	clear_ptr (game);
	return (false);
}

static bool
create_gamefiles (const char *dir, int id, GAMEFILE *game)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	char		name[PATHMAX+1];
	HANDLE		hfd;

	if (!dir || !game) return (false);

	clear_ptr (game);
	game->cmt_fd = game->dat_fd = -1;

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "%s\\save%03.3d.cmt", dir, id);
	game->cmt_name = strdup (name); COOMGOTO (game->cmt_name, "game cmt filename", error);

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "%s\\save%03.3d.dat", dir, id);
	game->dat_name = strdup (name); COOMGOTO (game->cmt_name, "game dat filename", error);

	game->cmt_fd = open (game->cmt_name, O_WRONLY|O_BINARY|O_CREAT|O_TRUNC, 0666);
	if (game->cmt_fd == -1) {
		ERROR1 ("failed to open game cmt file \"%s\" for writing", game->cmt_name);
		goto error;
	}
	log ("create_gamefiles: comment: name=\"%s\", fd=%d\n", game->cmt_name, game->cmt_fd);

	hfd = (void *)_get_osfhandle (game->cmt_fd);
	if (hfd != INVALID_HANDLE_VALUE) GetFileTime (hfd, NULL, NULL, &(game->cmt_date));

	game->dat_fd = open (game->dat_name, O_WRONLY|O_BINARY|O_CREAT|O_TRUNC, 0666);
	if (game->dat_fd == -1) {
		ERROR1 ("failed to open game dat file \"%s\" for writing", game->dat_name);
		goto error;
	}
	log ("open_gamefiles: data: name=\"%s\", fd=%d\n", game->dat_name, game->dat_fd);

	hfd = (void *)_get_osfhandle (game->dat_fd);
	if (hfd != INVALID_HANDLE_VALUE)
		GetFileTime (hfd, NULL, NULL, &(game->dat_date));

	return (true);

error:
	if (game->dat_fd != -1) close (game->dat_fd);
	if (game->cmt_fd != -1) close (game->cmt_fd);
	if (game->dat_name) free (game->dat_name);
	if (game->cmt_name) free (game->cmt_name);
	clear_ptr (game);
	return (false);
}

static bool
close_gamefiles (GAMEFILE *game)
{
	if (!game) return (false);

	if (game->dat_fd != -1) close (game->dat_fd);
	if (game->cmt_fd != -1) close (game->cmt_fd);
	if (game->dat_name) free (game->dat_name);
	if (game->cmt_name) free (game->cmt_name);
	clear_ptr (game);
	game->cmt_fd = game->dat_fd = -1;

	return (true);
}

static void
setup_info (GAMEINFO *info, char *filename, FILETIME filedate, STRUCT37 *gamedata, CMTDATA *gamecmt, STRUCT35 *formdata)
{
	char		*p, *q;
	SPWAW_DATE	base, date;
	SPWAW_PERIOD	add;
	SPWAW_ERROR	rc;
	FULIST		ful1, ful2;
	USHORT		cfcnt;

	if (!info) return;
	clear_ptr (info);

	sec37_prepare (gamedata);

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
	rc = sec35_detection (formdata, ful1, ful2);
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

GAMEDATA *
game_load_full (const char *dir, unsigned int id, GAMEINFO *info)
{
	GAMEFILE	game;
	GAMEDATA	*data = NULL;
	bool		cmt_rc, dat_rc;

	log ("game_load_full (dir=\"%s\", id=%u, info=0x%8.8x)\n", dir, id, info);

	if (info) clear_ptr (info);

	if (!open_gamefiles (dir, id, &game)) return (NULL);

	data = game_new(); COOMRET (data, "GAMEDATA", NULL);

	cmt_rc = gamedata_load_cmt (&game, &(data->cmt));
	dat_rc = gamedata_load_all (&game, data);

	if (!cmt_rc) {
		ERROR0 ("failed to read game comment data");
		freegame (&data);
	} else if (!dat_rc) {
		ERROR0 ("failed to decompress game data");
		freegame (&data);
	}

	if (data) {
		setup_info (info, game.dat_name, game.dat_date, &(data->sec37), &(data->cmt), &(data->sec35));
		data->type = info->type;
	}

	close_gamefiles (&game);

	return (data);
}

bool
game_save_full (GAMEDATA *src, const char *dir, unsigned int id)
{
	GAMEFILE	game;
	bool		cmt_rc, dat_rc;

	log ("game_save_full (src=0x%8.8x, dir=\"%s\", id=%u)\n", src, dir, id);

	if (!create_gamefiles (dir, id, &game)) return false;

	cmt_rc = gamedata_save_cmt (&(src->cmt), &game);
	dat_rc = gamedata_save_all (src, &game);

	close_gamefiles (&game);

	if (!cmt_rc) {
		ERROR0 ("failed to write game comment data");
		return (false);
	} else if (!dat_rc) {
		ERROR0 ("failed to write game data");
		return (false);
	}

	return (true);
}

void
game_free (GAMEDATA **game)
{
	if (!game || !*game) return;
	free (*game); *game = NULL;
}

bool
game_load_cmt (const char *dir, unsigned int id, CMTDATA *dst)
{
	GAMEFILE	game;
	bool		rc;

	if (!open_gamefiles (dir, id, &game)) return (false);

	rc = gamedata_load_cmt (&game, dst);
	close_gamefiles (&game);
	if (!rc) ERROR0 ("failed to load game comment");

	return (rc);
}

bool
game_load_section (const char *dir, unsigned int id, int sec, void *dst, unsigned long len)
{
	GAMEFILE	game;
	bool		rc;

	if (!open_gamefiles (dir, id, &game)) return (false);

	rc = gamedata_load_section (&game, sec, dst, len);
	close_gamefiles (&game);

	if (!rc) ERROR0 ("failed to load game section");

	return (rc);
}

bool
game_load_info (const char *dir, unsigned int id, GAMEINFO *info)
{
	GAMEFILE	game;
	bool		grc = true, rc;
	STRUCT37	struct37;
	STRUCT35	struct35;
	CMTDATA		cmt;

	if (!info) return (false);
	clear_ptr (info);

	if (!open_gamefiles (dir, id, &game)) {
		ERROR0 ("failed to open gamefiles");
		return (false);
	}

	rc = game_load_section (dir, id, 37, &struct37, sizeof (struct37)); grc = grc && rc;
	if (!rc) {
		ERROR0 ("failed to load section #37 game data");
		memset (&struct37, 0, sizeof (struct37));
	}

	rc = game_load_section (dir, id, 35, &struct35, sizeof (struct35)); grc = grc && rc;
	if (!rc) {
		ERROR0 ("failed to load section #35 game data");
		memset (&struct35, 0, sizeof (struct35));
	}

	rc = gamedata_load_cmt (&game, &cmt); grc = grc && rc;
	if (!rc) {
		ERROR0 ("failed to load game comment");
		memset (&cmt, 0, sizeof (cmt));
	}

	setup_info (info, game.dat_name, game.dat_date, &struct37, &cmt, &struct35);

	close_gamefiles (&game);

	if (!grc) ERROR0 ("failed to load all game info");

	return (grc);
}
