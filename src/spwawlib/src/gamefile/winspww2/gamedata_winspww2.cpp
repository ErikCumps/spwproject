/** \file
 * The SPWaW Library - game data handling - winSPWW2 game data.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/winspww2/gamedata_winspww2.h"
#include "gamefile/winspww2/defines_winspww2.h"
#include "common/internal.h"

static SECMAPEL	MAPLIST[WINSPWW2_SECTION_COUNT] = {
	{  0,	(void *)offsetof (GAMEDATA_WINSPWW2, sec00),	WINSPWW2_SIZESEC00,	false,	false	},
	{  1,	(void *)offsetof (GAMEDATA_WINSPWW2, sec01),	WINSPWW2_SIZESEC01,	true,	false	},
	{  2,	(void *)offsetof (GAMEDATA_WINSPWW2, sec02),	WINSPWW2_SIZESEC02,	false,	false	},
	{  3,	(void *)offsetof (GAMEDATA_WINSPWW2, sec03),	WINSPWW2_SIZESEC03,	true,	false	},
	{  4,	(void *)offsetof (GAMEDATA_WINSPWW2, sec04),	WINSPWW2_SIZESEC04,	true,	false	},
	{  5,	(void *)offsetof (GAMEDATA_WINSPWW2, sec05),	WINSPWW2_SIZESEC05,	true,	false	},
	{  6,	(void *)offsetof (GAMEDATA_WINSPWW2, sec06),	WINSPWW2_SIZESEC06,	true,	false	},
	{  7,	(void *)offsetof (GAMEDATA_WINSPWW2, sec07),	WINSPWW2_SIZESEC07,	true,	false	},
	{  8,	(void *)offsetof (GAMEDATA_WINSPWW2, sec08),	WINSPWW2_SIZESEC08,	true,	false	},
	{  9,	(void *)offsetof (GAMEDATA_WINSPWW2, sec09),	WINSPWW2_SIZESEC09,	true,	false	},
	{ 10,	(void *)offsetof (GAMEDATA_WINSPWW2, sec10),	WINSPWW2_SIZESEC10,	false,	false	},
	{ 11,	(void *)offsetof (GAMEDATA_WINSPWW2, sec11),	WINSPWW2_SIZESEC11,	true,	false	},
	{ 12,	(void *)offsetof (GAMEDATA_WINSPWW2, sec12),	WINSPWW2_SIZESEC12,	false,	false	},
	{ 13,	(void *)offsetof (GAMEDATA_WINSPWW2, sec13),	WINSPWW2_SIZESEC13,	false,	false	},
	{ 14,	(void *)offsetof (GAMEDATA_WINSPWW2, sec14),	WINSPWW2_SIZESEC14,	false,	false	},
	{ 15,	(void *)offsetof (GAMEDATA_WINSPWW2, sec15),	WINSPWW2_SIZESEC15,	false,	false	},
	{ 16,	(void *)offsetof (GAMEDATA_WINSPWW2, sec16),	WINSPWW2_SIZESEC16,	false,	false	},
	{ 17,	(void *)offsetof (GAMEDATA_WINSPWW2, sec17),	WINSPWW2_SIZESEC17,	true,	false	},
	{ 18,	(void *)offsetof (GAMEDATA_WINSPWW2, sec18),	WINSPWW2_SIZESEC18,	true,	false	},
	{ 19,	(void *)offsetof (GAMEDATA_WINSPWW2, sec19),	WINSPWW2_SIZESEC19,	true,	false	},
	{ 20,	(void *)offsetof (GAMEDATA_WINSPWW2, sec20),	WINSPWW2_SIZESEC20,	false,	false	},
	{ 21,	(void *)offsetof (GAMEDATA_WINSPWW2, sec21),	WINSPWW2_SIZESEC21,	false,	false	},
	{ 22,	(void *)offsetof (GAMEDATA_WINSPWW2, sec22),	WINSPWW2_SIZESEC22,	false,	false	},
	{ 23,	(void *)offsetof (GAMEDATA_WINSPWW2, sec23),	WINSPWW2_SIZESEC23,	true,	false	},
	{ 24,	(void *)offsetof (GAMEDATA_WINSPWW2, sec24),	WINSPWW2_SIZESEC24,	true,	false	},
	{ 25,	(void *)offsetof (GAMEDATA_WINSPWW2, sec25),	WINSPWW2_SIZESEC25,	true,	false	},
	{ 26,	(void *)offsetof (GAMEDATA_WINSPWW2, sec26),	WINSPWW2_SIZESEC26,	true,	false	},
	{ 27,	(void *)offsetof (GAMEDATA_WINSPWW2, sec27),	WINSPWW2_SIZESEC27,	false,	false	},
	{ 28,	(void *)offsetof (GAMEDATA_WINSPWW2, sec28),	WINSPWW2_SIZESEC28,	true,	false	},
	{ 29,	(void *)offsetof (GAMEDATA_WINSPWW2, sec29),	WINSPWW2_SIZESEC29,	true,	false	},
	{ 30,	(void *)offsetof (GAMEDATA_WINSPWW2, sec30),	WINSPWW2_SIZESEC30,	true,	false	},
	{ 31,	(void *)offsetof (GAMEDATA_WINSPWW2, sec31),	WINSPWW2_SIZESEC31,	true,	false	},
	{ 32,	(void *)offsetof (GAMEDATA_WINSPWW2, sec32),	WINSPWW2_SIZESEC32,	true,	false	},
	{ 33,	(void *)offsetof (GAMEDATA_WINSPWW2, sec33),	WINSPWW2_SIZESEC33,	true,	false	},
	{ 34,	(void *)offsetof (GAMEDATA_WINSPWW2, sec34),	WINSPWW2_SIZESEC34,	true,	false	},
	{ 35,	(void *)offsetof (GAMEDATA_WINSPWW2, sec35),	WINSPWW2_SIZESEC35,	true,	false	},
	{ 36,	(void *)offsetof (GAMEDATA_WINSPWW2, sec36),	WINSPWW2_SIZESEC36,	true,	false	},
	{ 37,	(void *)offsetof (GAMEDATA_WINSPWW2, sec37),	WINSPWW2_SIZESEC37,	true,	false	},
	{ 38,	(void *)offsetof (GAMEDATA_WINSPWW2, sec38),	WINSPWW2_SIZESEC38,	false,	false	},
	{ 39,	(void *)offsetof (GAMEDATA_WINSPWW2, sec39),	WINSPWW2_SIZESEC39,	false,	false	},
	{ 40,	(void *)offsetof (GAMEDATA_WINSPWW2, sec40),	WINSPWW2_SIZESEC40,	true,	false	},
	{ 41,	(void *)offsetof (GAMEDATA_WINSPWW2, sec41),	WINSPWW2_SIZESEC41,	true,	false	},
	{ 42,	(void *)offsetof (GAMEDATA_WINSPWW2, sec42),	WINSPWW2_SIZESEC42,	false,	false	},
	{ 43,	(void *)offsetof (GAMEDATA_WINSPWW2, sec43),	WINSPWW2_SIZESEC43,	false,	false	},
	{ 51,	(void *)offsetof (GAMEDATA_WINSPWW2, sec51),	WINSPWW2_SIZESEC51,	false,	false	},
	{ 52,	(void *)offsetof (GAMEDATA_WINSPWW2, sec52),	WINSPWW2_SIZESEC52,	true,	false	},
	{ 53,	(void *)offsetof (GAMEDATA_WINSPWW2, sec53),	WINSPWW2_SIZESEC53,	false,	false	},
	{ 54,	(void *)offsetof (GAMEDATA_WINSPWW2, sec54),	WINSPWW2_SIZESEC54,	true,	false	},
	{ 56,	(void *)offsetof (GAMEDATA_WINSPWW2, sec56),	WINSPWW2_SIZESEC56,	true,	false	},
	{ 57,	(void *)offsetof (GAMEDATA_WINSPWW2, sec57),	WINSPWW2_SIZESEC57,	true,	false	},
	{ 59,	(void *)offsetof (GAMEDATA_WINSPWW2, sec59),	WINSPWW2_SIZESEC59,	true,	false	},
	{ 48,	(void *)offsetof (GAMEDATA_WINSPWW2, sec48),	WINSPWW2_SIZESEC48,	true,	false	},
	{ 49,	0,						0,			true,	true	},
	{ 50,	0,						0,			true,	true	},
};

static SECMAP	MAP = { WINSPWW2_SECTION_COUNT, MAPLIST };

static bool
validate_winspww2_gamedata_structures (void)
{
	GAMEDATA_WINSPWW2	*p = NULL; //fixme: compiler warning C4189, variable not referenced
	assert (sizeof(p->sec00.u.raw) == sizeof(p->sec00.u.d));
	assert (sizeof(p->sec01.u.raw) == sizeof(p->sec01.u.d));
	assert (sizeof(p->sec02.u.raw) == sizeof(p->sec02.u.d));
	assert (sizeof(p->sec03.u.raw) == sizeof(p->sec03.u.d));
	assert (sizeof(p->sec04.u.raw) == sizeof(p->sec04.u.d));
	assert (sizeof(p->sec05.u.raw) == sizeof(p->sec05.u.d));
	assert (sizeof(p->sec06.u.raw) == sizeof(p->sec06.u.d));
	assert (sizeof(p->sec07.u.raw) == sizeof(p->sec07.u.d));
	assert (sizeof(p->sec08.u.raw) == sizeof(p->sec08.u.d));
	assert (sizeof(p->sec09.u.raw) == sizeof(p->sec09.u.d));
	assert (sizeof(p->sec10.u.raw) == sizeof(p->sec10.u.d));
	assert (sizeof(p->sec11.u.raw) == sizeof(p->sec11.u.d));
	assert (sizeof(p->sec12.u.raw) == sizeof(p->sec12.u.d));
	assert (sizeof(p->sec13.u.raw) == sizeof(p->sec13.u.d));
	assert (sizeof(p->sec14.u.raw) == sizeof(p->sec14.u.d));
	assert (sizeof(p->sec15.u.raw) == sizeof(p->sec15.u.d));
	assert (sizeof(p->sec16.u.raw) == sizeof(p->sec16.u.d));
	assert (sizeof(p->sec17.u.raw) == sizeof(p->sec17.u.d));
	assert (sizeof(p->sec18.u.raw) == sizeof(p->sec18.u.d));
	assert (sizeof(p->sec19.u.raw) == sizeof(p->sec19.u.d));
	assert (sizeof(p->sec20.u.raw) == sizeof(p->sec20.u.d));
	assert (sizeof(p->sec21.u.raw) == sizeof(p->sec21.u.d));
	assert (sizeof(p->sec22.u.raw) == sizeof(p->sec22.u.d));
	assert (sizeof(p->sec23.u.raw) == sizeof(p->sec23.u.d));
	assert (sizeof(p->sec24.u.raw) == sizeof(p->sec24.u.d));
	assert (sizeof(p->sec25.u.raw) == sizeof(p->sec25.u.d));
	assert (sizeof(p->sec26.u.raw) == sizeof(p->sec26.u.d));
	assert (sizeof(p->sec27.u.raw) == sizeof(p->sec27.u.d));
	assert (sizeof(p->sec28.u.raw) == sizeof(p->sec28.u.d));
	assert (sizeof(p->sec29.u.raw) == sizeof(p->sec29.u.d));
	assert (sizeof(p->sec30.u.raw) == sizeof(p->sec30.u.d));
	assert (sizeof(p->sec31.u.raw) == sizeof(p->sec31.u.d));
	assert (sizeof(p->sec32.u.raw) == sizeof(p->sec32.u.d));
	assert (sizeof(p->sec33.u.raw) == sizeof(p->sec33.u.d));
	assert (sizeof(p->sec34.u.raw) == sizeof(p->sec34.u.d));
	assert (sizeof(p->sec35.u.raw) == sizeof(p->sec35.u.d));
	assert (sizeof(p->sec36.u.raw) == sizeof(p->sec36.u.d));
	assert (sizeof(p->sec37.u.raw) == sizeof(p->sec37.u.d));
	assert (sizeof(p->sec38.u.raw) == sizeof(p->sec38.u.d));
	assert (sizeof(p->sec39.u.raw) == sizeof(p->sec39.u.d));
	assert (sizeof(p->sec40.u.raw) == sizeof(p->sec40.u.d));
	assert (sizeof(p->sec41.u.raw) == sizeof(p->sec41.u.d));
	assert (sizeof(p->sec42.u.raw) == sizeof(p->sec42.u.d));
	assert (sizeof(p->sec43.u.raw) == sizeof(p->sec43.u.d));
	assert (sizeof(p->sec51.u.raw) == sizeof(p->sec51.u.d));
	assert (sizeof(p->sec51.u.raw) == sizeof(p->sec51.u.d));
	assert (sizeof(p->sec52.u.raw) == sizeof(p->sec52.u.d));
	assert (sizeof(p->sec53.u.raw) == sizeof(p->sec53.u.d));
	assert (sizeof(p->sec54.u.raw) == sizeof(p->sec54.u.d));
	assert (sizeof(p->sec56.u.raw) == sizeof(p->sec56.u.d));
	assert (sizeof(p->sec57.u.raw) == sizeof(p->sec57.u.d));
	assert (sizeof(p->sec59.u.raw) == sizeof(p->sec59.u.d));
	assert (sizeof(p->sec48.u.raw) == sizeof(p->sec48.u.d));
	return (true);
}
static bool winspww2_gamedata_structures_ok = validate_winspww2_gamedata_structures();

SECMAP *
gamedata_winspww2_SECMAP (void)
{
	return (&MAP);
}

void *
gamedata_winspww2_new_data (void)
{
	return ((void *)safe_malloc (GAMEDATA_WINSPWW2));
}

void
gamedata_winspww2_free_data (void **data)
{
	void	*p;

	CWRNULL (data);
	p = *data; *data = NULL;

	if (p) safe_free (p);
}
