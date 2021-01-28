/** \file
 * The SPWaW Library - game data handling - winSPMBT game data.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/winspmbt/gamedata_winspmbt.h"
#include "gamefile/winspmbt/defines_winspmbt.h"
#include "common/internal.h"

static SECMAPEL	MAPLIST[WINSPMBT_SECTION_COUNT] = {
	{  0,	(void *)offsetof (GAMEDATA_WINSPMBT, sec00),	WINSPMBT_SIZESEC00,	false,	false	},
	{  1,	(void *)offsetof (GAMEDATA_WINSPMBT, sec01),	WINSPMBT_SIZESEC01,	true,	false	},
	{  2,	(void *)offsetof (GAMEDATA_WINSPMBT, sec02),	WINSPMBT_SIZESEC02,	false,	false	},
	{  3,	(void *)offsetof (GAMEDATA_WINSPMBT, sec03),	WINSPMBT_SIZESEC03,	true,	false	},
	{  4,	(void *)offsetof (GAMEDATA_WINSPMBT, sec04),	WINSPMBT_SIZESEC04,	true,	false	},
	{  5,	(void *)offsetof (GAMEDATA_WINSPMBT, sec05),	WINSPMBT_SIZESEC05,	true,	false	},
	{  6,	(void *)offsetof (GAMEDATA_WINSPMBT, sec06),	WINSPMBT_SIZESEC06,	true,	false	},
	{  7,	(void *)offsetof (GAMEDATA_WINSPMBT, sec07),	WINSPMBT_SIZESEC07,	true,	false	},
	{  8,	(void *)offsetof (GAMEDATA_WINSPMBT, sec08),	WINSPMBT_SIZESEC08,	true,	false	},
	{  9,	(void *)offsetof (GAMEDATA_WINSPMBT, sec09),	WINSPMBT_SIZESEC09,	true,	false	},
	{ 10,	(void *)offsetof (GAMEDATA_WINSPMBT, sec10),	WINSPMBT_SIZESEC10,	false,	false	},
	{ 11,	(void *)offsetof (GAMEDATA_WINSPMBT, sec11),	WINSPMBT_SIZESEC11,	true,	false	},
	{ 12,	(void *)offsetof (GAMEDATA_WINSPMBT, sec12),	WINSPMBT_SIZESEC12,	false,	false	},
	{ 13,	(void *)offsetof (GAMEDATA_WINSPMBT, sec13),	WINSPMBT_SIZESEC13,	false,	false	},
	{ 14,	(void *)offsetof (GAMEDATA_WINSPMBT, sec14),	WINSPMBT_SIZESEC14,	false,	false	},
	{ 15,	(void *)offsetof (GAMEDATA_WINSPMBT, sec15),	WINSPMBT_SIZESEC15,	false,	false	},
	{ 16,	(void *)offsetof (GAMEDATA_WINSPMBT, sec16),	WINSPMBT_SIZESEC16,	false,	false	},
	{ 17,	(void *)offsetof (GAMEDATA_WINSPMBT, sec17),	WINSPMBT_SIZESEC17,	true,	false	},
	{ 18,	(void *)offsetof (GAMEDATA_WINSPMBT, sec18),	WINSPMBT_SIZESEC18,	true,	false	},
	{ 19,	(void *)offsetof (GAMEDATA_WINSPMBT, sec19),	WINSPMBT_SIZESEC19,	true,	false	},
	{ 20,	(void *)offsetof (GAMEDATA_WINSPMBT, sec20),	WINSPMBT_SIZESEC20,	false,	false	},
	{ 21,	(void *)offsetof (GAMEDATA_WINSPMBT, sec21),	WINSPMBT_SIZESEC21,	false,	false	},
	{ 22,	(void *)offsetof (GAMEDATA_WINSPMBT, sec22),	WINSPMBT_SIZESEC22,	false,	false	},
	{ 23,	(void *)offsetof (GAMEDATA_WINSPMBT, sec23),	WINSPMBT_SIZESEC23,	true,	false	},
	{ 24,	(void *)offsetof (GAMEDATA_WINSPMBT, sec24),	WINSPMBT_SIZESEC24,	true,	false	},
	{ 25,	(void *)offsetof (GAMEDATA_WINSPMBT, sec25),	WINSPMBT_SIZESEC25,	true,	false	},
	{ 26,	(void *)offsetof (GAMEDATA_WINSPMBT, sec26),	WINSPMBT_SIZESEC26,	true,	false	},
	{ 27,	(void *)offsetof (GAMEDATA_WINSPMBT, sec27),	WINSPMBT_SIZESEC27,	false,	false	},
	{ 28,	(void *)offsetof (GAMEDATA_WINSPMBT, sec28),	WINSPMBT_SIZESEC28,	true,	false	},
	{ 29,	(void *)offsetof (GAMEDATA_WINSPMBT, sec29),	WINSPMBT_SIZESEC29,	true,	false	},
	{ 30,	(void *)offsetof (GAMEDATA_WINSPMBT, sec30),	WINSPMBT_SIZESEC30,	true,	false	},
	{ 31,	(void *)offsetof (GAMEDATA_WINSPMBT, sec31),	WINSPMBT_SIZESEC31,	true,	false	},
	{ 32,	(void *)offsetof (GAMEDATA_WINSPMBT, sec32),	WINSPMBT_SIZESEC32,	true,	false	},
	{ 33,	(void *)offsetof (GAMEDATA_WINSPMBT, sec33),	WINSPMBT_SIZESEC33,	true,	false	},
	{ 34,	(void *)offsetof (GAMEDATA_WINSPMBT, sec34),	WINSPMBT_SIZESEC34,	true,	false	},
	{ 35,	(void *)offsetof (GAMEDATA_WINSPMBT, sec35),	WINSPMBT_SIZESEC35,	true,	false	},
	{ 36,	(void *)offsetof (GAMEDATA_WINSPMBT, sec36),	WINSPMBT_SIZESEC36,	true,	false	},
	{ 37,	(void *)offsetof (GAMEDATA_WINSPMBT, sec37),	WINSPMBT_SIZESEC37,	true,	false	},
	{ 38,	(void *)offsetof (GAMEDATA_WINSPMBT, sec38),	WINSPMBT_SIZESEC38,	false,	false	},
	{ 39,	(void *)offsetof (GAMEDATA_WINSPMBT, sec39),	WINSPMBT_SIZESEC39,	false,	false	},
	{ 40,	(void *)offsetof (GAMEDATA_WINSPMBT, sec40),	WINSPMBT_SIZESEC40,	true,	false	},
	{ 41,	(void *)offsetof (GAMEDATA_WINSPMBT, sec41),	WINSPMBT_SIZESEC41,	true,	false	},
	{ 42,	(void *)offsetof (GAMEDATA_WINSPMBT, sec42),	WINSPMBT_SIZESEC42,	false,	false	},
	{ 43,	(void *)offsetof (GAMEDATA_WINSPMBT, sec43),	WINSPMBT_SIZESEC43,	false,	false	},
	{ 51,	(void *)offsetof (GAMEDATA_WINSPMBT, sec51),	WINSPMBT_SIZESEC51,	false,	false	},
	{ 52,	(void *)offsetof (GAMEDATA_WINSPMBT, sec52),	WINSPMBT_SIZESEC52,	true,	false	},
	{ 53,	(void *)offsetof (GAMEDATA_WINSPMBT, sec53),	WINSPMBT_SIZESEC53,	false,	false	},
	{ 54,	(void *)offsetof (GAMEDATA_WINSPMBT, sec54),	WINSPMBT_SIZESEC54,	true,	false	},
	{ 56,	(void *)offsetof (GAMEDATA_WINSPMBT, sec56),	WINSPMBT_SIZESEC56,	true,	false	},
	{ 57,	(void *)offsetof (GAMEDATA_WINSPMBT, sec57),	WINSPMBT_SIZESEC57,	true,	false	},
	{ 59,	(void *)offsetof (GAMEDATA_WINSPMBT, sec59),	WINSPMBT_SIZESEC59,	true,	false	},
	{ 48,	(void *)offsetof (GAMEDATA_WINSPMBT, sec48),	WINSPMBT_SIZESEC48,	true,	false	},
	{ 49,	0,						0,			true,	true	},
	{ 50,	0,						0,			true,	true	},
	{ 61,	0,						0,			true,	true	},
};

static SECMAP	MAP = { WINSPMBT_SECTION_COUNT, MAPLIST };

#define UNREFERENCED_LOCAL_VARIABLE(V)	(V)

static bool
validate_winspmbt_gamedata_structures (void)
{
	GAMEDATA_WINSPMBT	*p = NULL;

	/* prevent compiler warnings */
	UNREFERENCED_LOCAL_VARIABLE (p);

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
static bool winspmbt_gamedata_structures_ok = validate_winspmbt_gamedata_structures();

SECMAP *
gamedata_winspmbt_SECMAP (void)
{
	return (&MAP);
}

void *
gamedata_winspmbt_new_data (void)
{
	return ((void *)safe_malloc (GAMEDATA_WINSPMBT));
}

void
gamedata_winspmbt_free_data (void **data)
{
	void	*p;

	CWRNULL (data);
	p = *data; *data = NULL;

	if (p) safe_free (p);
}
