/** \file
 * Slay's Library - internal debugging support.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <SL/system.h>
#include <SL/lock.h>
#include <SL/util.h>

#define	LOGFILE	"SL_internal_debug.log"

static SL_LOCK	*lock = NULL;
static FILE	*logf = NULL;

#if	_DEBUG

void
IDBG_init (void)
{
	FILE	*file = NULL;

	if (lock) return;

	lock = SL_LOCK_new ();
	if (!lock) return;

	file = fopen (LOGFILE, "w");
	if (file) {
		fprintf (file, "*** SL internal debug logging started ***\n");
		fflush (file);
	}
	logf = file;
}

void
IDBG_shutdown (void)
{
	FILE	*file = NULL;

	if (!lock) return;

	if (logf) { file = logf; logf = NULL; }
	if (file) {
		fprintf (file, "*** SL internal debug logging stopped ***\n");
		fflush (file); fclose (file);
	}
	SL_LOCK_free (&lock);
	lock = NULL;
}

void
IDBG_log (const char *file, unsigned long line, const char *func, const char *fmt, ...)
{
	time_t	now;
	char	stamp[64];
	char	buffer[512];
	va_list	AP;

	UNREFERENCED (file); UNREFERENCED (line);

	if (!lock || !logf) return;

	time (&now);
	SL_UTIL_timestamp (&now, stamp, sizeof (stamp));
	memset (buffer, 0, sizeof (buffer));

	/* make sure line contains trailing linefeed */
	snprintf (buffer, sizeof (buffer) - 1, "%s [0x%8.8lx] %s: %s%s", stamp, SL_system_threadid(), func, fmt,
		(fmt[strlen(fmt)-1] == '\n') ? "" : "\n");

	va_start (AP, fmt);

	SL_LOCK_take (lock);
	vfprintf (logf, buffer, AP);
	fflush (logf);
	SL_LOCK_drop (lock);

	va_end (AP);
}

#endif	/* _DEBUG */
