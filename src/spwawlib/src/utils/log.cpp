/** \file
 * The SPWaW Library - utility code: logging.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "common/internal.h"
#include <spwawlib_api.h>

#ifdef	_DEBUG

static FILE		*logfile = NULL;
static bool		logtime = true;

static LARGE_INTEGER	lastPF = {0, 0};
static LARGE_INTEGER	lastPC = {0, 0};

void
log_init (char *log, bool append)
{
	log_shutdown ();
	logfile = fopen (log, append?"a":"w");

	if (getenv ("SPWAWLIB_DISABLE_LOGTIME") != NULL) logtime = false;
}

static void
log_core (char *hdr, char *fmt, va_list AP)
{
	char	buf[1024];

	memset (buf, 0, sizeof (buf));
	vsnprintf (buf, sizeof (buf) - 1, fmt, AP);

	if (hdr)
		fprintf (logfile, "%s%s", hdr, buf);
	else
		fprintf (logfile, "%s", buf);

	fflush (logfile);
}

void
log (char *fmt, ...)
{
	LARGE_INTEGER	pc_now, pc_dlt;
	SYSTEMTIME	now;
	char		hdr[64];
	va_list		AP;

	if (!logfile) return;

	memset (hdr, 0, sizeof (hdr));
	if (logtime) {
		QueryPerformanceCounter (&pc_now);
		if (!lastPF.QuadPart) {
			QueryPerformanceFrequency (&lastPF);
			lastPF.QuadPart /= 1000000;
		}
		pc_dlt.QuadPart = !lastPC.QuadPart ? 0 : pc_now.QuadPart - lastPC.QuadPart;
		pc_dlt.QuadPart /= lastPF.QuadPart;
		lastPC.QuadPart = pc_now.QuadPart;

		memset (&now, 0, sizeof (now));
		GetSystemTime (&now);

		snprintf (hdr, sizeof (hdr) - 1, "%10I64d %02.2u:%02.2u:%02.2u.%03.3u ",
			pc_dlt.QuadPart, now.wHour, now.wMinute, now.wSecond, now.wMilliseconds);
	}

	va_start (AP, fmt);
	log_core (hdr, fmt, AP);
	va_end (AP);
}

void
log_nots (char *fmt, ...)
{
	va_list		AP;

	if (!logfile) return;

	va_start (AP, fmt);
	log_core (NULL, fmt, AP);
	va_end (AP);
}

void
log_shutdown (void)
{
	if (logfile) {
		fflush (logfile);
		fclose (logfile);
		logfile = NULL;
	}
}

#else	/* !_DEBUG */

void
log_init (char *log, bool append)
{ UNREFERENCED_PARAMETER(log); UNREFERENCED_PARAMETER(append); }

void
log (char *fmt, ...)
{ UNREFERENCED_PARAMETER(fmt); }

void
log_nots (char *fmt, ...)
{ UNREFERENCED_PARAMETER(fmt); }

void
log_shutdown (void)
{ }

#endif	/* !_DEBUG */