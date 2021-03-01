/** \file
 * The SPWaW Library - OOB handling - winSPWW2 nations info.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SPWOOB_NATIONS_WINSPWW2_H
#define	SPWOOB_NATIONS_WINSPWW2_H	1

#include "spwoob_nations.h"

#define	DEFAULT_WINSPWW2_DATE_RANGE	1930, 01, 1947, 01

static const HISTEL winspww2_history_FR_list[] = {
	{ 1930,	 1,	{ "French",			"France",				"FR",	"SPWW2_FR"	} },
	{ 1940,	 7,	{ "Free French",		"Free France",				"FF",	"SPWW2_FF"	} },
	{ 1944,	 7,	{ "French",			"France",				"FR",	"SPWW2_FR"	} },
};
mkHISTORY(winspww2_history_FR);

static const HISTEL winspww2_history_GE_list[] = {
	{ 1930,	 1,	{ "German",			"Weimar Republic",			"GE",	"SPWW2_GE_wmr"	} },
	{ 1933,	 4,	{ "German",			"German Empire",			"GE",	"SPWW2_GE_emp"	} },
	{ 1935,	 9,	{ "German",			"Nazi Germany",				"GE",	"SPWW2_GE"	} },
};
mkHISTORY(winspww2_history_GE);

static const HISTEL winspww2_history_YU_list[] = {
	{ 1930,	 1,	{ "Yugoslavian",		"Yugoslavia",				"YU",	"SPWW2_YU_old"	} },
	{ 1941,	 6,	{ "Yugoslavian",		"Democratic Federal Yugoslavia",	"YU",	"SPWW2_YU"	} },
};
mkHISTORY(winspww2_history_YU);

static const HISTEL winspww2_history_RS_list[] = {
	{ 1930,	 1,	{ "Republican Spanish",		"Republican Spain",			"RS",	"SPWW2_RS_old"	} },
	{ 1931,	 4,	{ "Republican Spanish",		"Republican Spain",			"RS",	"SPWW2_RS"	} },
};
mkHISTORY(winspww2_history_RS);

static const HISTEL winspww2_history_NS_list[] = {
	{ 1936,	 7,	{ "Nationalist Spanish",	"Nationalist Spain",			"NS",	"SPWW2_NS_old"	} },
	{ 1937,	 1,	{ "Nationalist Spanish",	"Nationalist Spain",			"NS",	"SPWW2_NS"	} },
};
mkHISTORY(winspww2_history_NS);

static const IDMAP winspww2_idmap[] = {
	{  1,	{ "Slovakian",		"Slovak Republic",		"SK",	"SPWW2_SK"	},
		1939,	 4,	1943,	11,	NULL					},
	{  2,	{ "Polish",		"Poland",			"PO",	"SPWW2_PO"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{  3,	{ "Manchurian",		"Manchukuo",			"MN",	"SPWW2_MN"	},
		1932,	 3,	1945,	 9,	NULL					},
	{  4,	{ "Fascist Italian",	"Italian Social Republic",	"ISR",	"SPWW2_ISR"	},
		1943,	10,	1945,	 4,	NULL					},
	{  5,	{ "Japanese",		"Japan",			"JA",	"SPWW2_JA"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{  6,	{ "French",		"France",			"FR",	"SPWW2_FR"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	&winspww2_history_FR			},
	{  7,	{ "British",		"Great Britain",		"BR",	"SPWW2_BR"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{  8,	{ "Vichy French",	"Vichy France",			"VFR",	"SPWW2_VFR"	},
		1940,	 7,	1943,	 5,	NULL					},
	{  9,	{ "Polish",		"People's Army of Poland",	"LWP",	"SPWW2_LWP"	},
		1942,	 5,	1947,	 1,	NULL					},
	{ 11,	{ "Soviet",		"Soviet Union",			"SO",	"SPWW2_SO"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 12,	{ "US Army",		"US Army",			"USA",	"SPWW2_USA"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 13,	{ "US Marine",		"US Marine Corps",		"USMC",	"SPWW2_USMC"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 14,	{ "Communist Chinese",	"Communist China",		"CC",	"SPWW2_CC"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 15,	{ "ANZAC",		"ANZAC",			"ANZ",	"SPWW2_ANZ"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 16,	{ "German",		"Germany",			"GE",	"SPWW2_GE"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	&winspww2_history_GE			},
	{ 18,	{ "Indian",		"India",			"IN",	"SPWW2_IN"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 19,	{ "Bulgarian",		"Bulgaria",			"BU",	"SPWW2_BU"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 20,	{ "Yugoslavian",	"Yugoslavia",			"YU",	"SPWW2_YU"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	&winspww2_history_YU			},
	{ 21,	{ "Nationalist Chinese","Nationalist China",		"NC",	"SPWW2_NC"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 27,	{ "Belgian",		"Belgium",			"BE",	"SPWW2_BE"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 28,	{ "Dutch",		"Netherlands",			"NL",	"SPWW2_NL"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 29,	{ "Norwegian",		"Norway",			"NOR",	"SPWW2_NOR"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 30,	{ "Canadian",		"Canada",			"CA",	"SPWW2_CA"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 31,	{ "Greek",		"Greece",			"GK",	"SPWW2_GK"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 32,	{ "Republican Spanish",	"Republican Spain",		"RS",	"SPWW2_RS"	},
		1930,	 1,	1939,	 5,	&winspww2_history_RS			},
	{ 33,	{ "Nationalist Spanish","Nationalist Spain",		"NS",	"SPWW2_NS"	},
		1936,	 7,	1947,	 1,	&winspww2_history_NS			},
	{ 34,	{ "Italian",		"Italy",			"IT",	"SPWW2_IT"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 35,	{ "Finnish",		"Finland",			"FI",	"SPWW2_FI"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 36,	{ "Swedish",		"Sweden",			"SE",	"SPWW2_SE"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 37,	{ "Czechoslovakian",	"Czechoslovakia",		"CS",	"SPWW2_CS"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 38,	{ "Hungarian",		"Hungary",			"HU",	"SPWW2_HU"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 39,	{ "Romanian",		"Romania",			"RO",	"SPWW2_RO"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 40,	{ "Green",		"Green",			"GRN",	"SPWW2_GRN"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 41,	{ "Thai",		"Thailand",			"THA",	"SPWW2_THA"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 40,	{ "Blue",		"Blue",				"BLU",	"SPWW2_BLU"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
	{ 40,	{ "Red",		"Red",				"RED",	"SPWW2_RED"	},
		DEFAULT_WINSPWW2_DATE_RANGE,	NULL					},
};

#endif	/* SPWOOB_NATIONS_WINSPWW2_H */
