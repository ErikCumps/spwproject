/** \file
 * The SPWaW war cabinet - debugging support - console handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"
#include "dbg_private.h"

/* Set to 1 to enable automatic log flushing */
#define	FLUSHING	1

#define	TITLE	"application debug console"
#define	LOGFILE	"debug_console.log"
#define	LIHV	INVALID_HANDLE_VALUE

#define	CONSW	132
#define	CONSH	50
#define	CONBW	CONSW
#define	CONBH	512

static void	statereport	(SL_STDBG_INFO_LEVEL level);

/*! Module name */
static const char	*MODULE = "DBG_CONS";

static SL_LOCK		*lock = NULL;
static HANDLE		logH = LIHV;
static FILE *		logF = NULL;
static LARGE_INTEGER	lastPF = {0, 0};
static LARGE_INTEGER	lastPC = {0, 0};

static BOOL WINAPI
DBG_CONS_handler (DWORD CtrlType)
{
	fflush (NULL);
	switch (CtrlType) {
		case CTRL_C_EVENT:
			DBG_log ("--- Windows console: CTRL-C event ---");
			break;
		case CTRL_CLOSE_EVENT:
			DBG_log ("--- Windows console: close event ---");
			break;
		case CTRL_BREAK_EVENT:
			DBG_log ("--- Windows console: CTRL-BREAK event ---");
			break;
		case CTRL_LOGOFF_EVENT:
			DBG_log ("--- Windows console: user logoff event ---");
			break;
		case CTRL_SHUTDOWN_EVENT:
			DBG_log ("--- Windows console: system shutdown event ---");
			break;
		default:
			DBG_log ("--- Windows console: unknown windows console event (0x%8.8lx) ---", CtrlType);
			break;
	}
	fflush (NULL);

	return (TRUE);
}


static void
DBG_CONS_fix_console (HANDLE ch)
{
	COORD		c;
	SMALL_RECT	r;

	SetConsoleTitleA (TITLE);

	c.X = CONBW;
	c.Y = CONBH;
	SetConsoleScreenBufferSize (ch, c);

	r.Left   = 0;
	r.Top    = 0;
	r.Right  = CONSW - 1;
	r.Bottom = CONSH - 1;
	SetConsoleWindowInfo (ch, TRUE, &r);
}

void
DBG_CONS_init (void)
{
	if (lock) return;

	lock = SL_LOCK_new ();
	if (!AllocConsole ())
		logH = LIHV;
	else {
		logH = GetStdHandle (STD_OUTPUT_HANDLE);
		DBG_CONS_fix_console (logH);
	}
	logF = fopen (LOGFILE, "w");

	if (!SetConsoleCtrlHandler (DBG_CONS_handler, TRUE)) DEV_PANIC0 ("failed to install windows console handler");

	SL_STDBG_add (statereport, MODULE);
}

void
DBG_CONS_shutdown (void)
{
	if (!lock) return;

	SL_STDBG_delete (statereport);

	DBG_CONS_flush ();

	if (logF) {
		fclose (logF);
		logF = NULL;
	}
	if (logH != LIHV) {
		CloseHandle (logH);
		logH = LIHV;
	}
	SL_LOCK_free (&lock);
}

void
DBG_CONS_log (SL_BOOL cons, SL_BOOL file, char *fmt, ...)
{
	va_list	AP;

	if (logH == LIHV || !lock) return;

	va_start (AP, fmt);
	DBG_CONS_vlog (cons, file, fmt, AP);
	va_end (AP);
}

void
DBG_CONS_vlog (SL_BOOL cons, SL_BOOL file, char *fmt, va_list AP)
{
	LARGE_INTEGER	pc_now, pc_dlt;
	SYSTEMTIME	now;
	char		hdr[64];
	char		buf[512];
	char		buffer[512+64];
	DWORD		idx, bw;

	if (logH == LIHV || !lock) return;

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

	memset (hdr, 0, sizeof (hdr));
	snprintf (hdr, sizeof (hdr) - 1, "%16I64d %02.2u:%02.2u:%02.2u.%03.3u [0x%8.8x] ",
		pc_dlt.QuadPart, now.wHour, now.wMinute, now.wSecond, now.wMilliseconds, GetCurrentThreadId());

	memset (buf, 0, sizeof (buf));
	vsnprintf (buf, sizeof (buf) - 1, fmt, AP);

	memset (buffer, 0, sizeof (buffer));
	snprintf (buffer, sizeof (buffer) - 2, "%s%s", hdr, buf);

	idx = strlen (buffer);
	if (!idx) {
		buffer[0]='\n';
	} else {
		if (buffer[idx-1] != '\n') buffer[idx] = '\n';
	}

	SL_LOCK_take (lock);
	if (cons && (logH != LIHV)) WriteFile (logH, buffer, strlen (buffer), &bw, NULL);
	if (file && (logF != NULL)) fwrite (buffer, 1, strlen (buffer), logF);
#if	(FLUSHING)
	DBG_CONS_flush ();
#endif	/* FLUSH */
	SL_LOCK_drop (lock);
}

void
DBG_CONS_logbuf (SL_BOOL cons, SL_BOOL file, char *buf, int len)
{
	DWORD	bw;

	if (!buf || !len) return;

	if (logH == LIHV || !lock) return;

	SL_LOCK_take (lock);
	if (cons && (logH != LIHV)) WriteFile (logH, buf, len, &bw, NULL);
	if (file && (logF != NULL)) fwrite (buf, 1, len, logF);
#if	(FLUSHING)
	DBG_CONS_flush ();
#endif	/* FLUSH */
	SL_LOCK_drop (lock);
}

void
DBG_CONS_flush (void)
{
	if (!lock) return;

	if (logH != LIHV) FlushFileBuffers (logH);
	if (logF) fflush (logF);
}

static void
statereport (SL_STDBG_INFO_LEVEL level)
{
	SAYSTATE0 ("### Debug console state:\n");

	/* basic information */
	if (level >= SL_STDBG_LEVEL_BAS) {
		SAYSTATE1 ("\tlogH = 0x%8.8x\n", logH);
		SAYSTATE1 ("\tlogF = 0x%8.8x\n", logF);
	}

	/* extended information */
	if (level >= SL_STDBG_LEVEL_EXT) {
		SAYSTATE1 ("\tlock = 0x%8.8x\n", lock);
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
	}

	SAYSTATE0 ("\n");
}

