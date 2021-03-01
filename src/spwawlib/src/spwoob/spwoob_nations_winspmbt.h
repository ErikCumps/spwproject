/** \file
* The SPWaW Library - OOB handling - winSPMBT nations info.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SPWOOB_NATIONS_WINSPMBT_H
#define	SPWOOB_NATIONS_WINSPMBT_H	1

#include "spwoob_nations.h"

#define	DEFAULT_WINSPMBT_DATE_RANGE	1946, 01, 2026, 01

static const HISTEL winspmbt_history_EG_list[] = {
	{ 1946,	 1,	{ "Egyptian",		"Kingdom of Egypt",			"EG",	"SPWMBT_EG_kng"	} },
	{ 1953,	 7,	{ "Egyptian",		"Republic of Egypt",			"EG",	"SPWMBT_EG_rep"	} },
	{ 1958,	 2,	{ "Egyptian",		"United Arab Republic",			"EG",	"SPWMBT_EG_uar"	} },
	{ 1971,	 5,	{ "Egyptian",		"Arab Republic of Egypt",		"EG",	"SPWMBT_EG"	} },
};
mkHISTORY(winspmbt_history_EG);

static const HISTEL winspmbt_history_SY_list[] = {
	{ 1946,	 1,	{ "Syrian",		"Syrian Republic",			"SY",	"SPWMBT_SY_rep"	} },
	{ 1958,	 4,	{ "Syrian",		"United Arab Republic",			"SY",	"SPWMBT_SY_uar"	} },
	{ 1961,	10,	{ "Syrian",		"Syrian Arab Republic",			"SY",	"SPWMBT_SY_rep"	} },
	{ 1963,	 3,	{ "Syrian",		"Syrian Republic",			"SY",	"SPWMBT_SY"	} },
};
mkHISTORY(winspmbt_history_SY);

static const HISTEL winspmbt_history_IR_list[] = {
	{ 1946,	 1,	{ "Iranian",		"Iran",					"IR",	"SPWMBT_IR_old"	} },
	{ 1979,	 1,	{ "Iranian",		"Islamic Republic of Iran",		"IR",	"SPWMBT_IR"	} },
};
mkHISTORY(winspmbt_history_IR);

static const HISTEL winspmbt_history_RU_list[] = {
	{ 1946,	 1,	{ "Soviet",		"Soviet Union",				"RU",	"SPWMBT_RU_old"	} },
	{ 1991,	 8,	{ "Russian",		"Russian Federation",			"RU",	"SPWMBT_RU"	} },
};
mkHISTORY(winspmbt_history_RU);

static const HISTEL winspmbt_history_IQ_list[] = {
	{ 1946,	 1,	{ "Iraqi",		"Kingdom of Iraqi",			"IQ",	"SPWMBT_IQ_kng"	} },
	{ 1959,	 1,	{ "Iraqi",		"Iraqi Republic",			"IQ",	"SPWMBT_IQ_rep"	} },
	{ 1963,	 8,	{ "Iraqi",		"Iraq",					"IQ",	"SPWMBT_IQ"	} },
	{ 2008,	 1,	{ "Iraqi",		"Iraq",					"IQ",	"SPWMBT_IQ_pst"	} },
};
mkHISTORY(winspmbt_history_IQ);

static const HISTEL winspmbt_history_AF_list[] = {
	{ 1946,	 1,	{ "Afghani",		"Kingdom of Afghanistan",		"AF",	"SPWMBT_AF_kng"	} },
	{ 1974,	 1,	{ "Afghani",		"Republic of Afghanistan",		"AF",	"SPWMBT_AF_rep1"} },
	{ 1978,	 1,	{ "Afghani",		"Democratic Republic of Afghanistan",	"AF",	"SPWMBT_AF_drep1"} },
	{ 1980,	 1,	{ "Afghani",		"Soviet Occupied Afghanistan",		"AF",	"SPWMBT_AF_sov"	} },
	{ 1987,	 1,	{ "Afghani",		"Democratic Republic of Afghanistan",	"AF",	"SPWMBT_AF_drep2"} },
	{ 1992,	 1,	{ "Afghani",		"Islamic State of Afghanistan",		"AF",	"SPWMBT_AF_isl"	} },
	{ 1995,	 1,	{ "Afghani",		"Northern Alliance of Afghanistan",	"AF",	"SPWMBT_AF_nor"	} },
	{ 1997,	 1,	{ "Taliban",		"Emirate of Afghanistan",		"AF",	"SPWMBT_AF_tal"	} },
	{ 2002,	 2,	{ "Afghani",		"Islamic Republic of Afghanistan",	"AF",	"SPWMBT_AF_rep2"} },
};
mkHISTORY(winspmbt_history_AF);

static const HISTEL winspmbt_history_CA_list[] = {
	{ 1946,	 1,	{ "Canadian",		"Canada",				"CA",	"SPWMBT_CA_old"	} },
	{ 1965,	 2,	{ "Canadian",		"Canada",				"CA",	"SPWMBT_CA"	} },
};
mkHISTORY(winspmbt_history_CA);

static const HISTEL winspmbt_history_ES_list[] = {
	{ 1946,	 1,	{ "Spanish",		"Spain",				"ES",	"SPWMBT_ES_old"	} },
	{ 1981,	 1,	{ "Spanish",		"Spain",				"ES",	"SPWMBT_ES"	} },
};
mkHISTORY(winspmbt_history_ES);

static const HISTEL winspmbt_history_CZ_list[] = {
	{ 1946,	 1,	{ "Czechoslovakian",	"Czechoslovakia",			"CZ",	"SPWMBT_CZ_old"	} },
	{ 1993,	 1,	{ "Czech",		"Czech Republic",			"CZ",	"SPWMBT_CZ"	} },
};
mkHISTORY(winspmbt_history_CZ);

static const HISTEL winspmbt_history_RO_list[] = {
	{ 1946,	 1,	{ "Romanian",		"Romania",				"RO",	"SPWMBT_RO_old"	} },
	{ 1989,	12,	{ "Romanian",		"Romania",				"RO",	"SPWMBT_RO"	} },
};
mkHISTORY(winspmbt_history_RO);

static const HISTEL winspmbt_history_LY_list[] = {
	{ 1946,	 1,	{ "Libyan",		"Libya",				"LY",	"SPWMBT_LY_old"	} },
	{ 1951,	12,	{ "Libyan",		"Kingdom of Libya",			"LY",	"SPWMBT_LY_kng"	} },
	{ 1977,	 1,	{ "Libyan",		"Libyan Arab Republic",			"LY",	"SPWMBT_LY_rep"	} },
	{ 2011,	11,	{ "Libyan",		"Libya",				"LY",	"SPWMBT_LY"	} },
	
};
mkHISTORY(winspmbt_history_LY);

static const HISTEL winspmbt_history_YU_list[] = {
	{ 1946,	 1,	{ "Yugoslavian",	"Yugoslavia",				"YU",	"SPWMBT_YU"	} },
	{ 1992,	 4,	{ "Yugoslavian",	"Federal Republic of Yugoslavia",	"YU",	"SPWMBT_YU_rep"	} },
	{ 2003,	 2,	{ "Serbian",		"Serbia and Montenegro",		"YU",	"SPWMBT_YU_srb"	} },
	{ 2006,	 7,	{ "Serbian",		"Serbia",				"YU",	"SPWMBT_YU_srb"	} },
};
mkHISTORY(winspmbt_history_YU);

static const HISTEL winspmbt_history_ZA_list[] = {
	{ 1946,	 1,	{ "South African",	"South Africa",				"ZA",	"SPWMBT_ZA_old"	} },
	{ 1995,	 1,	{ "South African",	"South Africa",				"ZA",	"SPWMBT_ZA"	} },
};
mkHISTORY(winspmbt_history_ZA);

static const HISTEL winspmbt_history_BA_list[] = {
	{ 1946,	 1,	{ "Bosnian",		"Bosnia-Herzegovina",			"BA",	"SPWMBT_BA_old"	} },
	{ 1995,	 1,	{ "Bosnian",		"Bosnia-Herzegovina",			"BA",	"SPWMBT_BA"	} },
};
mkHISTORY(winspmbt_history_BA);

static const HISTEL winspmbt_history_SD_list[] = {
	{ 1956,	 1,	{ "Sudanese",		"Sudan",				"SD",	"SPWMBT_SD_old"	} },
	{ 1970,	 5,	{ "Sudanese",		"Sudan",				"SD",	"SPWMBT_SD"	} },
};
mkHISTORY(winspmbt_history_SD);

static const HISTEL winspmbt_history_KE_list[] = {
	{ 1946,	 1,	{ "Kenyan",		"British Kenya",			"KE",	"SPWMBT_KE_old"	} },
	{ 1963,	12,	{ "Kenyan",		"Kenya",				"KE",	"SPWMBT_KE"	} },
};
mkHISTORY(winspmbt_history_KE);

static const HISTEL winspmbt_history_VC_list[] = {
	{ 1946,	12,	{ "Viet Minh",		"Viet Minh",				"VM",	"SPWMBT_VM"	} },
	{ 1960,	12,	{ "Viet Cong",		"Viet Cong",				"VC",	"SPWMBT_VC"	} },
};
mkHISTORY(winspmbt_history_VC);

static const HISTEL winspmbt_history_MY_list[] = {
	{ 1946,	 1,	{ "Malaysian",		"Malayan Union",			"MY",	"SPWMBT_MY_old"	} },
	{ 1948,	 2,	{ "Malaysian",		"Federation of Malaya",			"MY",	"SPWMBT_MY_old"	} },
	{ 1950,	 5,	{ "Malaysian",		"Federation of Malaya",			"MY",	"SPWMBT_MY"	} },
	{ 1963,	 9,	{ "Malaysian",		"Malaysia",				"MY",	"SPWMBT_MY"	} },
};
mkHISTORY(winspmbt_history_MY);

static const HISTEL winspmbt_history_YE_list[] = {
	{ 1946,	 1,	{ "North Yemenite",	"North Yemen",				"YE",	"SPWMBT_YE_old"	} },
	{ 1962,	 9,	{ "Yemenite",		"Yemen Arab Republic",			"YE",	"SPWMBT_YE_rep"	} },
	{ 1990,	 4,	{ "Yemenite",		"Yemen",				"YE",	"SPWMBT_YE"	} },
};
mkHISTORY(winspmbt_history_YE);

static const HISTEL winspmbt_history_RHD_list[] = {
	{ 1948,	 1,	{ "Rhodesian",		"Southern Rhodesia",			"RHD",	"SPWMBT_RHD_south"} },
	{ 1953,	 1,	{ "Rhodesian",		"Federation of Rhodesia",		"RHD",	"SPWMBT_RHD_FED"} },
	{ 1964,	 1,	{ "Rhodesian",		"Rhodesia",				"RHD",	"SPWMBT_RHD_old"} },
	{ 1965,	 5,	{ "Rhodesian",		"Rhodesia",				"RHD",	"SPWMBT_RHD"	} },
	{ 1979,	 5,	{ "Rhodesian",		"Zimbabwe-Rhodesia",			"RHD",	"SPWMBT_RHD_zw"	} },
};
mkHISTORY(winspmbt_history_RHD);

static const HISTEL winspmbt_history_GE_list[] = {
	{ 1991,	 4,	{ "Georgian",		"Georgia",				"GE",	"SPWMBT_GE_old"	} },
	{ 2004,	 1,	{ "Georgian",		"Georgia",				"GE",	"SPWMBT_GE"	} },
};
mkHISTORY(winspmbt_history_GE);

static const IDMAP winspmbt_idmap[] = {
	{  1,	{ "Egyptian",		"Egypt",			"EG",	"WINSPMBT_EG"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	&winspmbt_history_EG			},
	{  2,	{ "Polish",		"Poland",			"PL",	"WINSPMBT_PL"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{  3,	{ "Syrian",		"Syria",			"SY",	"WINSPMBT_SY"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	&winspmbt_history_SY			},
	{  4,	{ "Israeli",		"Israel",			"IL",	"WINSPMBT_IL"		},
		1948,	 5,	2026,	 1,	NULL					},
	{  5,	{ "Japanese",		"Japan",			"JP",	"WINSPMBT_JP"		},
		1954,	 7,	2026,	 1,	NULL					},
	{  6,	{ "French",		"France",			"FR",	"WINSPMBT_FR"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{  7,	{ "British",		"Great Britain",		"GB",	"WINSPMBT_GB"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{  8,	{ "PLO",		"PLO",				"PLO",	"WINSPMBT_PLO"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{  9,	{ "Jordanian",		"Jordan",			"JO",	"WINSPMBT_JO"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 10,	{ "Iranian",		"Iran",				"IR",	"WINSPMBT_IR"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	&winspmbt_history_IR			},
	{ 11,	{ "Russian",		"Russia",			"RU",	"WINSPMBT_RU"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	&winspmbt_history_RU			},
	{ 12,	{ "US Army",		"US Army",			"USA",	"WINSPMBT_USA"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 13,	{ "US Marine",		"US Marine Corps",		"USMC",	"WINSPMBT_USMC"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 14,	{ "Chinese",		"China",			"CN",	"WINSPMBT_CN"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 15,	{ "Australian",		"Australia",			"AU",	"WINSPMBT_AU"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 16,	{ "Gulf State",		"Gulf States",			"GS",	"WINSPMBT_GS"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 17,	{ "Iraqi",		"Iraq",				"IQ",	"WINSPMBT_IQ"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	&winspmbt_history_IQ			},
	{ 18,	{ "Indian",		"India",			"IN",	"WINSPMBT_IN"		},
		1947,	 8,	2026,	 1,	NULL					},
	{ 19,	{ "North Korean",	"North Korea",			"KP",	"WINSPMBT_KP"		},
		1948,	 9,	2026,	 1,	NULL					},
	{ 20,	{ "South Korean",	"South Korea",			"KR",	"WINSPMBT_KR"		},
		1948,	 8,	2026,	 1,	NULL					},
	{ 21,	{ "Taiwanese",		"Taiwan",			"TW",	"WINSPMBT_TW"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 22,	{ "Mujahideen",		"Mujahideen",			"MJH",	"WINSPMBT_MJH"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 23,	{ "North Vietnamese",	"North Vietnam",		"NVN",	"WINSPMBT_NVN"		},
		1948,	 1,	2026,	 1,	NULL					},
	{ 24,	{ "South Vietnamese",	"South Vietnam",		"SVN",	"WINSPMBT_SVN"		},
		1948,	 1,	1975,	 5,	NULL					},
	{ 25,	{ "Pakistani",		"Pakistan",			"PK",	"WINSPMBT_PK"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 26,	{ "Afghani",		"Afghanistan",			"AF",	"WINSPMBT_AF_rep2"	},
		DEFAULT_WINSPMBT_DATE_RANGE,	&winspmbt_history_AF			},
	{ 27,	{ "Belgian",		"Belgium",			"BE",	"WINSPMBT_BE"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 28,	{ "Dutch",		"Netherlands",			"NL",	"WINSPMBT_NL"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 29,	{ "Norwegian",		"Norway",			"NO",	"WINSPMBT_NO"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 30,	{ "Canadian",		"Canada",			"CA",	"WINSPMBT_CA"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	&winspmbt_history_CA			},
	{ 31,	{ "Greek",		"Greece",			"GR",	"WINSPMBT_GR"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 32,	{ "Turkish",		"Turkey",			"TR",	"WINSPMBT_TR"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 33,	{ "Spanish",		"Spain",			"ES",	"WINSPMBT_ES"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	&winspmbt_history_ES			},
	{ 34,	{ "Italian",		"Italy",			"IT",	"WINSPMBT_IT"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 35,	{ "Danish",		"Denmark",			"DK",	"WINSPMBT_DK"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 36,	{ "East German",	"East Germany",			"EDE",	"WINSPMBT_EDE"		},
		1949,	10,	1990,	10,	NULL					},
	{ 37,	{ "Czech",		"Czech Republic",		"CZ",	"WINSPMBT_CZ"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	&winspmbt_history_CZ			},
	{ 38,	{ "Hungarian",		"Hungary",			"HU",	"WINSPMBT_HU"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 39,	{ "Romanian",		"Romania",			"RO",	"WINSPMBT_RO"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	&winspmbt_history_RO			},
	{ 40,	{ "Libyan",		"Libya",			"LY",	"WINSPMBT_LY"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	&winspmbt_history_LY			},
	{ 41,	{ "Yugoslavian",	"Yugoslavia",			"YU",	"WINSPMBT_YU"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	&winspmbt_history_LY			},
	{ 42,	{ "Green",		"Green",			"GRN",	"WINSPMBT_GRN"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 43,	{ "Red",		"Red",				"RED",	"WINSPMBT_RED"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 44,	{ "German",		"Germany",			"DE",	"WINSPMBT_DE"		},
		1949,	 5,	2026,	 1,	NULL					},
	{ 45,	{ "Argentinian",	"Argentina",			"AR",	"WINSPMBT_AR"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 46,	{ "Austrian",		"Austria",			"AT",	"WINSPMBT_AT"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 47,	{ "Cypriot",		"Cyprus",			"CY",	"WINSPMBT_CY"		},
		1960,	 8,	2026,	 1,	NULL					},
	{ 48,	{ "Brazilian",		"Brazil",			"BR",	"WINSPMBT_BR"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 49,	{ "Cuban",		"Cuba",				"CU",	"WINSPMBT_CU"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 50,	{ "Algerian",		"Algeria",			"DZ",	"WINSPMBT_DZ"		},
		1954,	11,	2026,	1,	NULL					},
	{ 51,	{ "Slovakian",		"Slovakia",			"SK",	"WINSPMBT_SK"		},
		1993,	 1,	2026,	1,	NULL					},
	{ 52,	{ "South African",	"South Africa",			"ZA",	"WINSPMBT_ZA"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	&winspmbt_history_ZA			},
	{ 53,	{ "Swiss",		"Switzerland",			"CH",	"WINSPMBT_CH"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 54,	{ "Thai",		"Thailand",			"TH",	"WINSPMBT_TH"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 55,	{ "Albanian",		"Albania",			"AL",	"WINSPMBT_AL"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 56,	{ "Angolan",		"Angola",			"AO",	"WINSPMBT_AO"		},
		1975,	11,	2026,	 1,	NULL					},
	{ 57,	{ "Bosnian",	"Bosnia-Herzegovina",			"BA",	"WINSPMBT_BA"		},
		1992,	 3,	2026,	 1,	&winspmbt_history_BA			},
	{ 58,	{ "Cambodian",		"Cambodia",			"KH",	"WINSPMBT_KH"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 59,	{ "Chilean",		"Chile",			"CL",	"WINSPMBT_CL"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 60,	{ "Croatian",		"Croatia",			"HR",	"WINSPMBT_HR"		},
		1991,	 6,	2026,	 1,	NULL					},
	{ 61,	{ "Nigerian",		"Nigeria",			"NG",	"WINSPMBT_NG"		},
		1960,	10,	2026,	 1,	NULL					},
	{ 62,	{ "Chadian",		"Chad",				"TD",	"WINSPMBT_TD"		},
		1960,	 8,	2026,	 1,	NULL					},
	{ 63,	{ "Ethiopian",		"Ethiopia",			"ET",	"WINSPMBT_ET"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 64,	{ "Saudi",		"Saudi Arabia",			"SA",	"WINSPMBT_SA"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 65,	{ "Finnish",		"Finland",			"FI",	"WINSPMBT_FI"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 66,	{ "Swedish",		"Sweden",			"SE",	"WINSPMBT_SE"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 67,	{ "Sudanese",		"Sudan",			"SD",	"WINSPMBT_SD"		},
		1956,	 1,	2026,	 1,	&winspmbt_history_SD			},
	{ 68,	{ "Lebanese",		"Lebanon",			"LB",	"WINSPMBT_LB"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 69,	{ "Kenyan",		"Kenya",			"KE",	"WINSPMBT_KE"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	&winspmbt_history_KE			},
	{ 70,	{ "Chechen",		"Chechnya",			"CCY",	"WINSPMBT_CCY"		},
		1990,	11,	2000,	 6,	NULL					},
	{ 71,	{ "Indonesian",		"Indonesia",			"ID",	"WINSPMBT_ID"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 72,	{ "Viet Cong",		"Viet Cong",			"VC",	"WINSPMBT_VC"		},
		1946,	12,	1975,	 4,	&winspmbt_history_VC			},
	{ 73,	{ "Portuguese",		"Portugal",			"PT",	"WINSPMBT_PT"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 74,	{ "Malaysian",		"Malaysia",			"MY",	"WINSPMBT_MY"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	&winspmbt_history_MY			},
	{ 75,	{ "Zimbabwean",		"Zimbabwe",			"ZW",	"WINSPMBT_ZW"		},
		1980,	 5,	2026,	 1,	NULL					},
	{ 76,	{ "South Yemenite",	"South Yemen",			"SYE",	"WINSPMBT_SYE"		},
		1967,	11,	1990,	 5,	NULL					},
	{ 77,	{ "Yemenite",		"Yemen",			"YE",	"WINSPMBT_YE"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	&winspmbt_history_YE			},
	{ 78,	{ "Eritrean",		"Eritrea",			"ER",	"WINSPMBT_ER"		},
		1963,	 1,	2026,	 1,	NULL					},
	{ 79,	{ "Ukrainian",		"Ukrain",			"UA",	"WINSPMBT_UA"		},
		1991,	 8,	2026,	 1,	NULL					},
	{ 80,	{ "UN",			"United Nations",		"UN",	"WINSPMBT_UN"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 81,	{ "Mozambican",		"Mozambique",			"MZ",	"WINSPMBT_MZ"		},
		1974,	 9,	2026,	 1,	NULL					},
	{ 82,	{ "Ecuadorian",		"Ecuador",			"EC",	"WINSPMBT_EC"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 83,	{ "Salvadoran",		"El Salvador",			"SV",	"WINSPMBT_SV"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 84,	{ "Nicaraguan",		"Nicaragua",			"NI",	"WINSPMBT_NI"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 85,	{ "Peruvian",		"Peru",				"PE",	"WINSPMBT_PE"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 86,	{ "Rhodesian",		"Rhodesia",			"RHD",	"WINSPMBT_RHD"		},
		1948,	 1,	1980,	 5,	&winspmbt_history_RHD			},
	{ 87,	{ "Bulgarian",		"Bulgaria",			"BG",	"WINSPMBT_BG"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 88,	{ "Somalian",		"Somalia",			"SO",	"WINSPMBT_SO"		},
		1960,	 7,	2026,	 1,	NULL					},
	{ 89,	{ "Tanzanian",		"Tanzania",			"TZ",	"WINSPMBT_TZ"		},
		1964,	 4,	2026,	 1,	NULL					},
	{ 90,	{ "Ugandan",		"Uganda",			"UG",	"WINSPMBT_UG"		},
		1962,	10,	2026,	 1,	NULL					},
	{ 91,	{ "Uruguayan",		"Uruguay",			"UY",	"WINSPMBT_UY"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 92,	{ "Paraguayan",		"Paraguay",			"PY",	"WINSPMBT_PY"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 93,	{ "Georgian",		"Georgia",			"GE",	"WINSPMBT_GE"		},
		1991,	 4,	2026,	 1,	&winspmbt_history_GE			},
	{ 94,	{ "R&D #94",		"R&D #94",			"RD94",	"WINSPMBT_RD94"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 95,	{ "R&D #95",		"R&D #95",			"RD95",	"WINSPMBT_RD95"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 96,	{ "R&D #96",		"R&D #96",			"RD96",	"WINSPMBT_RD96"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 97,	{ "R&D #97",		"R&D #97",			"RD97",	"WINSPMBT_RD97"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 98,	{ "R&D #98",		"R&D #98",			"RD98",	"WINSPMBT_RD98"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
	{ 99,	{ "R&D #99",		"R&D #99",			"RD99",	"WINSPMBT_RD99"		},
		DEFAULT_WINSPMBT_DATE_RANGE,	NULL					},
};

#endif	/* SPWOOB_NATIONS_WINSPMBT_H */
