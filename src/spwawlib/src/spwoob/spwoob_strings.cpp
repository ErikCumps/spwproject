/** \file
 * The SPWaW Library - SPWaW OOB handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_spwoob.h>
#include "common/internal.h"

/* Convenience macro to build a simple comparable timestamp from a year/month date */
#define	SIMPLE_STAMP(yr_,mo_) ((yr_)*12+((mo_)-1))

typedef struct s_OOBSTRINGS {
	const char	*people;
	const char	*nation;
	const char	*prefix;
	const char	*flagid;
} OOBSTRINGS;

typedef struct s_HISTMAP {
	int		from_year;
	int		from_month;
	OOBSTRINGS	strings;
} HISTMAP;

typedef struct s_IDMAP {
	BYTE		id;
	OOBSTRINGS	strings;
	const HISTMAP	*history;
} IDMAP;

static const IDMAP spwaw_idmap[] = {
	{  9,	{ "Communist Chinese",	"Communist China",	"CC",	"SPWAW_CC"	},	NULL },
	{ 15,	{ "Czechoslovakian",	"Czechoslovakia",	"CS",	"SPWAW_CS"	},	NULL	},
	{ 16,	{ "Philippine",		"Philippines",		"PI",	"SPWAW_PI"	},	NULL	},
	{ 19,	{ "Nationalist Spanish","Nationalist Spain",	"NS",	"SPWAW_NS"	},	NULL	},
	{ 20,	{ "Republican Spanish",	"Republican Spain",	"RS",	"SPWAW_RS"	},	NULL	},
	{ 29,	{ "Bulgarian",		"Bulgaria",		"BU",	"SPWAW_BU"	},	NULL	},
	{ 70,	{ "German",		"Germany",		"GE",	"SPWAW_GE"	},	NULL	},
	{ 71,	{ "Finnish",		"Finland",		"FI",	"SPWAW_FI"	},	NULL	},
	{ 72,	{ "Italian",		"Italy",		"IT",	"SPWAW_IT"	},	NULL	},
	{ 73,	{ "Romanian",		"Romania",		"RO",	"SPWAW_RO"	},	NULL	},
	{ 74,	{ "Hungarian",		"Hungary",		"HU",	"SPWAW_HU"	},	NULL	},
	{ 75,	{ "Japanese",		"Japan",		"JA",	"SPWAW_JA"	},	NULL	},
	{ 76,	{ "French",		"France",		"FR",	"SPWAW_FR"	},	NULL	},
	{ 77,	{ "British",		"United Kingdom",	"BR",	"SPWAW_BR"	},	NULL	},
	{ 78,	{ "Belgian",		"Belgium",		"BE",	"SPWAW_BE"	},	NULL	},
	{ 79,	{ "Dutch",		"Netherlands",		"NL",	"SPWAW_NL"	},	NULL	},
	{ 80,	{ "Polish",		"Poland",		"PO",	"SPWAW_PO"	},	NULL	},
	{ 81,	{ "Soviet",		"Soviet Union",		"SO",	"SPWAW_SO"	},	NULL	},
	{ 82,	{ "US army",		"US Army",		"US",	"SPWAW_US"	},	NULL	},
	{ 83,	{ "US marine",		"US Marine Corps",	"USMC",	"SPWAW_USMC"	},	NULL	},
	{ 84,	{ "Nationalist Chinese","Nationalist China",	"NC",	"SPWAW_NC"	},	NULL	},
	{ 85,	{ "Greek",		"Greece",		"GK",	"SPWAW_GK"	},	NULL	},
	{ 86,	{ "Norwegian",		"Norway",		"NOR",	"SPWAW_NOR"	},	NULL	},
	{ 87,	{ "Yugoslavian",	"Yugoslavia",		"YU",	"SPWAW_YU"	},	NULL	},
	{ 88,	{ "Canadian",		"Canada",		"CA",	"SPWAW_CA"	},	NULL	},
	{ 89,	{ "Indian",		"India",		"IN",	"SPWAW_IN"	},	NULL	},
	{ 90,	{ "ANZAC",		"ANZAC",		"ANZ",	"SPWAW_ANZ"	},	NULL	},
	{ 91,	{ "Free French",	"Free France",		"FF",	"SPWAW_FF"	},	NULL	},
	{ SPWOOB_UNKNOWN, { "???", "???",			"??",	"SPWAW_??"	},	NULL	},
};

static const HISTMAP winspww2_history_map_FR[] = {
	{ 1944,	 7,	{ "French",		"France",				"FR",	"SPWW2_FR"	} },
	{ 1940,	 7,	{ "Free French",	"Free France",				"FF",	"SPWW2_FF"	} },
	{ 1930,	 1,	{ "French",		"France",				"FR",	"SPWW2_FR"	} },
	{    0,	 0,	{ "French",		"France",				"FR",	"SPWW2_FR"	} },
};

static const HISTMAP winspww2_history_map_GE[] = {
	{ 1935,	 9,	{ "German",		"Nazi Germany",				"GE",	"SPWW2_GE"	} },
	{ 1933,	 4,	{ "German",		"German Empire",			"GE",	"SPWW2_GE_emp"	} },
	{ 1930,	 1,	{ "German",		"Weimar Republic",			"GE",	"SPWW2_GE_wmr"	} },
	{    0,	 0,	{ "German",		"Germany",				"GE",	"SPWW2_GE_old"	} },
};

static const HISTMAP winspww2_history_map_YU[] = {
	{ 1941,	 6,	{ "Yugoslavian",	"Democratic Federal Yugoslavia",	"YU",	"SPWW2_YU"	} },
	{    0,	 0,	{ "Yugoslavian",	"Yugoslavia",				"YU",	"SPWW2_YU_old"	} },
};

static const HISTMAP winspww2_history_map_RS[] = {
	{ 1931,	 4,	{ "Republican Spanish",	"Republican Spain",			"RS",	"SPWW2_RS"	} },
	{    0,	 0,	{ "Republican Spanish",	"Republican Spain",			"RS",	"SPWW2_RS_old"	} },
};

static const HISTMAP winspww2_history_map_NS[] = {
	{ 1937,	 1,	{ "Nationalist Spanish","Nationalist Spain",			"NS",	"SPWW2_NS"	} },
	{    0,	 0,	{ "Nationalist Spanish","Nationalist Spain",			"NS",	"SPWW2_NS_old"	} },
};

static const IDMAP winspww2_idmap[] = {
	{  1,	{ "Slovakian",		"Slovak Republic",		"SK",	"SPWW2_SK"	},	NULL			},
	{  2,	{ "Polish",		"Poland",			"PO",	"SPWW2_PO"	},	NULL			},
	{  3,	{ "Manchurian",		"Manchukuo",			"MN",	"SPWW2_MN"	},	NULL			},
	{  4,	{ "Fascist Italian",	"Italian Social Republic",	"ISR",	"SPWW2_ISR"	},	NULL			},
	{  5,	{ "Japanese",		"Japan",			"JA",	"SPWW2_JA"	},	NULL			},
	{  6,	{ "French",		"France",			"FR",	"SPWW2_FR"	},	winspww2_history_map_FR	},
	{  7,	{ "British",		"Great Britain",		"BR",	"SPWW2_BR"	},	NULL			},
	{  8,	{ "Vichy French",	"Vichy France",			"VFR",	"SPWW2_VFR"	},	NULL			},
	{  9,	{ "Polish",		"People's Army of Poland",	"LWP",	"SPWW2_LWP"	},	NULL			},
	{ 11,	{ "Soviet",		"Soviet Union",			"SO",	"SPWW2_SO"	},	NULL			},
	{ 12,	{ "US army",		"US Army",			"USA",	"SPWW2_USA"	},	NULL			},
	{ 13,	{ "US marine",		"US Marine Corps",		"USMC",	"SPWW2_USMC"	},	NULL			},
	{ 14,	{ "Communist Chinese",	"Communist China",		"CC",	"SPWW2_CC"	},	NULL			},
	{ 15,	{ "ANZAC",		"ANZAC",			"ANZ",	"SPWW2_ANZ"	},	NULL			},
	{ 16,	{ "German",		"Germany",			"GE",	"SPWW2_GE"	},	winspww2_history_map_GE	},
	{ 18,	{ "Indian",		"India",			"IN",	"SPWW2_IN"	},	NULL			},
	{ 19,	{ "Bulgarian",		"Bulgaria",			"BU",	"SPWW2_BU"	},	NULL			},
	{ 20,	{ "Yugoslavian",	"Yugoslavia",			"YU",	"SPWW2_YU"	},	winspww2_history_map_YU	},
	{ 21,	{ "Nationalist Chinese","Nationalist China",		"NC",	"SPWW2_NC"	},	NULL			},
	{ 27,	{ "Belgian",		"Belgium",			"BE",	"SPWW2_BE"	},	NULL			},
	{ 28,	{ "Dutch",		"Netherlands",			"NL",	"SPWW2_NL"	},	NULL			},
	{ 29,	{ "Norwegian",		"Norway",			"NOR",	"SPWW2_NOR"	},	NULL			},
	{ 30,	{ "Canadian",		"Canada",			"CA",	"SPWW2_CA"	},	NULL			},
	{ 31,	{ "Greek",		"Greece",			"GK",	"SPWW2_GK"	},	NULL			},
	{ 32,	{ "Republican Spanish",	"Republican Spain",		"RS",	"SPWW2_RS"	},	winspww2_history_map_RS	},
	{ 33,	{ "Nationalist Spanish","Nationalist Spain",		"NS",	"SPWW2_NS"	},	winspww2_history_map_NS	},
	{ 34,	{ "Italian",		"Italy",			"IT",	"SPWW2_IT"	},	NULL			},
	{ 35,	{ "Finnish",		"Finland",			"FI",	"SPWW2_FI"	},	NULL			},
	{ 36,	{ "Swedish",		"Sweden",			"SE",	"SPWW2_SE"	},	NULL			},
	{ 37,	{ "Czechoslovakian",	"Czechoslovakia",		"CS",	"SPWW2_CS"	},	NULL			},
	{ 38,	{ "Hungarian",		"Hungary",			"HU",	"SPWW2_HU"	},	NULL			},
	{ 39,	{ "Romanian",		"Romania",			"RO",	"SPWW2_RO"	},	NULL			},
	{ 40,	{ "Green",		"Green",			"GRN",	"SPWW2_GRN"	},	NULL			},
	{ 41,	{ "Thai",		"Thailand",			"THA",	"SPWW2_THA"	},	NULL			},
	{ 40,	{ "Blue",		"Blue",				"BLU",	"SPWW2_BLU"	},	NULL			},
	{ 40,	{ "Red",		"Red",				"RED",	"SPWW2_RED"	},	NULL			},
	{ SPWOOB_UNKNOWN, { "???", "???",				"??",	"SPWW2_??"	},	NULL			},
};

const OOBSTRINGS *
lookup_oobstrings (SPWAW_GAME_TYPE gametype, BYTE id, int year, int month)
{
	int		td = SIMPLE_STAMP(year, month);
	const IDMAP	*idmap;
	int		i;

	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			idmap = spwaw_idmap;
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			idmap = winspww2_idmap;
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			ERROR0 ("unsupported game type");
			return (NULL);
			break;
	}

	i = 0;
	while (idmap[i].id != 0) {
		if (idmap[i].id == id) break;
		i++;
	}

	if ((year == 0 && month == 0) || idmap[i].history == NULL)
		return (&(idmap[i].strings));

	const HISTMAP *hist = idmap[i].history; i = 0;
	while (td < SIMPLE_STAMP(hist[i].from_year, hist[i].from_month)) i++;
	return (&(hist[i].strings));
}

const char *
spwoob_id2people (SPWAW_GAME_TYPE gametype, BYTE id)
{
	return (spwoob_id2people(gametype, id, 0, 0));
}

const char *
spwoob_id2people (SPWAW_GAME_TYPE gametype, BYTE id, int year, int month)
{
	const OOBSTRINGS *s = lookup_oobstrings (gametype, id, year, month);
	if (!s) return (NULL);
	return (s->people);
}

const char *
spwoob_id2nation (SPWAW_GAME_TYPE gametype, BYTE id)
{
	return (spwoob_id2nation(gametype, id, 0, 0));
}

const char *
spwoob_id2nation (SPWAW_GAME_TYPE gametype, BYTE id, int year, int month)
{
	const OOBSTRINGS *s = lookup_oobstrings (gametype, id, year, month);
	if (!s) return (NULL);
	return (s->nation);
}

const char *
spwoob_id2prefix (SPWAW_GAME_TYPE gametype, BYTE id)
{
	return (spwoob_id2prefix(gametype, id, 0, 0));
}

const char *
spwoob_id2prefix (SPWAW_GAME_TYPE gametype, BYTE id, int year, int month)
{
	const OOBSTRINGS *s = lookup_oobstrings (gametype, id, year, month);
	if (!s) return (NULL);
	return (s->prefix);
}

const char *
spwoob_id2flagid (SPWAW_GAME_TYPE gametype, BYTE id)
{
	return (spwoob_id2flagid(gametype, id, 0, 0));
}

const char *
spwoob_id2flagid (SPWAW_GAME_TYPE gametype, BYTE id, int year, int month)
{
	const OOBSTRINGS *s = lookup_oobstrings (gametype, id, year, month);
	if (!s) return (NULL);
	return (s->flagid);
}
