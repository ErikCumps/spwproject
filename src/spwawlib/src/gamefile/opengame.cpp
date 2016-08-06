/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/gamefile.h"
#include "utils/log.h"
#include "common/internal.h"

#define	PATHMAX	256

static SECMAP	MAP[SECMAXID+1] = {
	{  0,	(void *)offsetof (GAMEDATA, sec00),	SIZESEC00	},
	{  1,	(void *)offsetof (GAMEDATA, sec01),	SIZESEC01	},
	{  2,	(void *)offsetof (GAMEDATA, sec02),	SIZESEC02	},
	{  3,	(void *)offsetof (GAMEDATA, sec03),	SIZESEC03	},
	{  4,	(void *)offsetof (GAMEDATA, sec04),	SIZESEC04	},
	{  5,	(void *)offsetof (GAMEDATA, sec05),	SIZESEC05	},
	{  6,	(void *)offsetof (GAMEDATA, sec06),	SIZESEC06	},
	{  7,	(void *)offsetof (GAMEDATA, sec07),	SIZESEC07	},
	{  8,	(void *)offsetof (GAMEDATA, sec08),	SIZESEC08	},
	{  9,	(void *)offsetof (GAMEDATA, sec09),	SIZESEC09	},
	{ 10,	(void *)offsetof (GAMEDATA, sec10),	SIZESEC10	},
	{ 11,	(void *)offsetof (GAMEDATA, sec11),	SIZESEC11	},
	{ 12,	(void *)offsetof (GAMEDATA, sec12),	SIZESEC12	},
	{ 13,	(void *)offsetof (GAMEDATA, sec13),	SIZESEC13	},
	{ 14,	(void *)offsetof (GAMEDATA, sec14),	SIZESEC14	},
	{ 15,	(void *)offsetof (GAMEDATA, sec15),	SIZESEC15	},
	{ 16,	(void *)offsetof (GAMEDATA, sec16),	SIZESEC16	},
	{ 17,	(void *)offsetof (GAMEDATA, sec17),	SIZESEC17	},
	{ 18,	(void *)offsetof (GAMEDATA, sec18),	SIZESEC18	},
	{ 19,	(void *)offsetof (GAMEDATA, sec19),	SIZESEC19	},
	{ 20,	(void *)offsetof (GAMEDATA, sec20),	SIZESEC20	},
	{ 21,	(void *)offsetof (GAMEDATA, sec21),	SIZESEC21	},
	{ 22,	(void *)offsetof (GAMEDATA, sec22),	SIZESEC22	},
	{ 23,	(void *)offsetof (GAMEDATA, sec23),	SIZESEC23	},
	{ 24,	(void *)offsetof (GAMEDATA, sec24),	SIZESEC24	},
	{ 25,	(void *)offsetof (GAMEDATA, sec25),	SIZESEC25	},
	{ 26,	(void *)offsetof (GAMEDATA, sec26),	SIZESEC26	},
	{ 27,	(void *)offsetof (GAMEDATA, sec27),	SIZESEC27	},
	{ 28,	(void *)offsetof (GAMEDATA, sec28),	SIZESEC28	},
	{ 29,	(void *)offsetof (GAMEDATA, sec29),	SIZESEC29	},
	{ 30,	(void *)offsetof (GAMEDATA, sec30),	SIZESEC30	},
	{ 31,	(void *)offsetof (GAMEDATA, sec31),	SIZESEC31	},
	{ 32,	(void *)offsetof (GAMEDATA, sec32),	SIZESEC32	},
	{ 33,	(void *)offsetof (GAMEDATA, sec33),	SIZESEC33	},
	{ 34,	(void *)offsetof (GAMEDATA, sec34),	SIZESEC34	},
	{ 35,	(void *)offsetof (GAMEDATA, sec35),	SIZESEC35	},
	{ 36,	(void *)offsetof (GAMEDATA, sec36),	SIZESEC36	},
	{ 37,	(void *)offsetof (GAMEDATA, sec37),	SIZESEC37	},
	{ 38,	(void *)offsetof (GAMEDATA, sec38),	SIZESEC38	},
	{ 39,	(void *)offsetof (GAMEDATA, sec39),	SIZESEC39	},
	{ 40,	(void *)offsetof (GAMEDATA, sec40),	SIZESEC40	},
	{ 41,	(void *)offsetof (GAMEDATA, sec41),	SIZESEC41	},
	{ 42,	(void *)offsetof (GAMEDATA, sec42),	SIZESEC42	},
	{ 43,	(void *)offsetof (GAMEDATA, sec43),	SIZESEC43	},
	{ 44,	(void *)offsetof (GAMEDATA, sec44),	SIZESEC44	},
	{ 45,	(void *)offsetof (GAMEDATA, sec45),	SIZESEC45	},
	{ 46,	(void *)offsetof (GAMEDATA, sec46),	SIZESEC46	},
	{ 47,	(void *)offsetof (GAMEDATA, sec47),	SIZESEC47	},
	{ 48,	(void *)offsetof (GAMEDATA, sec48),	SIZESEC48	},
	{ 49,	NULL,					0		},
	{ 50,	NULL,					0		},
	{ 51,	(void *)offsetof (GAMEDATA, sec51),	SIZESEC51	},
};

SECMAP *
gamedata_secmap (void)
{
	return (MAP);
}

static void
newgame (GAMEDATA **ptr)
{
	int	i;
	DWORD	p;

	if (!ptr) return;

	*ptr = safe_malloc (GAMEDATA);
	if (!*ptr) return;

	clear_ptr (*ptr);
	memcpy ((*ptr)->MAP, &MAP, sizeof (MAP));
	for (i=0; i<=SECMAXID; i++) {
		if ((*ptr)->MAP[i].size == 0) continue;

		p = (DWORD)(*ptr)->MAP[i].ptr;
		p += (DWORD)(*ptr);
		(*ptr)->MAP[i].ptr = (void *)p;
	}
}

static void
freegame (GAMEDATA **ptr)
{
	if (!ptr) return;
	if (*ptr) { free (*ptr); *ptr = NULL; }
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
	snprintf (name, sizeof (name) - 1, "%s\\save%03.3u.cmt", dir, id);
	game->cmt_name = strdup (name); COOMGOTO (game->cmt_name, "game cmt filename", error);

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "%s\\save%03.3u.dat", dir, id);
	game->dat_name = strdup (name); COOMGOTO (game->cmt_name, "game dat filename", error);

	game->cmt_fd = open (game->cmt_name, O_RDONLY|O_BINARY);
	if (game->cmt_fd == -1) {
		ERROR1 ("failed to open game cmt file \"%s\" for reading", game->cmt_name);
		goto error;
	}
	log ("open_gamefile: comment: name=\"%s\", fd=%d\n", game->cmt_name, game->cmt_fd);

	hfd = (void *)_get_osfhandle (game->cmt_fd);
	if (hfd != INVALID_HANDLE_VALUE) GetFileTime (hfd, NULL, NULL, &(game->cmt_date));

	game->dat_fd = open (game->dat_name, O_RDONLY|O_BINARY);
	if (game->dat_fd == -1) {
		ERROR1 ("failed to open game dat file \"%s\" for reading", game->dat_name);
		goto error;
	}
	log ("open_gamefile: data: name=\"%s\", fd=%d\n", game->dat_name, game->dat_fd);

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
close_gamefile (GAMEFILE *game)
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
setup_info (GAMEINFO *info, char *filename, FILETIME filedate, STRUCT37 *gamedata, CMTDATA *gamecmt)
{
	char		*p, *q;
	SPWAW_DATE	base, date;
	SPWAW_PERIOD	add;

	if (!info) return;
	clear_ptr (info);

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

	memset (&base, 0, sizeof (base));
	base.year  = gamedata->u.d.data.Ygame + 1900;
	base.month = gamedata->u.d.data.Mgame;
	base.day   = gamedata->u.d.data.Dgame;
	base.hour  = gamedata->u.d.data.Hgame;
	add.stamp  = gamedata->u.d.data.turn * SPWAW_MINSPERTURN;
	SPWAW_date_add (&base, &add, &date);
	snprintf (info->stamp, sizeof (info->stamp) - 1, "%4.4u/%02.2u/%02.2u %02.2u:%02.2u, turn %u",
		date.year, date.month, date.day, date.hour, date.minute, gamedata->u.d.data.turn);

	memcpy (info->location, gamedata->u.d.data.location, sizeof (gamedata->u.d.data.location));
	memcpy (info->comment, gamecmt->title, sizeof (gamecmt->title));
}

GAMEDATA *
game_load_full (const char *dir, unsigned int id, GAMEINFO *info)
{
	GAMEFILE	game;
	GAMEDATA	*data = NULL;
	bool		cmt_rc, dat_rc;

	log ("opengame (dir=\"%s\", id=%u, info=0x%8.8x)\n", dir, id, info);

	if (info) clear_ptr (info);

	if (!open_gamefiles (dir, id, &game)) return (NULL);

	newgame (&data); COOMRET (data, "GAMEDATA", NULL);

	cmt_rc = gamedata_load_cmt (&game, &(data->cmt));
	dat_rc = gamedata_load_all (&game, data);

	if (!cmt_rc) {
		ERROR0 ("failed to read game comment data");
		freegame (&data);
	} else if (!dat_rc) {
		ERROR0 ("failed to decompress game data");
		freegame (&data);
	}

	if (data) setup_info (info, game.dat_name, game.dat_date, &(data->sec37), &(data->cmt));
	close_gamefile (&game);

	return (data);
}

void
game_free (GAMEDATA *game)
{
	if (!game) return;
	free (game); game = NULL;
}

bool
game_load_cmt (const char *dir, unsigned int id, CMTDATA *dst)
{
	GAMEFILE	game;
	bool		rc;

	if (!open_gamefiles (dir, id, &game)) return (false);

	rc = gamedata_load_cmt (&game, dst);
	close_gamefile (&game);
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
	close_gamefile (&game);

	if (!rc) ERROR0 ("failed to load game section");

	return (rc);
}

bool
game_load_info (const char *dir, unsigned int id, GAMEINFO *info)
{
	GAMEFILE	game;
	bool		grc = true, rc;
	STRUCT37	struct37;
	CMTDATA		cmt;

	if (!info) return (false);
	clear_ptr (info);

	if (!open_gamefiles (dir, id, &game)) {
		ERROR0 ("failed to open gamefiles");
		return (false);
	}

	rc = game_load_section (dir, id, 37, &struct37, sizeof (struct37)); grc = grc && rc;
	if (!rc) {
		ERROR0 ("failed to load game data");
		memset (&struct37, 0, sizeof (struct37));
	}

	rc = gamedata_load_cmt (&game, &cmt); grc = grc && rc;
	if (!rc) {
		ERROR0 ("failed to load game comment");
		memset (&cmt, 0, sizeof (cmt));
	}

	setup_info (info, game.dat_name, game.dat_date, &struct37, &cmt);

	close_gamefile (&game);

	if (!grc) ERROR0 ("failed to load all game info");

	return (grc);
}
