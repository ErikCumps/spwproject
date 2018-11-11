/** \file
 * The SPWaW Library - SPWaW OOB handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_spwoob.h>
#include "common/internal.h"

typedef struct s_IDSTRMAP {
	BYTE		id;
	const char	*people;
	const char	*nation;
	const char	*prefix;
} IDSTRMAP;

//static const IDSTRMAP spwoob_idstrings_map[] = {
//	{  9,	"Communist Chinese",	"Communist China",	"CC"	},
//	{ 15,	"Czechoslovakian",	"Czechoslovakia",	"CS"	},
//	{ 16,	"Philippine",		"Philippines",		"PI"	},
//	{ 19,	"Nationalist Spanish",	"Nationalist Spain",	"NS"	},
//	{ 20,	"Republican Spanish",	"Republican Spain",	"RS"	},
//	{ 29,	"Bulgarian",		"Bulgaria",		"BU"	},
//	{ 70,	"German",		"Germany",		"GE"	},
//	{ 71,	"Finnish",		"Finland",		"FI"	},
//	{ 72,	"Italian",		"Italy",		"IT"	},
//	{ 73,	"Romanian",		"Romania",		"RO"	},
//	{ 74,	"Hungarian",		"Hungary",		"HU"	},
//	{ 75,	"Japanese",		"Japan",		"JA"	},
//	{ 76,	"French",		"France",		"FR"	},
//	{ 77,	"British",		"United Kingdom",	"BR"	},
//	{ 78,	"Belgian",		"Belgium",		"BE"	},
//	{ 79,	"Dutch",		"Netherlands",		"NL"	},
//	{ 80,	"Polish",		"Poland",		"PO"	},
//	{ 81,	"Soviet",		"Soviet Union",		"SO"	},
//	{ 82,	"US army",		"US Army",		"US"	},
//	{ 83,	"US marine",		"US Marine Corps",	"USMC"	},
//	{ 84,	"Nationalist Chinese",	"Nationalist China",	"NC"	},
//	{ 85,	"Greek",		"Greece",		"GK"	},
//	{ 86,	"Norwegian",		"Norway",		"NOR"	},
//	{ 87,	"Yugoslavian",		"Yugoslavia",		"YU"	},
//	{ 88,	"Canadian",		"Canada",		"CA"	},
//	{ 89,	"Indian",		"India",		"IN"	},
//	{ 90,	"ANZAC",		"ANZAC",		"ANZ"	},
//	{ 91,	"Free French",		"Free France",		"FF"	},
//	{  0,	"???",			"???",			"??"	}
//};

static const IDSTRMAP spwoob_idstrings_map[] = {
	{  1,	"Slovakian",		"Slovak Republic",		"SK"	},
	{  2,	"Polish",		"Poland",			"PO"	},
	{  3,	"Manchurian",		"Manchukuo",			"MN"	},
	{  4,	"Fascist Italian",	"Italian Social Republic",	"ISR"	},
	{  5,	"Japanese",		"Japan",			"JA"	},
	{  6,	"French",		"France",			"FR"	},
	{  7,	"British",		"Great Britain",		"BR"	},
	{  8,	"Vichy French",		"Vichy France",			"VFR"	},
	{  9,	"Polish",		"People's Army of Poland",	"LWP"	},
	{ 11,	"Soviet",		"Soviet Union",			"SO"	},
	{ 12,	"US army",		"US Army",			"USA"	},
	{ 13,	"US marine",		"US Marine Corps",		"USMC"	},
	{ 14,	"Communist Chinese",	"Communist China",		"CC"	},
	{ 15,	"ANZAC",		"ANZAC",			"ANZ"	},
	{ 16,	"German",		"Germany",			"GE"	},
	{ 18,	"Indian",		"India",			"IN"	},
	{ 19,	"Bulgarian",		"Bulgaria",			"BU"	},
	{ 20,	"Yugoslavian",		"Yugoslavia",			"YU"	},
	{ 21,	"Nationalist Chinese",	"Nationalist China",		"NC"	},
	{ 27,	"Belgian",		"Belgium",			"BE"	},
	{ 28,	"Dutch",		"Netherlands",			"NL"	},
	{ 29,	"Norwegian",		"Norway",			"NOR"	},
	{ 30,	"Canadian",		"Canada",			"CA"	},
	{ 31,	"Greek",		"Greece",			"GK"	},
	{ 32,	"Republican Spanish",	"Republican Spain",		"RS"	},
	{ 33,	"Nationalist Spanish",	"Nationalist Spain",		"NS"	},
	{ 34,	"Italian",		"Italy",			"IT"	},
	{ 35,	"Finnish",		"Finland",			"FI"	},
	{ 36,	"Swedish",		"Sweden",			"SE"	},
	{ 37,	"Czechoslovakian",	"Czechoslovakia",		"CS"	},
	{ 38,	"Hungarian",		"Hungary",			"HU"	},
	{ 39,	"Romanian",		"Romania",			"RO"	},
	{ 40,	"Green",		"Green",			"GRN"	},
	{ 41,	"Thai",			"Thailand",			"THA"	},
	{ 40,	"Blue",			"Blue",				"BLU"	},
	{ 40,	"Red",			"Red",				"RED"	},
	{  0,	"???",			"???",				"??"	}
};

const char *
spwoob_id2people (BYTE id)
{
	int	i = 0;

	while (spwoob_idstrings_map[i].id != 0) {
		if (spwoob_idstrings_map[i].id == id) break;
		i++;
	}

	return ((char*)(spwoob_idstrings_map[i].people));
}

const char *
spwoob_id2nation (BYTE id)
{
	int	i = 0;

	while (spwoob_idstrings_map[i].id != 0) {
		if (spwoob_idstrings_map[i].id == id) break;
		i++;
	}

	return ((char*)(spwoob_idstrings_map[i].nation));
}

const char *
spwoob_id2prefix (BYTE id)
{
	int	i = 0;

	while (spwoob_idstrings_map[i].id != 0) {
		if (spwoob_idstrings_map[i].id == id) break;
		i++;
	}

	return ((char*)(spwoob_idstrings_map[i].prefix));
}
