#include "stdafx.h"
#include <spwawlib_spwoob.h>
#include "common/internal.h"

typedef struct s_IDSTRMAP {
	BYTE		id;
	const char	*people;
	const char	*nation;
	const char	*prefix;
} IDSTRMAP;

static const IDSTRMAP spwoob_idstrings_map[] = {
	{  9,	"Communist Chinese",	"Communist China",	"CC"	},
	{ 15,	"Czechoslovakian",	"Czechoslovakia",	"CS"	},
	{ 16,	"Philippine",		"Philippines",		"PI"	},
	{ 19,	"Nationalist Spanish",	"Nationalist Spain",	"NS"	},
	{ 20,	"Republican Spanish",	"Republican Spain",	"RS"	},
	{ 29,	"Bulgarian",		"Bulgaria",		"BU"	},
	{ 70,	"German",		"Germany",		"GE"	},
	{ 71,	"Finnish",		"Finland",		"FI"	},
	{ 72,	"Italian",		"Italy",		"IT"	},
	{ 73,	"Romanian",		"Romania",		"RO"	},
	{ 74,	"Hungarian",		"Hungary",		"HU"	},
	{ 75,	"Japanese",		"Japan",		"JA"	},
	{ 76,	"French",		"France",		"FR"	},
	{ 77,	"British",		"United Kingdom",	"BR"	},
	{ 78,	"Belgian",		"Belgium",		"BE"	},
	{ 79,	"Dutch",		"Netherlands",		"NL"	},
	{ 80,	"Polish",		"Poland",		"PO"	},
	{ 81,	"Soviet",		"Soviet Union",		"SO"	},
	{ 82,	"US army",		"US Army",		"US"	},
	{ 83,	"US marine",		"US Marine Corps",	"USMC"	},
	{ 84,	"Nationalist Chinese",	"Nationalist China",	"NC"	},
	{ 85,	"Greek",		"Greece",		"GK"	},
	{ 86,	"Norwegian",		"Norway",		"NOR"	},
	{ 87,	"Yugoslavian",		"Yugoslavia",		"YU"	},
	{ 88,	"Canadian",		"Canada",		"CA"	},
	{ 89,	"Indian",		"India",		"IN"	},
	{ 90,	"ANZAC",		"ANZAC",		"ANZ"	},
	{ 91,	"Free French",		"Free France",		"FF"	},
	{  0,	"???",			"???",			"??"	}
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
