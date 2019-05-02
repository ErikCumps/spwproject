/** \file
 * The SPWaW Library - game data handling - SPWaW game data.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/spwaw/gamedata_spwaw.h"
#include "gamefile/spwaw/defines_spwaw.h"
#include "common/internal.h"

static SECMAPEL	MAPLIST[SPWAW_SECTION_COUNT] = {
	{  0,	(void *)offsetof (GAMEDATA_SPWAW, sec00),	SPWAW_SIZESEC00,	false,	false	},
	{  1,	(void *)offsetof (GAMEDATA_SPWAW, sec01),	SPWAW_SIZESEC01,	true,	false	},
	{  2,	(void *)offsetof (GAMEDATA_SPWAW, sec02),	SPWAW_SIZESEC02,	false,	false	},
	{  3,	(void *)offsetof (GAMEDATA_SPWAW, sec03),	SPWAW_SIZESEC03,	true,	false	},
	{  4,	(void *)offsetof (GAMEDATA_SPWAW, sec04),	SPWAW_SIZESEC04,	true,	false	},
	{  5,	(void *)offsetof (GAMEDATA_SPWAW, sec05),	SPWAW_SIZESEC05,	true,	false	},
	{  6,	(void *)offsetof (GAMEDATA_SPWAW, sec06),	SPWAW_SIZESEC06,	true,	false	},
	{  7,	(void *)offsetof (GAMEDATA_SPWAW, sec07),	SPWAW_SIZESEC07,	true,	false	},
	{  8,	(void *)offsetof (GAMEDATA_SPWAW, sec08),	SPWAW_SIZESEC08,	true,	false	},
	{  9,	(void *)offsetof (GAMEDATA_SPWAW, sec09),	SPWAW_SIZESEC09,	true,	false	},
	{ 10,	(void *)offsetof (GAMEDATA_SPWAW, sec10),	SPWAW_SIZESEC10,	false,	false	},
	{ 11,	(void *)offsetof (GAMEDATA_SPWAW, sec11),	SPWAW_SIZESEC11,	true,	false	},
	{ 12,	(void *)offsetof (GAMEDATA_SPWAW, sec12),	SPWAW_SIZESEC12,	false,	false	},
	{ 13,	(void *)offsetof (GAMEDATA_SPWAW, sec13),	SPWAW_SIZESEC13,	false,	false	},
	{ 14,	(void *)offsetof (GAMEDATA_SPWAW, sec14),	SPWAW_SIZESEC14,	false,	false	},
	{ 15,	(void *)offsetof (GAMEDATA_SPWAW, sec15),	SPWAW_SIZESEC15,	false,	false	},
	{ 16,	(void *)offsetof (GAMEDATA_SPWAW, sec16),	SPWAW_SIZESEC16,	false,	false	},
	{ 17,	(void *)offsetof (GAMEDATA_SPWAW, sec17),	SPWAW_SIZESEC17,	true,	false	},
	{ 18,	(void *)offsetof (GAMEDATA_SPWAW, sec18),	SPWAW_SIZESEC18,	true,	false	},
	{ 19,	(void *)offsetof (GAMEDATA_SPWAW, sec19),	SPWAW_SIZESEC19,	true,	false	},
	{ 20,	(void *)offsetof (GAMEDATA_SPWAW, sec20),	SPWAW_SIZESEC20,	false,	false	},
	{ 21,	(void *)offsetof (GAMEDATA_SPWAW, sec21),	SPWAW_SIZESEC21,	false,	false	},
	{ 22,	(void *)offsetof (GAMEDATA_SPWAW, sec22),	SPWAW_SIZESEC22,	false,	false	},
	{ 23,	(void *)offsetof (GAMEDATA_SPWAW, sec23),	SPWAW_SIZESEC23,	true,	false	},
	{ 24,	(void *)offsetof (GAMEDATA_SPWAW, sec24),	SPWAW_SIZESEC24,	true,	false	},
	{ 25,	(void *)offsetof (GAMEDATA_SPWAW, sec25),	SPWAW_SIZESEC25,	true,	false	},
	{ 26,	(void *)offsetof (GAMEDATA_SPWAW, sec26),	SPWAW_SIZESEC26,	true,	false	},
	{ 27,	(void *)offsetof (GAMEDATA_SPWAW, sec27),	SPWAW_SIZESEC27,	false,	false	},
	{ 28,	(void *)offsetof (GAMEDATA_SPWAW, sec28),	SPWAW_SIZESEC28,	true,	false	},
	{ 29,	(void *)offsetof (GAMEDATA_SPWAW, sec29),	SPWAW_SIZESEC29,	true,	false	},
	{ 30,	(void *)offsetof (GAMEDATA_SPWAW, sec30),	SPWAW_SIZESEC30,	true,	false	},
	{ 31,	(void *)offsetof (GAMEDATA_SPWAW, sec31),	SPWAW_SIZESEC31,	true,	false	},
	{ 32,	(void *)offsetof (GAMEDATA_SPWAW, sec32),	SPWAW_SIZESEC32,	true,	false	},
	{ 33,	(void *)offsetof (GAMEDATA_SPWAW, sec33),	SPWAW_SIZESEC33,	true,	false	},
	{ 34,	(void *)offsetof (GAMEDATA_SPWAW, sec34),	SPWAW_SIZESEC34,	true,	false	},
	{ 35,	(void *)offsetof (GAMEDATA_SPWAW, sec35),	SPWAW_SIZESEC35,	true,	false	},
	{ 36,	(void *)offsetof (GAMEDATA_SPWAW, sec36),	SPWAW_SIZESEC36,	true,	false	},
	{ 37,	(void *)offsetof (GAMEDATA_SPWAW, sec37),	SPWAW_SIZESEC37,	true,	false	},
	{ 38,	(void *)offsetof (GAMEDATA_SPWAW, sec38),	SPWAW_SIZESEC38,	false,	false	},
	{ 39,	(void *)offsetof (GAMEDATA_SPWAW, sec39),	SPWAW_SIZESEC39,	false,	false	},
	{ 40,	(void *)offsetof (GAMEDATA_SPWAW, sec40),	SPWAW_SIZESEC40,	true,	false	},
	{ 41,	(void *)offsetof (GAMEDATA_SPWAW, sec41),	SPWAW_SIZESEC41,	true,	false	},
	{ 42,	(void *)offsetof (GAMEDATA_SPWAW, sec42),	SPWAW_SIZESEC42,	false,	false	},
	{ 43,	(void *)offsetof (GAMEDATA_SPWAW, sec43),	SPWAW_SIZESEC43,	false,	false	},
	{ 44,	(void *)offsetof (GAMEDATA_SPWAW, sec44),	SPWAW_SIZESEC44,	true,	false	},
	{ 45,	(void *)offsetof (GAMEDATA_SPWAW, sec45),	SPWAW_SIZESEC45,	false,	false	},
	{ 46,	(void *)offsetof (GAMEDATA_SPWAW, sec46),	SPWAW_SIZESEC46,	true,	false	},
	{ 47,	(void *)offsetof (GAMEDATA_SPWAW, sec47),	SPWAW_SIZESEC47,	true,	false	},
	{ 51,	(void *)offsetof (GAMEDATA_SPWAW, sec51),	SPWAW_SIZESEC51,	false,	false	},
	{ 48,	(void *)offsetof (GAMEDATA_SPWAW, sec48),	SPWAW_SIZESEC48,	true,	false	},
};

static SECMAP	MAP = { SPWAW_SECTION_COUNT, MAPLIST };

static bool
validate_spwaw_gamedata_structures (void)
{
	GAMEDATA_SPWAW	*p = NULL; //fixme: compiler warning C4189, variable not referenced
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
	assert (sizeof(p->sec44.u.raw) == sizeof(p->sec44.u.d));
	assert (sizeof(p->sec45.u.raw) == sizeof(p->sec45.u.d));
	assert (sizeof(p->sec46.u.raw) == sizeof(p->sec46.u.d));
	assert (sizeof(p->sec47.u.raw) == sizeof(p->sec47.u.d));
	assert (sizeof(p->sec51.u.raw) == sizeof(p->sec51.u.d));
	assert (sizeof(p->sec48.u.raw) == sizeof(p->sec48.u.d));
	return (true);
}
static bool spwaw_gamedata_structures_ok = validate_spwaw_gamedata_structures();

SECMAP *
gamedata_spwaw_SECMAP (void)
{
	return (&MAP);
}

void *
gamedata_spwaw_new_data (void)
{
	return ((void *)safe_malloc (GAMEDATA_SPWAW));
}

void
gamedata_spwaw_free_data (void **data)
{
	void	*p;

	CWRNULL (data);
	p = *data; *data = NULL;

	if (p) safe_free (p);
}
