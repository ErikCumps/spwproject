/** \file
* The SPWaW Library - OOB handling - SP:WaW nations info.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SPWOOB_NATIONS_SPWAW_H
#define	SPWOOB_NATIONS_SPWAW_H	1

#include "spwoob_nations.h"

#define	DEFAULT_SPWAW_DATE_RANGE	1930, 01, 1950, 01

static const IDMAP spwaw_idmap[] = {
	{  9,	{ "Communist Chinese",	"Communist China",	"CC",	"SPWAW_CC"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 15,	{ "Czechoslovakian",	"Czechoslovakia",	"CS",	"SPWAW_CS"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 16,	{ "Philippine",		"Philippines",		"PI",	"SPWAW_PI"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 19,	{ "Nationalist Spanish","Nationalist Spain",	"NS",	"SPWAW_NS"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 20,	{ "Republican Spanish",	"Republican Spain",	"RS",	"SPWAW_RS"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 29,	{ "Bulgarian",		"Bulgaria",		"BU",	"SPWAW_BU"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 70,	{ "German",		"Germany",		"GE",	"SPWAW_GE"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 71,	{ "Finnish",		"Finland",		"FI",	"SPWAW_FI"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 72,	{ "Italian",		"Italy",		"IT",	"SPWAW_IT"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 73,	{ "Romanian",		"Romania",		"RO",	"SPWAW_RO"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 74,	{ "Hungarian",		"Hungary",		"HU",	"SPWAW_HU"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 75,	{ "Japanese",		"Japan",		"JA",	"SPWAW_JA"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 76,	{ "French",		"France",		"FR",	"SPWAW_FR"	},
		1930,	 1,	1943,	 1,	NULL				},
	{ 77,	{ "British",		"United Kingdom",	"BR",	"SPWAW_BR"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 78,	{ "Belgian",		"Belgium",		"BE",	"SPWAW_BE"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 79,	{ "Dutch",		"Netherlands",		"NL",	"SPWAW_NL"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 80,	{ "Polish",		"Poland",		"PO",	"SPWAW_PO"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 81,	{ "Soviet",		"Soviet Union",		"SO",	"SPWAW_SO"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 82,	{ "US Army",		"US Army",		"US",	"SPWAW_US"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 83,	{ "US Marine",		"US Marine Corps",	"USMC",	"SPWAW_USMC"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 84,	{ "Nationalist Chinese","Nationalist China",	"NC",	"SPWAW_NC"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 85,	{ "Greek",		"Greece",		"GK",	"SPWAW_GK"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 86,	{ "Norwegian",		"Norway",		"NOR",	"SPWAW_NOR"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 87,	{ "Yugoslavian",	"Yugoslavia",		"YU",	"SPWAW_YU"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 88,	{ "Canadian",		"Canada",		"CA",	"SPWAW_CA"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 89,	{ "Indian",		"India",		"IN",	"SPWAW_IN"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 90,	{ "ANZAC",		"ANZAC",		"ANZ",	"SPWAW_ANZ"	},
		DEFAULT_SPWAW_DATE_RANGE,	NULL				},
	{ 91,	{ "Free French",	"Free France",		"FF",	"SPWAW_FF"	},
		1943,	 1,	1950,	 1,	NULL				},
};

#endif	/* SPWOOB_NATIONS_SPWAW_H */
