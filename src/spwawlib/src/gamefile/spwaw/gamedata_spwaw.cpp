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
	{  0,	(void *)offsetof (GAMEDATA_SPWAW, sec00),	SIZESEC00,	false,	false	},
	{  1,	(void *)offsetof (GAMEDATA_SPWAW, sec01),	SIZESEC01,	true,	false	},
	{  2,	(void *)offsetof (GAMEDATA_SPWAW, sec02),	SIZESEC02,	false,	false	},
	{  3,	(void *)offsetof (GAMEDATA_SPWAW, sec03),	SIZESEC03,	true,	false	},
	{  4,	(void *)offsetof (GAMEDATA_SPWAW, sec04),	SIZESEC04,	true,	false	},
	{  5,	(void *)offsetof (GAMEDATA_SPWAW, sec05),	SIZESEC05,	true,	false	},
	{  6,	(void *)offsetof (GAMEDATA_SPWAW, sec06),	SIZESEC06,	true,	false	},
	{  7,	(void *)offsetof (GAMEDATA_SPWAW, sec07),	SIZESEC07,	true,	false	},
	{  8,	(void *)offsetof (GAMEDATA_SPWAW, sec08),	SIZESEC08,	true,	false	},
	{  9,	(void *)offsetof (GAMEDATA_SPWAW, sec09),	SIZESEC09,	true,	false	},
	{ 10,	(void *)offsetof (GAMEDATA_SPWAW, sec10),	SIZESEC10,	false,	false	},
	{ 11,	(void *)offsetof (GAMEDATA_SPWAW, sec11),	SIZESEC11,	true,	false	},
	{ 12,	(void *)offsetof (GAMEDATA_SPWAW, sec12),	SIZESEC12,	false,	false	},
	{ 13,	(void *)offsetof (GAMEDATA_SPWAW, sec13),	SIZESEC13,	false,	false	},
	{ 14,	(void *)offsetof (GAMEDATA_SPWAW, sec14),	SIZESEC14,	false,	false	},
	{ 15,	(void *)offsetof (GAMEDATA_SPWAW, sec15),	SIZESEC15,	false,	false	},
	{ 16,	(void *)offsetof (GAMEDATA_SPWAW, sec16),	SIZESEC16,	false,	false	},
	{ 17,	(void *)offsetof (GAMEDATA_SPWAW, sec17),	SIZESEC17,	true,	false	},
	{ 18,	(void *)offsetof (GAMEDATA_SPWAW, sec18),	SIZESEC18,	true,	false	},
	{ 19,	(void *)offsetof (GAMEDATA_SPWAW, sec19),	SIZESEC19,	true,	false	},
	{ 20,	(void *)offsetof (GAMEDATA_SPWAW, sec20),	SIZESEC20,	false,	false	},
	{ 21,	(void *)offsetof (GAMEDATA_SPWAW, sec21),	SIZESEC21,	false,	false	},
	{ 22,	(void *)offsetof (GAMEDATA_SPWAW, sec22),	SIZESEC22,	false,	false	},
	{ 23,	(void *)offsetof (GAMEDATA_SPWAW, sec23),	SIZESEC23,	true,	false	},
	{ 24,	(void *)offsetof (GAMEDATA_SPWAW, sec24),	SIZESEC24,	true,	false	},
	{ 25,	(void *)offsetof (GAMEDATA_SPWAW, sec25),	SIZESEC25,	true,	false	},
	{ 26,	(void *)offsetof (GAMEDATA_SPWAW, sec26),	SIZESEC26,	true,	false	},
	{ 27,	(void *)offsetof (GAMEDATA_SPWAW, sec27),	SIZESEC27,	false,	false	},
	{ 28,	(void *)offsetof (GAMEDATA_SPWAW, sec28),	SIZESEC28,	true,	false	},
	{ 29,	(void *)offsetof (GAMEDATA_SPWAW, sec29),	SIZESEC29,	true,	false	},
	{ 30,	(void *)offsetof (GAMEDATA_SPWAW, sec30),	SIZESEC30,	true,	false	},
	{ 31,	(void *)offsetof (GAMEDATA_SPWAW, sec31),	SIZESEC31,	true,	false	},
	{ 32,	(void *)offsetof (GAMEDATA_SPWAW, sec32),	SIZESEC32,	true,	false	},
	{ 33,	(void *)offsetof (GAMEDATA_SPWAW, sec33),	SIZESEC33,	true,	false	},
	{ 34,	(void *)offsetof (GAMEDATA_SPWAW, sec34),	SIZESEC34,	true,	false	},
	{ 35,	(void *)offsetof (GAMEDATA_SPWAW, sec35),	SIZESEC35,	true,	false	},
	{ 36,	(void *)offsetof (GAMEDATA_SPWAW, sec36),	SIZESEC36,	true,	false	},
	{ 37,	(void *)offsetof (GAMEDATA_SPWAW, sec37),	SIZESEC37,	true,	false	},
	{ 38,	(void *)offsetof (GAMEDATA_SPWAW, sec38),	SIZESEC38,	false,	false	},
	{ 39,	(void *)offsetof (GAMEDATA_SPWAW, sec39),	SIZESEC39,	false,	false	},
	{ 40,	(void *)offsetof (GAMEDATA_SPWAW, sec40),	SIZESEC40,	true,	false	},
	{ 41,	(void *)offsetof (GAMEDATA_SPWAW, sec41),	SIZESEC41,	true,	false	},
	{ 42,	(void *)offsetof (GAMEDATA_SPWAW, sec42),	SIZESEC42,	false,	false	},
	{ 43,	(void *)offsetof (GAMEDATA_SPWAW, sec43),	SIZESEC43,	false,	false	},
	{ 44,	(void *)offsetof (GAMEDATA_SPWAW, sec44),	SIZESEC44,	true,	false	},
	{ 45,	(void *)offsetof (GAMEDATA_SPWAW, sec45),	SIZESEC45,	false,	false	},
	{ 46,	(void *)offsetof (GAMEDATA_SPWAW, sec46),	SIZESEC46,	true,	false	},
	{ 47,	(void *)offsetof (GAMEDATA_SPWAW, sec47),	SIZESEC47,	true,	false	},
	{ 51,	(void *)offsetof (GAMEDATA_SPWAW, sec51),	SIZESEC51,	false,	false	},
	{ 48,	(void *)offsetof (GAMEDATA_SPWAW, sec48),	SIZESEC48,	true,	false	},
};

static SECMAP	MAP = { SPWAW_SECTION_COUNT, MAPLIST };

static bool
validate_spwaw_gamedata_structures (void)
{
	GAMEDATA_SPWAW	*p = NULL;
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
