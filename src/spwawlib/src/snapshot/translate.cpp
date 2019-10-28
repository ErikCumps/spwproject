/** \file
 * The SPWaW Library - snapshot handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "snapshot/translate.h"
#include "common/types.h"
#include "common/internal.h"

SPWAW_TERRAIN
raw2terrain (BYTE id)
{
	SPWAW_TERRAIN	rc;

	switch (id) {
		case T_DESERT:
			rc = SPWAW_TDESERT;
			break;
		case T_SUMMER:
			rc = SPWAW_TSUMMER;
			break;
		case T_WINTER:
			rc = SPWAW_TWINTER;
			break;
		case T_JUNGLE:
			rc = SPWAW_TJUNGLE;
			break;
		case T_ROUGH:
			rc = SPWAW_TROUGH;
			break;
		default:
			rc = SPWAW_TUNKNOWN;
			break;
	}
	return (rc);
}

SPWAW_WEATHER
raw2weather (BYTE id)
{
	SPWAW_WEATHER	rc;

	switch (id) {
		case W_BEST:
			rc = SPWAW_WBEST;
			break;
		case W_GOOD:
			rc = SPWAW_WGOOD;
			break;
		case W_MEDIUM:
			rc = SPWAW_WMEDIUM;
			break;
		case W_BAD:
			rc = SPWAW_WBAD;
			break;
		case W_VERYBAD:
			rc = SPWAW_WVERYBAD;
			break;
		case W_WORST:
			rc = SPWAW_WWORST;
			break;
		default:
			rc = SPWAW_WUNKNOWN;
			break;
	}
	return (rc);
}

SPWAW_MISSION
raw2mission (BYTE id)
{
	SPWAW_MISSION	rc;

	switch (id) {
		case M_ASSAULT:
			rc = SPWAW_MASSAULT;
			break;
		case M_ADVANCE:
			rc = SPWAW_MADVANCE;
			break;
		case M_DELAY:
			rc = SPWAW_MDELAY;
			break;
		case M_DEFEND:
			rc = SPWAW_MDEFEND;
			break;
		default:
			rc = SPWAW_MUNKNOWN;
			break;
	}
	return (rc);
}

SPWAW_FSTATUS
raw2fstatus (BYTE id)
{
	SPWAW_FSTATUS	rc;

	switch (id) {
		case F_REG:
			rc = SPWAW_FREG;
			break;
		case F_CORE:
			rc = SPWAW_FCORE;
			//TEST to force 0 core formations
			//rc = SPWAW_FAUX;
			//TEST
			break;
		case F_AUX:
			rc = SPWAW_FAUX;
			break;
		default:
			rc = SPWAW_FUNKNOWN;
			break;
	}
	return (rc);
}

SPWAW_USTATUS
raw2ustatus (BYTE id)
{
	SPWAW_USTATUS	rc;

	switch (id) {
		case U_READY:
			rc = SPWAW_UREADY;
			break;
		case U_PINNED:
			rc = SPWAW_UPINNED;
			break;
		case U_RETREATING:
			rc = SPWAW_URETREATING;
			break;
		case U_ROUTED:
			rc = SPWAW_UROUTED;
			break;
		case U_ABANDONED:
			rc = SPWAW_UABANDONED;
			break;
		case U_DESTROYED:
			rc = SPWAW_UDESTROYED;
			break;
		case U_BURNING:
			rc = SPWAW_UBURNING;
			break;
		case U_BUTTONED:
			rc = SPWAW_UBUTTONED;
			break;
		case U_IMMOBILIZED:
			rc = SPWAW_UIMMOBILIZED;
			break;
		case U_PASSENGER:
			rc = SPWAW_UPASSENGER;
			break;
		default:
			rc = SPWAW_UUNKNOWN;
			break;
	}
	return (rc);
}

SPWAW_CSTATUS
raw2cstatus (BYTE id)
{
	SPWAW_CSTATUS	rc;

	switch (id) {
		case C_NONE:
			rc = SPWAW_CSTATUSNONE;
			break;
		case C_RADIO:
			rc = SPWAW_CSTATUSRADIO;
			break;
		case C_DIRECT:
			rc = SPWAW_CSTATUSDIRECT;
			break;
		default:
			rc = SPWAW_CSTATUSUNKNOWN;
			break;
	}
	return (rc);
}

SPWAW_ENTR
raw2entr (BYTE id)
{
	SPWAW_ENTR	rc;

	switch (id) {
		case E_NONE:
			rc = SPWAW_ENTRNONE;
			break;
		case E_COVER:
			rc = SPWAW_ENTRCOVER;
			break;
		case E_DUGIN:
			rc = SPWAW_ENTRDUGIN;
			break;
		default:
			rc = SPWAW_ENTRUNKNOWN;
			break;
	}
	return (rc);

}

SPWAW_RANK
raw2rank (BYTE id)
{
	SPWAW_RANK	rc;

	switch (id) {
		case R_KIA:
			rc = SPWAW_RKIA;
			break;
		case R_PVT:
			rc = SPWAW_RPVT;
			break;
		case R_CPL:
			rc = SPWAW_RCPL;
			break;
		case R_SGT:
			rc = SPWAW_RSGT;
			break;
		case R_2LT:
			rc = SPWAW_R2LT;
			break;
		case R_1LT:
			rc = SPWAW_R1LT;
			break;
		case R_CPT:
			rc = SPWAW_RCPT;
			break;
		case R_MAJ:
			rc = SPWAW_RMAJ;
			break;
		case R_LTC:
			rc = SPWAW_RLTC;
			break;
		case R_COL:
			rc = SPWAW_RCOL;
			break;
		case R_GEN:
			rc = SPWAW_RGEN;
			break;
		default:
			rc = SPWAW_RUNKNOWN;
			break;
	}
	return (rc);
}

SPWAW_EXP
raw2exp (BYTE exp)
{
	SPWAW_EXP	e;

	if (exp < 30)
		e = SPWAW_ECADET;
	else if (exp < 50)
		e = SPWAW_EGREEN;
	else if (exp < 80)
		e = SPWAW_EAVERAGE;
	else if (exp < 100)
		e = SPWAW_EVETERAN;
	else	e = SPWAW_EELITE;

	return (e);
}

SPWAW_VHSTATUS
raw2vhstatus (BYTE id)
{
	SPWAW_VHSTATUS	rc;

	switch (id) {
		case VH_PLAYER1:
			rc = SPWAW_VHP1;
			break;
		case VH_PLAYER2:
			rc = SPWAW_VHP2;
			break;
		case VH_NEUTRAL:
			rc = SPWAW_VHN;
			break;
		default:
			rc = SPWAW_VHUNKNOWN;
			break;
	}
	return (rc);
}

SPWAW_BRESULT
raw2bresult (SHORT result)
{
	SPWAW_BRESULT	rc;

	switch (result) {
		case BR_DD:
			rc = SPWAW_BRDD;
			break;
		case BR_MD:
			rc = SPWAW_BRMD;
			break;
		case BR_DB:
			rc = SPWAW_BRDB;
			break;
		case BR_MV:
			rc = SPWAW_BRMV;
			break;
		case BR_DV:
			rc = SPWAW_BRDV;
			break;
		case BR_UNKNOWN:
		default:
			rc = SPWAW_BRUNKNOWN;
			break;
	}
	return (rc);
}



static const char *terrain_strings[SPWAW_TLASTCODE+1] = {
	"desert",
	"summer",
	"winter",
	"jungle",
	"rough",
	"???"
};

char *
terrain2str (SPWAW_TERRAIN id)
{
	return ((char*)terrain_strings[id]);
}

static const char *weather_temperate_strings[SPWAW_WLASTCODE+1] = {
	"clear",
	"hazy",
	"foggy",
	"rain",
	"heavy rain",
	"thunderstorm",
	"???"
};

static const char *weather_desert_strings[SPWAW_WLASTCODE+1] = {
	"clear",
	"clear",
	"clear",
	"dusty",
	"dust storm",
	"sandstorm",
	"???"
};

static const char *weather_winter_strings[SPWAW_WLASTCODE+1] = {
	"clear",
	"clear",
	"clear",
	"light snow",
	"snowy windstorm",
	"heavy snow",
	"???"
};

char *
weather2str (SPWAW_TERRAIN tid, SPWAW_WEATHER wid)
{
	const char	**table = NULL;

	switch (tid) {
		case SPWAW_TSUMMER:
		case SPWAW_TJUNGLE:
		case SPWAW_TROUGH:
		case SPWAW_TUNKNOWN:
			table = weather_temperate_strings;
			break;
		case SPWAW_TDESERT:
			table = weather_desert_strings;
			break;
		case SPWAW_TWINTER:
			table = weather_winter_strings;
			break;
	}
	return ((char*)table[wid]);
}

static const char *mission_strings[SPWAW_MLASTCODE+1] = {
	"assault",
	"advance",
	"delay",
	"defence",
	"???"
};

char *
mission2str (SPWAW_MISSION id)
{
	return ((char*)mission_strings[id]);
}

static const char *fstatus_strings[SPWAW_FLASTCODE+1] = {
	"regular",
	"core",
	"support",
	"???"
};

char *
fstatus2str (SPWAW_FSTATUS id)
{
	return ((char*)fstatus_strings[id]);
}

static const char *ustatus_strings[SPWAW_ULASTCODE+1] = {
	"ready",
	"pinned",
	"retreating",
	"routed",
	"abandoned",
	"destroyed",
	"burning",
	"buttoned",
	"immobilized",
	"passenger",
	"???"
};

char *
ustatus2str (SPWAW_USTATUS id)
{
	return ((char*)ustatus_strings[id]);
}

static const char *cstatus_strings[SPWAW_CSTATUSLASTCODE+1] = {
	"no contact",
	"radio contact",
	"direct contact",
	"???"
};

char *
cstatus2str (SPWAW_CSTATUS id)
{
	return ((char*)cstatus_strings[id]);
}

static const char *entr_strings[SPWAW_ENTRLASTCODE+1] = {
	"positioned",
	"in cover",
	"dug in",
	"???"
};

char *
entr2str (SPWAW_ENTR id)
{
	return ((char*)entr_strings[id]);
}

static const char *rank_strings[SPWAW_RLASTCODE+1] = {
	"KIA",
	"Pvt",
	"Cpl",
	"Sgt",
	"2Lt",
	"1Lt",
	"Cpt",
	"Maj",
	"LtC",
	"Col",
	"Gen",
	"???"
};

char *
rank2str (SPWAW_RANK id)
{
	return ((char*)rank_strings[id]);
}

static const char *exp_strings[SPWAW_ELASTCODE+1] = {
	"cadet",
	"green",
	"average",
	"veteran",
	"elite"
};

char *
exp2str (SPWAW_EXP exp)
{
	return ((char*)exp_strings[exp]);
}

static const char *aband_strings[SPWAW_ALASTCODE+1] = {
	"not abandoned",
	"abandoned unit",
	"abandoned crew"
};

static const char *btstatus_strings[SPWAW_BTLASTCODE+1] = {
	"deployment",
	"in progress",
	"score"
};

char *
btstatus2str (SPWAW_BTSTATUS id)
{
	return ((char*)btstatus_strings[id]);
}

static const char *vhstatus_strings[SPWAW_VHLASTCODE+1] = {
	"neutral",
	"player1",
	"player2",
	"???"
};

char *
vhstatus2str (SPWAW_VHSTATUS id)
{
	return ((char*)vhstatus_strings[id]);
}

static const char *bresult_strings[SPWAW_BRLASTCODE+1] = {
	"decisive victory",
	"marginal victory",
	"drawn battle",
	"marginal defeat",
	"decisive defeat",
	"???"
};

char *
bresult2str (SPWAW_BRESULT result)
{
	return ((char*)bresult_strings[result]);
}


void
FID2str (BYTE id, char *buf, int len)
{
	char	tmp[5], *p;

	if (!buf || !len) return;
	memset (buf, 0, len);

	memset (tmp, 0, sizeof (tmp));
	p = &(tmp[sizeof(tmp)-1]); p--;

	*p = 'A' + (id % 26); id /= 26;
	if (id) {
		id--;
		p--;
		*p = 'A' + (id % 26); id /= 26;
	}
	if (id) {
		id--;
		p--;
		*p = 'A' + (id % 26); id /= 26;
	}
	if (id) {
		id--;
		p--;
		*p = 'A' + (id % 26); id /= 26;
	}
	strncpy (buf, p, len - 1);
}

char *
FID2str (BYTE id)
{
	char	buf[16];

	FID2str (id, buf, sizeof (buf));
	return (safe_strdup(buf));
}

void
UID2str (BYTE id, BYTE subid, char *buf, int len)
{
	char	tmp[16];

	if (!buf || !len) return;
	memset (buf, 0, len);

	memset (tmp, 0, sizeof (tmp));
	FID2str (id, tmp, sizeof (tmp));
	snprintf (buf, len - 1, "%s%u", tmp, subid);
}

char *
UID2str (BYTE id, BYTE subid)
{
	char	buf[16];

	UID2str (id, subid, buf, sizeof (buf));
	return (safe_strdup(buf));
}



char *
aband2str (SPWAW_ABAND id)
{
	return ((char*)aband_strings[id]);
}

SPWAW_ABAND
raw2aband (BYTE aband)
{
	SPWAW_ABAND	a;

	switch (aband) {
		case AS_STAY:
			a = SPWAW_ASTAY;
			break;
		case AS_LEFT:
			a = SPWAW_ALEFT;
			break;
		case AS_NONE:
		default:
			a = SPWAW_ANONE;
			break;
	}
	return (a);
}

BYTE
aband2raw (SPWAW_ABAND aband)
{
	BYTE	b;

	switch (aband) {
		case SPWAW_ASTAY:
			b = AS_STAY;
			break;
		case SPWAW_ALEFT:
			b = AS_LEFT;
			break;
		case SPWAW_ANONE:
		default:
			b = AS_NONE;
			break;
	}
	return (b);
}



SPWAW_UNIT_TYPE
raw2unittype (BYTE type)
{
	SPWAW_UNIT_TYPE	t;

	switch (type) {
		case UT_UNIT:
			t = SPWAW_UNIT_TYPE_UNIT;
			break;
		case UT_CREW:
			t = SPWAW_UNIT_TYPE_CREW;
			break;
		case UT_SPAU:
			t = SPWAW_UNIT_TYPE_SPAU;
			break;
		case UT_UNKNOWN:
		default:
			t = SPWAW_UNIT_TYPE_UNKNOWN;
			break;
	}
	return (t);
}

BYTE
unittype2raw (SPWAW_UNIT_TYPE type)
{
	BYTE	b;

	switch (type) {
		case SPWAW_UNIT_TYPE_UNIT:
			b = UT_UNIT;
			break;
		case SPWAW_UNIT_TYPE_CREW:
			b = UT_CREW;
			break;
		case SPWAW_UNIT_TYPE_SPAU:
			b = UT_SPAU;
			break;
		case SPWAW_UNIT_TYPE_UNKNOWN:
		default:
			b = UT_UNKNOWN;
			break;
	}
	return (b);
}
