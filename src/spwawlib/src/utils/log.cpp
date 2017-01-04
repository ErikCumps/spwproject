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

#define        SEPARATOR       "----------------------------------------------------------------------"

void
log_init (char *log, bool append)
{
	log_shutdown ();
	logfile = fopen (log, append?"a":"w");

	if (logfile) fprintf (logfile, "%s\n", SEPARATOR);

	if (getenv ("SPWAWLIB_DISABLE_LOGTIME") != NULL) logtime = false;
}

void
log (char *fmt, ...)
{
	LARGE_INTEGER	pc_now, pc_dlt;
	SYSTEMTIME	now;
	char		hdr[64];
	char		buf[4096];
	va_list		AP;
	char		*p, *q;
	static bool	nots = false;

	if (!logfile) return;
	
	if (logtime) {
		memset (hdr, 0, sizeof (hdr));

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
	memset (buf, 0, sizeof(buf));
	vsnprintf (buf, sizeof(buf) - 1, fmt, AP);
	va_end (AP);

	if (logtime) {
		p = buf;

		while (p) {
			q = strchr (p, '\n');
			if (q) *q++ = '\0';
			if (nots) {
				fprintf (logfile, "%s%s", p, q ? "\n" : "");
			} else {
				fprintf (logfile, "%s%s%s", hdr, p, q ? "\n" : "");
			}
			nots = !q;
			if (q && *q == '\0') q = NULL;
			p = q;
		}
	} else {
		fprintf (logfile, "%s", buf);
	}

	fflush (logfile);
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
log_shutdown (void)
{ }

#endif	/* !_DEBUG */
