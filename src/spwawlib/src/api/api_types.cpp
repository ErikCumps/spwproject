/** \file
 * The SPWaW Library - API types implementation.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_types.h>
#include "common/internal.h"
#include "snapshot/translate.h"

#define	MD	31

static unsigned int dec_month_days[12] = { 30, 27, 30, 29, 30, 29, 30, 30, 29, 30, 29, 30 };

static void	eval_period	(SPWAW_PERIOD *period);
static bool	isValidDate	(SPWAW_DATE *date);
static bool	isValidStamp	(SPWAW_TIMESTAMP *stamp);
static bool	isValidPeriod	(SPWAW_PERIOD *period);


SPWAWLIB_API SPWAW_ERROR
SPWAW_date2stamp (SPWAW_DATE *date, SPWAW_TIMESTAMP *stamp)
{
	SYSTEMTIME	st;
	FILETIME	ft;
	ULARGE_INTEGER	mins;

	CNULLARG (date); CNULLARG (stamp);
	*stamp = -1;

	if (!isValidDate (date)) RWE (SPWERR_BADDATE, "invalid SPWAW date");

	memset (&st, 0, sizeof (st));
	st.wYear   = date->year;
	st.wMonth  = date->month;
	st.wDay	   = date->day;
	st.wHour   = date->hour;
	st.wMinute = date->minute;
	if (!SystemTimeToFileTime (&st, &ft)) RWE (SPWERR_BADDATE, "invalid SPWAW date");

	mins.LowPart  = ft.dwLowDateTime;
	mins.HighPart = ft.dwHighDateTime;
	mins.QuadPart /= (10 * 1000 * 1000 * 60); /* convert units of 100 ns to 1 minute */

	*stamp = mins.QuadPart;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_stamp2date (SPWAW_TIMESTAMP *stamp, SPWAW_DATE *date)
{
	ULARGE_INTEGER	ns;
	FILETIME	ft;
	SYSTEMTIME	st;

	CNULLARG (stamp); CNULLARG (date);
	fill_ptr (date, 0xFF);

	if (!isValidStamp (stamp)) RWE (SPWERR_BADSTAMP, "invalid SPWAW timestamp");

	ns.QuadPart = *stamp;
	ns.QuadPart *= (10 * 1000 * 1000 * 60); /* convert units of 100 ns to 1 minute */

	ft.dwHighDateTime = ns.HighPart;
	ft.dwLowDateTime  = ns.LowPart;
	if (!FileTimeToSystemTime (&ft, &st)) RWE (SPWERR_BADSTAMP, "invalid SPWAW timestamp");

	date->year   = st.wYear;
	date->month  = (char)st.wMonth;
	date->day    = (char)st.wDay;
	date->hour   = (char)st.wHour;
	date->minute = (char)st.wMinute;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_date2str (SPWAW_DATE *date, char *buf, int len)
{
	CNULLARG (date);

	if (!buf || !len) return (SPWERR_OK);
	memset (buf, 0, len);

	if (!isValidDate (date)) RWE (SPWERR_BADSTAMP, "invalid SPWAW date");

	if (date->minute >= 0) {
		snprintf (buf, len - 1, "%04d/%02d/%02d %02d:%02d", date->year, date->month, date->day, date->hour, date->minute);
	} else if (date->hour >= 0) {
		snprintf (buf, len - 1, "%04d/%02d/%02d %02d:00", date->year, date->month, date->day, date->hour);
	} else if (date->day >= 0) {
		snprintf (buf, len - 1, "%04d/%02d/%02d", date->year, date->month, date->day);
	} else if (date->month >= 0) {
		snprintf (buf, len - 1, "%04d/%02d", date->year, date->month);
	} else {
		snprintf (buf, len - 1, "%04d", date->year);
	}

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_date2str (SPWAW_DATE *date, char **str)
{
	SPWAW_ERROR	rc;
	char		buf[64];

	CNULLARG (date); CNULLARG (str);
	*str = NULL;

	rc = SPWAW_date2str (date, buf, sizeof (buf));
	ROE ("SPWAW_date2str(core)");

	*str = strdup (buf);
	if (!*str) RWE (SPWERR_OUTOFMEM, "strdup() failed");

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_period2stamp (SPWAW_PERIOD *period, SPWAW_TIMESTAMP *stamp)
{
	CNULLARG (period); CNULLARG (stamp);
	*stamp = -1;

	if (!isValidPeriod (period)) RWE (SPWERR_BADSTAMP, "invalid SPWAW period");
	*stamp = period->stamp;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_stamp2period (SPWAW_TIMESTAMP *stamp, SPWAW_PERIOD *period)
{
	CNULLARG (period); CNULLARG (stamp);
	period->stamp = -1; eval_period (period);

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
		first = false;
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
	delta->stamp = -1; eval_period (delta);

	if (!isValidDate (base)) RWE (SPWERR_BADSTAMP, "invalid SPWAW base date");
	if (!isValidDate (item)) RWE (SPWERR_BADSTAMP, "invalid SPWAW item date");

	rc = SPWAW_date2stamp (base, &sb); ROE ("SPWAW_date2stamp(base date)");
	rc = SPWAW_date2stamp (item, &si); ROE ("SPWAW_date2stamp(item date)");

	delta->stamp = si - sb; eval_period (delta);

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_date_add (SPWAW_DATE *base, SPWAW_PERIOD *add, SPWAW_DATE *sum)
{
	SPWAW_ERROR	rc;
	SPWAW_TIMESTAMP	stamp;

	CNULLARG (base); CNULLARG (add); CNULLARG (sum);
	fill_ptr (sum, 0xFF);

	if (!isValidDate (base)) RWE (SPWERR_BADSTAMP, "invalid SPWAW date");
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
	delta->stamp = -1; eval_period (delta);

	if (!isValidPeriod (base)) RWE (SPWERR_BADSTAMP, "invalid SPWAW base period");
	if (!isValidPeriod (item)) RWE (SPWERR_BADSTAMP, "invalid SPWAW item period");

	delta->stamp = item->stamp - base->stamp; eval_period (delta);

	return (SPWERR_OK);
}


SPWAWLIB_API SPWAW_ERROR
SPWAW_period_add (SPWAW_PERIOD *base, SPWAW_PERIOD *add, SPWAW_PERIOD *sum)
{
	CNULLARG (base); CNULLARG (add); CNULLARG (sum);
	sum->stamp = -1; eval_period (sum);

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
	if ((period->stamp = stamp) == -1) return;

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
isValidDate (SPWAW_DATE *date)
{
	bool	isleap = false;
	int	feb_days = 28;

	if (!date) return (false);

	if ((date->year == 1900) && (date->month == 0) && (date->day == 0) && (date->hour == 0) && (date->minute == 0)) return (true);

	isleap = (((date->year % 4) == 0) && (((date->year % 100) != 0) || ((date->year % 400) == 0)));
	if (isleap) feb_days = 29;

	if ((unsigned int)(date->month - 1) > 11) return (false);
	if ((unsigned int)(date->day - 1) > dec_month_days[date->month-1]) return (false);
	if ((date->month == 2) && (date->day > feb_days)) return (false);
	if ((unsigned int)date->hour > 23) return (false);
	if ((unsigned int)date->minute > 59) return (false);
	return (true);
}

static bool
isValidStamp (SPWAW_TIMESTAMP *stamp)
{
	return (stamp && (*stamp >= 0));
}

static bool
isValidPeriod (SPWAW_PERIOD *period)
{
	return (period && (period->stamp >= 0));
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

SPWAWLIB_API const char *
SPWAW_vhstatus2str (SPWAW_VHSTATUS id)
{
	if ((id < SPWAW_VHSTARTCODE) || (id > SPWAW_VHLASTCODE)) return ("???");
	return ((const char *)vhstatus2str (id));
}