/** \file
 * The SPWaW Library - API types implementation.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_types.h>
#include "common/internal.h"
#include "snapshot/translate.h"

#define	MD	31

static unsigned int dec_month_days[12] = { 30, 27, 30, 29, 30, 29, 30, 30, 29, 30, 29, 30 };

#define	INVALID_STAMP		0xffffffffffffffff

static SPWAW_DATE INVALID_DATE	= { 0, 0, 0, 0, 0 };
static SPWAW_DATE EMPTY_DATE	= { SPWAW_STARTYEAR, 0, 0, 0, 0 };

static void	eval_period(SPWAW_PERIOD *period);
static bool	isEmptyDate	(SPWAW_DATE *date);
static bool	isValidDate	(SPWAW_DATE *date);
static bool	isEmptyStamp	(SPWAW_TIMESTAMP *stamp);
static bool	isMonthOnlyStamp(SPWAW_TIMESTAMP *stamp);
static bool	isValidStamp	(SPWAW_TIMESTAMP *stamp);
static bool	isValidPeriod	(SPWAW_PERIOD *period);

SPWAWLIB_API void
SPWAW_set_date (SPWAW_DATE &date, short year, char month, char day, char hour, char minute)
{
	memset (&date, 0, sizeof(date));
	date.year   = year;
	date.month  = month;
	date.day    = day;
	date.hour   = hour;
	date.minute = minute;
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_date2stamp (SPWAW_DATE *date, SPWAW_TIMESTAMP *stamp)
{
	bool		mod = false;
	SYSTEMTIME	st;
	FILETIME	ft;
	ULARGE_INTEGER	mins;

	CNULLARG (date); CNULLARG (stamp);
	*stamp = INVALID_STAMP;

	if (isEmptyDate (date)) {
		*stamp = SPWAW_TIMESTAMP_EMPTY;
		return (SPWERR_OK);
	}

	if (!isValidDate (date)) RWE (SPWERR_BADDATE, "invalid SPWAW date");

	// Month-only dates can not be accurately represented by a stamp!
	// The naive fix of forcing the day to 1 (instead of 0) to generate a timestamp will
	// cause the loss of the original month-only date when converting back.
	// However, because we down-scale the stamp after creation, some additional bits are
	// gained which can be used to indicate the status of the original date.
	mod = SPWAW_isMonthOnlyDate (date);

	memset (&st, 0, sizeof (st));
	st.wYear   = date->year;
	st.wMonth  = date->month;
	st.wDay	   = mod ? 1 : date->day;
	st.wHour   = mod ? 0 : date->hour;
	st.wMinute = mod ? 0 : date->minute;
	if (!SystemTimeToFileTime (&st, &ft)) RWE (SPWERR_BADDATE, "invalid SPWAW date");

	mins.LowPart  = ft.dwLowDateTime;
	mins.HighPart = ft.dwHighDateTime;
	mins.QuadPart /= (10 * 1000 * 1000 * 60); /* convert units of 100 ns to 1 minute */
	*stamp = mins.QuadPart;

	if (mod) *stamp |= SPWAW_MONTHONLY_TIMESTAMP_BITS;

	//log ("%s: date=\"%04.4d/%02.2d/%02.2d %02.2d:%02.2d\" stamp=0x%016.16I64x\n",
	//	__FUNCTION__, date->year, date->month, date->day, date->hour, date->minute, *stamp);

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_stamp2date (SPWAW_TIMESTAMP *stamp, SPWAW_DATE *date)
{
	bool		mod = false;
	ULARGE_INTEGER	ns;
	FILETIME	ft;
	SYSTEMTIME	st;

	CNULLARG (stamp); CNULLARG (date);
	fill_ptr (date, 0xFF);

	if (isEmptyStamp (stamp)) {
		*date = EMPTY_DATE;
		return (SPWERR_OK);
	}

	if (!isValidStamp (stamp)) RWE (SPWERR_BADSTAMP, "invalid SPWAW timestamp");

	mod = isMonthOnlyStamp (stamp);

	ns.QuadPart = *stamp & SPWAW_MONTHONLY_TIMESTAMP_MASK;
	ns.QuadPart *= (10 * 1000 * 1000 * 60); /* convert units of 100 ns to 1 minute */

	ft.dwHighDateTime = ns.HighPart;
	ft.dwLowDateTime  = ns.LowPart;
	if (!FileTimeToSystemTime (&ft, &st)) RWE (SPWERR_BADSTAMP, "invalid SPWAW timestamp");

	date->year   = st.wYear;
	date->month  = (char)st.wMonth;
	date->day    = mod ? 0 : (char)st.wDay;
	date->hour   = mod ? 0 : (char)st.wHour;
	date->minute = mod ? 0 : (char)st.wMinute;

	//log ("%s: stamp=0x%016.16I64x date=\"%04.4d/%02.2d/%02.2d %02.2d:%02.2d\"\n",
	//	__FUNCTION__, *stamp, date->year, date->month, date->day, date->hour, date->minute);

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_date2str (SPWAW_DATE *date, char *buf, int len, bool full)
{
	CNULLARG (date);

	if (!buf || !len) return (SPWERR_OK);
	memset (buf, 0, len);

	if (isEmptyDate (date)) {
		snprintf (buf, len - 1, "(empty date)");
		return (SPWERR_OK);
	}

	if (!isValidDate (date)) RWE (SPWERR_BADSTAMP, "invalid SPWAW date");

	if (date->day > 0) {
		if (full) {
			snprintf (buf, len - 1, "%04d/%02d/%02d %02d:%02d", date->year, date->month, date->day, date->hour, date->minute);
		} else {
			snprintf (buf, len - 1, "%04d/%02d/%02d", date->year, date->month, date->day);
		}
	} else if (date->month > 0) {
		snprintf (buf, len - 1, "%04d/%02d", date->year, date->month);
	} else {
		snprintf (buf, len - 1, "%04d", date->year);
	}

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_date2str (SPWAW_DATE *date, char **str, bool full)
{
	SPWAW_ERROR	rc;
	char		buf[64];

	CNULLARG (date); CNULLARG (str);
	*str = NULL;

	rc = SPWAW_date2str (date, buf, sizeof (buf), full);
	ROE ("SPWAW_date2str(core)");

	*str = strdup (buf);
	if (!*str) RWE (SPWERR_OUTOFMEM, "strdup() failed");

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_period2stamp (SPWAW_PERIOD *period, SPWAW_TIMESTAMP *stamp)
{
	CNULLARG (period); CNULLARG (stamp);
	*stamp = INVALID_STAMP;

	if (!isValidPeriod (period)) RWE (SPWERR_BADSTAMP, "invalid SPWAW period");
	*stamp = period->stamp;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_stamp2period (SPWAW_TIMESTAMP *stamp, SPWAW_PERIOD *period)
{
	CNULLARG (period); CNULLARG (stamp);
	period->stamp = INVALID_STAMP; eval_period (period);

	if (isEmptyStamp (stamp)) return (SPWERR_OK);

	if (!isValidStamp (stamp)) RWE (SPWERR_BADSTAMP, "invalid SPWAW timestamp");

	period->stamp = *stamp; eval_period (period);

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_period2str (SPWAW_PERIOD *period, char *buf, int len)
{
	UtilStrbuf	sb (buf, len, true, true);
	bool		first = true;

	CNULLARG (period);

	if (!isValidPeriod (period)) RWE (SPWERR_BADSTAMP, "invalid SPWAW period");

	if (!buf || !len) return (SPWERR_OK);

	if (period->years != 0) {
		sb.printf ("%s%d years", first?"":", ", period->years);
		first = false;
	}
	if (period->months != 0) {
		sb.printf ("%s%d months", first?"":", ", period->months);
		first = false;
	}
	if (period->weeks != 0) {
		sb.printf ("%s%d weeks", first?"":", ", period->weeks);
		first = false;
	}
	if (period->days != 0) {
		sb.printf ("%s%d days", first?"":", ", period->days);
		first = false;
	}
	if (period->hours != 0) {
		sb.printf ("%s%d hours", first?"":", ", period->hours);
		first = false;
	}
	if (period->minutes != 0) {
		sb.printf ("%s%d minutes", first?"":", ", period->minutes);
	}

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_period2str (SPWAW_PERIOD *period, char **str)
{
	SPWAW_ERROR	rc;
	char		buf[256];

	CNULLARG (period); CNULLARG (str);
	*str = NULL;

	rc = SPWAW_period2str (period, buf, sizeof (buf));
	ROE ("SPWAW_period2str(core)");

	*str = strdup (buf);
	if (!*str) RWE (SPWERR_OUTOFMEM, "strdup() failed");

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_date_delta (SPWAW_DATE *base, SPWAW_DATE *item, SPWAW_PERIOD *delta)
{
	SPWAW_ERROR	rc;
	SPWAW_TIMESTAMP	sb, si;

	CNULLARG (base); CNULLARG (item); CNULLARG (delta);
	delta->stamp = INVALID_STAMP; eval_period (delta);

	if (!isValidDate (base)) RWE (SPWERR_BADSTAMP, "invalid SPWAW base date");
	if (!isValidDate (item)) RWE (SPWERR_BADSTAMP, "invalid SPWAW item date");

	if (SPWAW_isMonthOnlyDate (base) != SPWAW_isMonthOnlyDate (item)) RWE (SPWERR_BADSTAMP, "incompatible SPWAW dates");

	rc = SPWAW_date2stamp (base, &sb); ROE ("SPWAW_date2stamp(base date)");
	rc = SPWAW_date2stamp (item, &si); ROE ("SPWAW_date2stamp(item date)");

	delta->stamp = SPWAW_PURE_TIMESTAMP(si) - SPWAW_PURE_TIMESTAMP(sb); eval_period (delta);

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_date_add (SPWAW_DATE *base, SPWAW_PERIOD *add, SPWAW_DATE *sum)
{
	SPWAW_ERROR	rc;
	SPWAW_TIMESTAMP	stamp;

	CNULLARG (base); CNULLARG (add); CNULLARG (sum);
	*sum = INVALID_DATE;

	if (!isValidDate (base)) RWE (SPWERR_BADSTAMP, "invalid SPWAW date");
	if (SPWAW_isMonthOnlyDate (base)) RWE (SPWERR_BADSTAMP, "incompatible SPWAW date");
	if (!isValidPeriod (add)) RWE (SPWERR_BADSTAMP, "invalid SPWAW period");

	rc = SPWAW_date2stamp (base, &stamp); ROE ("SPWAW_date2stamp(base date)");
	stamp += add->stamp;
	rc = SPWAW_stamp2date (&stamp, sum); ROE ("SPWAW_stamp2date(sum stamp)");

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_period_delta (SPWAW_PERIOD *base, SPWAW_PERIOD *item, SPWAW_PERIOD *delta)
{
	CNULLARG (base); CNULLARG (item); CNULLARG (delta);
	delta->stamp = INVALID_STAMP; eval_period (delta);

	if (!isValidPeriod (base)) RWE (SPWERR_BADSTAMP, "invalid SPWAW base period");
	if (!isValidPeriod (item)) RWE (SPWERR_BADSTAMP, "invalid SPWAW item period");

	delta->stamp = item->stamp - base->stamp; eval_period (delta);

	return (SPWERR_OK);
}


SPWAWLIB_API SPWAW_ERROR
SPWAW_period_add (SPWAW_PERIOD *base, SPWAW_PERIOD *add, SPWAW_PERIOD *sum)
{
	CNULLARG (base); CNULLARG (add); CNULLARG (sum);
	sum->stamp = INVALID_STAMP; eval_period (sum);

	if (!isValidPeriod (base)) RWE (SPWERR_BADSTAMP, "invalid SPWAW base period");
	if (!isValidPeriod (add)) RWE (SPWERR_BADSTAMP, "invalid SPWAW add period");

	sum->stamp = base->stamp + add->stamp; eval_period (sum);

	return (SPWERR_OK);
}

static void
eval_period (SPWAW_PERIOD *period)
{
	SPWAW_TIMESTAMP	stamp;
	int		days, minutes;
	double		months, d;

	if (!period) return;

	stamp = period->stamp;
	clear_ptr (period);
	if ((period->stamp = stamp) == INVALID_STAMP) return;

	if (period->stamp < 0) stamp = -stamp;

	days = (int)(stamp / (60 * 24));
	minutes = (int)(stamp - (days * 60 * 24));

	// Prevent warning C4244
	period->years = (char)((days / 365) & 0xFF);
	days -= period->years * 365;

	months = (double)days / (365.0 / 12.0);
	period->months = (char)floor(months);
	d = (double)period->months * (365.0 / 12.0);
	days -= (int)floor(d);

	// Prevent warning C4244
	period->weeks = (char)((days / 7) & 0xFF);
	days -= period->weeks * 7;

	// Prevent warning C4244
	period->days = (char)(days & 0xFF);

	// Prevent warning C4244
	period->hours   = (char)((minutes / 60) & 0xFF);
	period->minutes = (char)((minutes - period->hours * 60) & 0xFF);
}

static bool
isEmptyDate (SPWAW_DATE *date)
{
	return (((date->year == 0) || (date->year == EMPTY_DATE.year)) &&
		(date->month	== EMPTY_DATE.month	) &&
		(date->day	== EMPTY_DATE.day	) &&
		(date->hour	== EMPTY_DATE.hour	) &&
		(date->minute	== EMPTY_DATE.minute	));
}

SPWAWLIB_API bool
SPWAW_isMonthOnlyDate (SPWAW_DATE *date)
{
	return ((date->year	!= 0			) &&
		(date->month	!= EMPTY_DATE.month	) &&
		(date->day	== EMPTY_DATE.day	) &&
		(date->hour	== EMPTY_DATE.hour	) &&
		(date->minute	== EMPTY_DATE.minute	));
}

static bool
isValidDate (SPWAW_DATE *date)
{
	bool	isleap = false;
	int	feb_days = 28;

	if (!date) return (false);
	if (!date->year) return (false);

	// Early exit when year/month/day/hour/minute are not set
	if (isEmptyDate(date)) return (true);

	// The year is set: determine the leap days
	isleap = (((date->year % 4) == 0) && (((date->year % 100) != 0) || ((date->year % 400) == 0)));
	if (isleap) feb_days = 29;

	// Early exit if year is correct and month/day/hour/minute are not set
	if ((date->month == 0) && (date->day == 0) && (date->hour == 0) && (date->minute == 0)) return (true);

	// Check validity of month
	if ((unsigned int)(date->month - 1) > 11) return (false);

	// Early exit if year/month are correct and day/hour/minute are not set
	if ((date->day == 0) && (date->hour == 0) && (date->minute == 0)) return (true);

	// Check validity of day
	if ((unsigned int)(date->day - 1) > dec_month_days[date->month-1]) return (false);
	if ((date->month == 2) && (date->day > feb_days)) return (false);

	// Early exit if year/month/day are correct and hour/minute are not set
	if ((date->hour == 0) && (date->minute == 0)) return (true);

	// Check validity of hour and minute
	if ((unsigned int)date->hour > 23) return (false);
	if ((unsigned int)date->minute > 59) return (false);

	// if we get here, the date is valid
	return (true);
}

static bool
isEmptyStamp (SPWAW_TIMESTAMP *stamp)
{
	return (stamp && (*stamp == SPWAW_TIMESTAMP_EMPTY));
}

static bool
isMonthOnlyStamp (SPWAW_TIMESTAMP *stamp)
{
	return (stamp && (*stamp & SPWAW_MONTHONLY_TIMESTAMP_BITS));
}


static bool
isValidStamp (SPWAW_TIMESTAMP *stamp)
{
	if (isEmptyStamp(stamp)) return (true);
	return (stamp && ((*stamp & SPWAW_MONTHONLY_TIMESTAMP_MASK) >= 0));
}

static bool
isValidPeriod (SPWAW_PERIOD *period)
{
	return (period && isValidStamp(&(period->stamp)));
}

SPWAWLIB_API int
SPWAW_bdate_cmp (SPWAW_BATTLE_DATE *a, SPWAW_BATTLE_DATE *b)
{
	if (!a || !b) {
		return ((!a&&!b)?0:((a&&!b)?-1:1));
	}

	SPWAW_TIMESTAMP sa, sb;
	SPWAW_date2stamp (&(a->date), &sa); sa = SPWAW_PURE_TIMESTAMP(sa);
	SPWAW_date2stamp (&(b->date), &sb); sb = SPWAW_PURE_TIMESTAMP(sb);

	return ((a->btlidx==b->btlidx)?((sa==sb)?0:((sa<sb)?-1:1)):((a->btlidx<b->btlidx)?-1:1));
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_bdate2str (SPWAW_BATTLE_DATE *bdate, char *buf, int len, bool tolog)
{
	char	bbdate[64];

	CNULLARG (bdate);

	if (!buf || !len) return (SPWERR_OK);
	memset (buf, 0, len);

	SPWAW_date2str (&(bdate->date), bbdate, sizeof (bbdate), tolog);
	if (tolog) {
		snprintf (buf, len - 1, "%s<B>%05u", bbdate, bdate->btlidx);
	} else {
		if (bdate->btlidx == SPWAW_NOBTLIDX) {
			snprintf (buf, len - 1, "%s", bbdate);
		} else {
			snprintf (buf, len - 1, "%s #%u", bbdate, bdate->btlidx+1);
		}
	}

	return (SPWERR_OK);
}

SPWAWLIB_API int
SPWAW_tdate_cmp (SPWAW_TURN_DATE *a, SPWAW_TURN_DATE *b)
{
	if (!a || !b) {
		return ((!a&&!b)?0:((a&&!b)?-1:1));
	}

	SPWAW_TIMESTAMP sa, sb;
	SPWAW_date2stamp (&(a->date), &sa); sa = SPWAW_PURE_TIMESTAMP(sa);
	SPWAW_date2stamp (&(b->date), &sb); sb = SPWAW_PURE_TIMESTAMP(sb);

	if (sa == sb) {
		return ((a->turn==b->turn)?0:((a->turn<b->turn)?-1:1));
	} else {
		return ((sa==sb)?0:((sa<sb)?-1:1));
	}
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_tdate2str (SPWAW_TURN_DATE *tdate, char *buf, int len, bool tolog)
{
	char	btdate[64];

	CNULLARG (tdate);

	if (!buf || !len) return (SPWERR_OK);
	memset (buf, 0, len);

	SPWAW_date2str (&(tdate->date), btdate, sizeof (btdate), tolog);
	if (tolog) {
		snprintf (buf, len - 1, "%s<T>%05u", btdate, tdate->turn);
	} else {
		if (SPWAW_isMonthOnlyDate(&(tdate->date))) {
			snprintf (buf, len - 1, "%s, turn %u", btdate, tdate->turn);
		} else {
			snprintf (buf, len - 1, "%s", btdate);
		}
	}

	return (SPWERR_OK);

}

SPWAWLIB_API const char *
SPWAW_rank2str (SPWAW_RANK id)
{
	if ((id < SPWAW_RSTARTCODE) || (id > SPWAW_RLASTCODE)) return ("???");
	return ((const char *)rank2str (id));
}

SPWAWLIB_API const char *
SPWAW_terrain2str (SPWAW_TERRAIN id)
{
	if ((id < SPWAW_TSTARTCODE) || (id > SPWAW_TLASTCODE)) return ("???");
	return ((const char *)terrain2str (id));
}

SPWAWLIB_API const char *
SPWAW_weather2str (SPWAW_TERRAIN tid, SPWAW_WEATHER wid)
{
	if ((tid < SPWAW_TSTARTCODE) || (tid > SPWAW_TLASTCODE)) return ("???");
	if ((wid < SPWAW_WSTARTCODE) || (wid > SPWAW_WLASTCODE)) return ("???");
	return ((const char *)weather2str (tid, wid));
}

SPWAWLIB_API const char *
SPWAW_mission2str (SPWAW_MISSION id)
{
	if ((id < SPWAW_MSTARTCODE) || (id > SPWAW_MLASTCODE)) return ("???");
	return ((const char *)mission2str (id));
}

SPWAWLIB_API const char *
SPWAW_fstatus2str (SPWAW_FSTATUS id)
{
	if ((id < SPWAW_FSTARTCODE) || (id > SPWAW_FLASTCODE)) return ("???");
	return ((const char *)fstatus2str (id));
}

SPWAWLIB_API const char *
SPWAW_ustatus2str (SPWAW_USTATUS id)
{
	if ((id < SPWAW_USTARTCODE) || (id > SPWAW_ULASTCODE)) return ("???");
	return ((const char *)ustatus2str (id));
}

SPWAWLIB_API const char *
SPWAW_exp2str (SPWAW_EXP id)
{
	if ((id < SPWAW_ESTARTCODE) || (id > SPWAW_ELASTCODE)) return ("???");
	return ((const char *)exp2str (id));
}

SPWAWLIB_API const char *
SPWAW_aband2str (SPWAW_ABAND id)
{
	if ((id < SPWAW_ASTARTCODE) || (id > SPWAW_ALASTCODE)) return ("???");
	return ((const char *)aband2str (id));
}

SPWAWLIB_API const char *
SPWAW_cstatus2str (SPWAW_CSTATUS id)
{
	if ((id < SPWAW_CSTATUSSTARTCODE) || (id > SPWAW_CSTATUSLASTCODE)) return ("???");
	return ((const char *)cstatus2str (id));
}

SPWAWLIB_API const char *
SPWAW_entr2str (SPWAW_ENTR id)
{
	if ((id < SPWAW_ENTRSTARTCODE) || (id > SPWAW_ENTRLASTCODE)) return ("???");
	return ((const char *)entr2str (id));
}

SPWAWLIB_API const char *
SPWAW_btstatus2str (SPWAW_BTSTATUS id)
{
	if ((id < SPWAW_BTSTARTCODE) || (id > SPWAW_BTLASTCODE)) return ("???");
	return ((const char *)btstatus2str (id));
}

SPWAWLIB_API bool
SPWAW_tfs2water (SPWAW_TFS tfs)
{
	return (tfs.tfs.stream || tfs.tfs.water || tfs.tfs.water_deep);
}

SPWAWLIB_API bool
SPWAW_tfs2bridge (SPWAW_TFS tfs)
{
	return (tfs.tfs.bridge_wood || tfs.tfs.bridge_stone);
}

SPWAWLIB_API bool
SPWAW_tfs2road (SPWAW_TFS tfs)
{
	return (tfs.tfs.road1 || tfs.tfs.road2 || tfs.tfs.railroad || tfs.tfs.tramline);
}

SPWAWLIB_API const char *
SPWAW_vhstatus2str (SPWAW_VHSTATUS id)
{
	if ((id < SPWAW_VHSTARTCODE) || (id > SPWAW_VHLASTCODE)) return ("???");
	return ((const char *)vhstatus2str (id));
}

SPWAWLIB_API const char *
SPWAW_bresult2str (SPWAW_BRESULT result)
{
	if ((result < SPWAW_BRSTARTCODE) || (result > SPWAW_BRLASTCODE)) return ("???");
	return ((const char *)bresult2str (result));
}

SPWAWLIB_API const char *
SPWAW_battletype2str (SPWAW_BATTLE_TYPE type)
{
	const char	*p;

	switch (type) {
		case SPWAW_UNKNOWN_BATTLE:
			p = "unknown";
			break;
		case SPWAW_CAMPAIGN_BATTLE:
			p = "campaign";
			break;
		case SPWAW_STDALONE_BATTLE:
			p = "standalone";
			break;
		case SPWAW_MEGACAM_BATTLE:
			p = "Mega Campaign";
			break;
		default:
			p = "???";
			break;
	}
	return (p);
}

SPWAWLIB_API const char *
SPWAW_dossiertype2str (SPWAW_DOSSIER_TYPE type)
{
	const char	*p;

	switch (type) {
		case SPWAW_EMPTY_DOSSIER:
			p = "empty";
			break;
		case SPWAW_CAMPAIGN_DOSSIER:
			p = "campaign";
			break;
		case SPWAW_STDALONE_DOSSIER:
			p = "standalone";
			break;
		case SPWAW_MEGACAM_DOSSIER:
			p = "megacampaign";
			break;
		default:
			p = "???";
			break;
	}
	return (p);
}

SPWAWLIB_API const char *
SPWAW_unittype2str (SPWAW_UNIT_TYPE type)
{
	const char	*p;

	switch (type) {
		case SPWAW_UNIT_TYPE_UNKNOWN:
			p = "unknown";
			break;
		case SPWAW_UNIT_TYPE_UNIT:
			p = "UNIT";
			break;
		case SPWAW_UNIT_TYPE_CREW:
			p = "CREW";
			break;
		case SPWAW_UNIT_TYPE_SPAU:
			p = "SPAU";
			break;
		default:
			p = "???";
			break;
	}
	return (p);
}

/* Convenience macro */
#define	STREQ(a_,b_)	strnicmp(a_, b_, strlen(b_)) == 0

SPWAWLIB_API const char *
SPWAW_gametype2str (SPWAW_GAME_TYPE gametype)
{
	const char	*p;

	switch (gametype) {
		case SPWAW_GAME_TYPE_UNKNOWN:
			p = "unknown";
			break;
		case SPWAW_GAME_TYPE_SPWAW:
			p = "SP:WaW";
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			p = "winSPWW2";
			break;
		default:
			p = "???";
			break;
	}
	return (p);
}

SPWAWLIB_API SPWAW_GAME_TYPE
SPWAW_str2gametype (char * gametype)
{
	if (STREQ(gametype, "SP:WaW")) {
		return (SPWAW_GAME_TYPE_SPWAW);
	} else if (STREQ (gametype, "SPWAW")) {
		return (SPWAW_GAME_TYPE_SPWAW);
	} else if (STREQ (gametype, "winSPWW2")) {
		return (SPWAW_GAME_TYPE_WINSPWW2);
	} else {
		return (SPWAW_GAME_TYPE_UNKNOWN);
	}
}

SPWAWLIB_API const char *
SPWAW_savetype2str (SPWAW_SAVE_TYPE savetype)
{
	const char	*p;

	switch (savetype) {
		case SPWAW_SAVE_TYPE_UNKNOWN:
			p = "unknown";
			break;
		case SPWAW_SAVE_TYPE_REGULAR:
			p = "regular";
			break;
		case SPWAW_SAVE_TYPE_MEGACAM:
			p = "SP:WaW Mega Campaign";
			break;
		default:
			p = "???";
			break;
	}
	return (p);
}

SPWAWLIB_API SPWAW_SAVE_TYPE
SPWAW_str2savetype (char * savetype)
{
	if (STREQ(savetype, "regular")) {
		return (SPWAW_SAVE_TYPE_REGULAR);
	} else if (STREQ (savetype, "SP:WaW - General's Edition Campaign")) {
		return (SPWAW_SAVE_TYPE_MEGACAM);
	} else {
		return (SPWAW_SAVE_TYPE_UNKNOWN);
	}
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_savegame_descriptor_init (SPWAW_SAVEGAME_DESCRIPTOR &sgd, SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype, const char *path, unsigned int id)
{
	SPWAW_ERROR	rc;

	memset (&sgd, 0, sizeof (sgd));
	sgd.gametype	= gametype;
	sgd.savetype	= savetype;
	sgd.path	= safe_strdup ((char *)path);	COOMGOTO (sgd.path, "savegame descriptor path", handle_error);
	sgd.numeric_id	= true;
	sgd.id.number	= id;

	return (SPWERR_OK);

handle_error:
	SPWAW_savegame_descriptor_clear (sgd);
	return (rc);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_savegame_descriptor_init (SPWAW_SAVEGAME_DESCRIPTOR &sgd, SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype, const char *path, const char * id)
{
	SPWAW_ERROR	rc;

	memset (&sgd, 0, sizeof (sgd));
	sgd.gametype	= gametype;
	sgd.savetype	= savetype;
	sgd.path	= safe_strdup ((char *)path);	COOMGOTO (sgd.path, "savegame descriptor path", handle_error);

	sgd.numeric_id	= false;
	sgd.id.name	= safe_strdup ((char *)id);	COOMGOTO (sgd.id.name, "savegame descriptor ID name", handle_error);

	return (SPWERR_OK);

handle_error:
	SPWAW_savegame_descriptor_clear (sgd);
	return (rc);
}

SPWAWLIB_API void
SPWAW_savegame_descriptor_clear (SPWAW_SAVEGAME_DESCRIPTOR &sgd)
{
	if (sgd.path) safe_free (sgd.path);
	if (!sgd.numeric_id) safe_free (sgd.id.name);
	memset (&sgd, 0, sizeof (sgd));
}
