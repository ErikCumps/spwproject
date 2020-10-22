/** \file
 * The SPWaW war cabinet - utility functions.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"
#include "util.h"

void
UTIL_fmt_longspan (SPWAW_PERIOD *span, char *buf, int len)
{
	UtilStrbuf	str(buf, len, true, true);

	if (!span || !buf || !len) return;
	UTIL_fmt_longspan (span, &str);
}

void
UTIL_fmt_longspan (SPWAW_PERIOD *span, UtilStrbuf *buf)
{
	bool	f = true;
	bool	done = false;

	if (!span || !buf) return;

	if (span->years) {
		buf->printf ("%s%u year%s", f ? "" : ", ", span->years, (span->years == 1) ? "" : "s");
		f = false;
		done = true;
	}
	if (span->months) {
		buf->printf ("%s%u month%s", f ? "" : ", ", span->months, (span->months == 1) ? "" : "s");
		f = false;
		done = true;
	}
	if (span->weeks) {
		buf->printf ("%s%u week%s", f ? "" : ", ", span->weeks, (span->weeks == 1) ? "" : "s");
		done = true;
	}

	if (!done) UTIL_fmt_shortspan (span, buf);
}

void
UTIL_fmt_shortspan (SPWAW_PERIOD *span, char *buf, int len)
{
	UtilStrbuf	str(buf, len, true, true);

	if (!span || !buf || !len) return;
	UTIL_fmt_shortspan (span, &str);
}

void
UTIL_fmt_shortspan (SPWAW_PERIOD *span, UtilStrbuf *buf)
{
	bool	f = true;
	bool	done = false;

	if (!span || !buf) return;

	if (span->days) {
		buf->printf ("%s%u day%s", f ? "" : ", ", span->days, (span->days == 1) ? "" : "s");
		f = false;
		done = true;
	}
	if (span->hours) {
		buf->printf ("%s%u hour%s", f ? "" : ", ", span->hours, (span->hours == 1) ? "" : "s");
		f = false;
		done = true;
	}
	if (span->minutes) {
		buf->printf ("%s%u minute%s", f ? "" : ", ", span->minutes, (span->minutes == 1) ? "" : "s");
		done = true;
	}
}

void
UTIL_fmt_fullspan (SPWAW_PERIOD *span, char *buf, int len)
{
	UtilStrbuf	str(buf, len, true, true);

	if (!span || !buf || !len) return;
	UTIL_fmt_fullspan (span, &str);
}

void
UTIL_fmt_fullspan (SPWAW_PERIOD *span, UtilStrbuf *buf)
{
	bool	f = true;
	bool	done = false;

	if (!span || !buf) return;

	if (span->years) {
		buf->printf ("%s%u year%s", f ? "" : ", ", span->years, (span->years == 1) ? "" : "s");
		f = false;
		done = true;
	}
	if (span->months) {
		buf->printf ("%s%u month%s", f ? "" : ", ", span->months, (span->months == 1) ? "" : "s");
		f = false;
		done = true;
	}
	if (span->days) {
		buf->printf ("%s%u day%s", f ? "" : ", ", span->days, (span->days == 1) ? "" : "s");
		f = false;
		done = true;
	}
	if (span->hours) {
		buf->printf ("%s%u hour%s", f ? "" : ", ", span->hours, (span->hours == 1) ? "" : "s");
		f = false;
		done = true;
	}
	if (span->minutes) {
		buf->printf ("%s%u minute%s", f ? "" : ", ", span->minutes, (span->minutes == 1) ? "" : "s");
		done = true;
	}
}
