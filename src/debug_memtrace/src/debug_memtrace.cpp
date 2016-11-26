/** \file
 * debug_memtrace: tracing implementation.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"

#ifdef	DEBUG_MEMTRACE
#undef	DEBUG_MEMTRACE
#endif
#include "debug_memtrace.h"
#include <new>

/* Set to 1 to only flush log,
 * do not clean up empty log or reinitialize critical section during dll process attach/detach
 */
#define	FLUSH_ONLY	0

/* Experimental:
 * set to 1 to also log the time spent executing the heap management operation
 */
#define	LOG_TIMING	0

/* Experimental:
 * set to 1 to  also log the name of the heap management operation
 */
#define	LOG_CALLER	0

static const char *logfile_default = "debug_memtrace.log";

static const char *logfile = logfile_default;

static bool		initialised = false;
static FILE		*log = NULL;
static CRITICAL_SECTION	CS;

static LARGE_INTEGER	lastPF = {0, 0};

BOOL APIENTRY
DllMain (HANDLE /*hModule*/, DWORD  ul_reason_for_call, LPVOID /*lpReserved*/)
{
	long	logp;

	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:

#if	FLUSH_ONLY
			if (initialised) {
				if (log) {
					fflush (log);
				} else {
					log = fopen (logfile, "w");
				}
			} else {
				log = fopen (logfile, "w");
				InitializeCriticalSection (&CS);
				initialised = true;
			}
#else	/* !FLUSH_ONLY */
			if (initialised) {
				if (log) {
					// flush log and obtain current file position
					fflush (log); logp = ftell (log);
					// close logfile and clean up if no data was written to it
					fclose (log); if (logp == 0) unlink (logfile);
					log = NULL;
				}
				DeleteCriticalSection (&CS);
				initialised = false;
			}
			log = fopen (logfile, "w");
			InitializeCriticalSection (&CS);
			initialised = true;
#endif	/* !FLUSH_ONLY */
			break;
		case DLL_PROCESS_DETACH:
#if	FLUSH_ONLY
			if (initialised && log) fflush (log);
#else	/* !FLUSH_ONLY */
			if (initialised) {
				if (log) {
					// flush log and obtain current file position
					fflush (log); logp = ftell (log);
					// close logfile and clean up if no data was written to it
					fclose (log); if (logp == 0) unlink (logfile);
					log = NULL;
				}
				DeleteCriticalSection (&CS);
				initialised = false;
			}
#endif	/* !FLUSH_ONLY */
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
	}

	return (TRUE);
}

void
debug_memtrace_init (const char *logf)
{
	if (initialised) {
		if (log) {
			// flush log and obtain current file position
			fflush (log); long logp = ftell (log);
			// close logfile and clean up if no data was written to it
			fclose (log); if (logp == 0) unlink (logfile);
			log = NULL;
		}
		DeleteCriticalSection (&CS);
		initialised = false;
	}
	logfile = logf;
	log = fopen (logfile, "w");
	InitializeCriticalSection (&CS);
	initialised = true;
}

void
debug_memtrace_mark (const char *mark)
{
	if (log) {
		char	buffer[512];

		memset (buffer, 0, sizeof (buffer));
		_snprintf (buffer, sizeof (buffer) - 1, "### MARK (%s)\n", mark);

		EnterCriticalSection (&CS);
		fprintf (log, "%s", buffer);
		fflush (log);
		LeaveCriticalSection (&CS);
	}
}

#if	LOG_TIMING
static inline void
pc_dlt (LARGE_INTEGER &base, LARGE_INTEGER &dlt)
{
	LARGE_INTEGER	pc;

	QueryPerformanceCounter (&pc);
	if (!lastPF.QuadPart) {
		QueryPerformanceFrequency (&lastPF);
		lastPF.QuadPart /= 1000000;
	}
	dlt.QuadPart = !base.QuadPart ? 0 : pc.QuadPart - base.QuadPart;
	dlt.QuadPart /= lastPF.QuadPart;
}
#endif	/* LOG_TIMING */

static void
log_alloc (const char *file, unsigned long line, const char *func, unsigned long size, void *ptr, LARGE_INTEGER &base, const char *caller)
{

	if (log) {
		char	header[64];
		char	buffer[512];
#if	LOG_TIMING
		LARGE_INTEGER	dlt;
		pc_dlt (base, dlt);
#else
		UNREFERENCED_PARAMETER (base);
#endif /* !LOG_TIMING */

		memset (header, 0, sizeof (header));
		_snprintf (header, sizeof (header) - 1, "+++ ptr=0x%8.8x size=0x%8.8lx", (unsigned int)ptr, size);

		memset (buffer, 0, sizeof (buffer));
#if	LOG_TIMING
#if	LOG_CALLER
		_snprintf (buffer, sizeof (buffer) - 1, "%-40s [0x%8.8x] (%s:%lu, %s, %lu bytes) <%I64d us> \"%s\"\n",
			header, GetCurrentThreadId(), file, line, func, size, dlt.QuadPart, caller);
#else	/* LOG_TIMING and !LOG_CALLER */
		UNREFERENCED_PARAMETER (caller);
		_snprintf (buffer, sizeof (buffer) - 1, "%-40s [0x%8.8x] (%s:%lu, %s, %lu bytes) <%I64d us>\n",
			header, GetCurrentThreadId(), file, line, func, size, dlt.QuadPart);
#endif	/* LOG_TIMING and !LOG_CALLER */
#else	/* !LOG_TIMING */
#if	LOG_CALLER
		_snprintf (buffer, sizeof (buffer) - 1, "%-40s [0x%8.8x] (%s:%lu, %s, %lu bytes) \"%s\"\n",
			header, GetCurrentThreadId(), file, line, func, size, caller);
#else	/* !LOG_TIMING and !LOG_CALLER */
		UNREFERENCED_PARAMETER (caller);
		_snprintf (buffer, sizeof (buffer) - 1, "%-40s [0x%8.8x] (%s:%lu, %s, %lu bytes)\n",
			header, GetCurrentThreadId(), file, line, func, size);
#endif	/* !LOG_TIMING and !LOG_CALLER */
#endif	/* !LOG_TIMING */

		EnterCriticalSection (&CS);
		fprintf (log, "%s", buffer);
		fflush (log);
		LeaveCriticalSection (&CS);
	}
}

static void
log_free (const char *file, unsigned long line, const char *func, void *ptr, LARGE_INTEGER &base, const char *caller)
{
	if (log) {
		char	header[64];
		char	buffer[512];

#if	LOG_TIMING
		LARGE_INTEGER	dlt;
		pc_dlt (base, dlt);
#else
		UNREFERENCED_PARAMETER (base);
#endif /* !LOG_TIMING */

		memset (header, 0, sizeof (header));
		_snprintf (header, sizeof (header) - 1, "--- ptr=0x%8.8x", (unsigned int)ptr);

		memset (buffer, 0, sizeof (buffer));
#if	LOG_TIMING
#if	LOG_CALLER
		_snprintf (buffer, sizeof (buffer) - 1, "%-40s [0x%8.8x] (%s:%lu, %s) <%I64d us> \"%s\"\n",
			header, GetCurrentThreadId(), file, line, func, dlt.QuadPart, caller);
#else	/* LOG_TIMING and !LOG_CALLER */
		UNREFERENCED_PARAMETER (caller);
		_snprintf (buffer, sizeof (buffer) - 1, "%-40s [0x%8.8x] (%s:%lu, %s) <%I64d us>\n",
			header, GetCurrentThreadId(), file, line, func, dlt.QuadPart);
#endif	/* LOG_TIMING and !LOG_CALLER */
#else	/* !LOG_TIMING */
#if	LOG_CALLER
		_snprintf (buffer, sizeof (buffer) - 1, "%-40s [0x%8.8x] (%s:%lu, %s) \"%s\"\n",
			header, GetCurrentThreadId(), file, line, func, caller);
#else	/* !LOG_TIMING and !LOG_CALLER */
		UNREFERENCED_PARAMETER (caller);
		_snprintf (buffer, sizeof (buffer) - 1, "%-40s [0x%8.8x] (%s:%lu, %s)\n",
			header, GetCurrentThreadId(), file, line, func);
#endif	/* !LOG_TIMING and !LOG_CALLER */
#endif	/* !LOG_TIMING */

		EnterCriticalSection (&CS);
		fprintf (log, "%s", buffer);
		fflush (log);
		LeaveCriticalSection (&CS);
	}
}

static void
log_new (const char *file, unsigned long line, const char *func, void *ptr, unsigned long size)
{
	if (log) {
		char	header[64];
		char	buffer[512];

		memset (header, 0, sizeof (header));
		_snprintf (header, sizeof (header) - 1, "NEW ptr=0x%8.8x size=0x%8.8lx", (unsigned int)ptr, size);

		memset (buffer, 0, sizeof (buffer));
		_snprintf (buffer, sizeof (buffer) - 1, "%-40s [0x%8.8x] (%s:%lu, %s, %lu bytes)\n",
			header, GetCurrentThreadId(), file, line, func, size);

		EnterCriticalSection (&CS);
		fprintf (log, "%s", buffer);
		fflush (log);
		LeaveCriticalSection (&CS);
	}
}

static void
log_delete (const char *file, unsigned long line, const char *func, void *ptr)
{
	if (log) {
		char	header[64];
		char	buffer[512];

		memset (header, 0, sizeof (header));
		_snprintf (header, sizeof (header) - 1, "DEL ptr=0x%8.8x", (unsigned int)ptr);

		memset (buffer, 0, sizeof (buffer));
		_snprintf (buffer, sizeof (buffer) - 1, "%-40s [0x%8.8x] (%s:%lu, %s)\n",
			header, GetCurrentThreadId(), file, line, func);

		EnterCriticalSection (&CS);
		fprintf (log, "%s", buffer);
		fflush (log);
		LeaveCriticalSection (&CS);
	}
}

static void
log_deletefree (void *ptr)
{
	if (log) {
		char	header[64];
		char	buffer[512];

		memset (header, 0, sizeof (header));
		_snprintf (header, sizeof (header) - 1, "D-- ptr=0x%8.8x", (unsigned int)ptr);

		memset (buffer, 0, sizeof (buffer));
		_snprintf (buffer, sizeof (buffer) - 1, "%-40s [0x%8.8x]\n",
			header, GetCurrentThreadId());

		EnterCriticalSection (&CS);
		fprintf (log, "%s", buffer);
		fflush (log);
		LeaveCriticalSection (&CS);
	}
}

void *
debug_memtrace_malloc (const char *file, unsigned long line, const char *func, size_t s)
{
	void		*ptr;
	LARGE_INTEGER	pc;

	QueryPerformanceCounter (&pc);
	ptr = malloc (s);
	if (ptr) log_alloc (file, line, func, s, ptr, pc, "malloc");
	return (ptr);
}

void *
debug_memtrace_calloc (const char *file, unsigned long line, const char *func, size_t n, size_t s)
{
	void		*ptr;
	LARGE_INTEGER	pc;

	QueryPerformanceCounter (&pc);
	ptr = calloc (n, s);
	if (ptr) log_alloc (file, line, func, n*s, ptr, pc, "calloc");
	return (ptr);
}

void
debug_memtrace_free (const char *file, unsigned long line, const char *func, void *p)
{
	LARGE_INTEGER	pc;

	QueryPerformanceCounter (&pc);
	free (p);
	log_free (file, line, func, p, pc, "free");
}

void *
debug_memtrace_realloc (const char *file, unsigned long line, const char *func, void *p, size_t s)
{
	void		*ptr;
	LARGE_INTEGER	pc;

	QueryPerformanceCounter (&pc);
	ptr = realloc (p, s);
	if (ptr || (p && !s)) {
		log_free (file, line, func, p, pc, "realloc");
		if (ptr) log_alloc (file, line, func, s, ptr, pc, "realloc");
	}
	return (ptr);

}

char *
debug_memtrace_strdup (const char *file, unsigned long line, const char *func, const char *p)
{
	char		*ptr;
	LARGE_INTEGER	pc;

	QueryPerformanceCounter (&pc);
	ptr = strdup (p);
	if (ptr) log_alloc (file, line, func, strlen (p), ptr, pc, "strdup");
	return (ptr);
}

void *
debug_memtrace_CoTaskMemAlloc (const char *file, unsigned long line, const char *func, unsigned long s)
{
	void		*ptr;
	LARGE_INTEGER	pc;

	QueryPerformanceCounter (&pc);
	ptr = CoTaskMemAlloc (s);
	if (ptr) log_alloc (file, line, func, s, ptr, pc, "CoTaskMemAlloc");
	return (ptr);
}

void
debug_memtrace_CoTaskMemFree (const char *file, unsigned long line, const char *func, void *p)
{
	LARGE_INTEGER	pc;

	QueryPerformanceCounter (&pc);
	CoTaskMemFree (p);
	log_free (file, line, func, p, pc, "CoTaskMemFree");

}

void *
debug_memtrace_CoTaskMemRealloc (const char *file, unsigned long line, const char *func, void *p, unsigned long s)
{
	void		*ptr;
	LARGE_INTEGER	pc;

	QueryPerformanceCounter (&pc);
	ptr = CoTaskMemRealloc (p, s);
	if (ptr || (p && !s)) {
		log_free (file, line, func, p, pc, "CoTaskMemRealloc");
		if (ptr) log_alloc (file, line, func, s, ptr, pc, "CoTaskMemRealloc");
	}
	return (ptr);
}

void
debug_memtrace_deletefree (void *p)
{
	log_deletefree (p);
}

void
debug_memtrace_deletefreearray (void *p)
{
	// Take into account the initial array count, otherwise the address logged here
	// will not match up with the allocation address. (the return value of new[])
	log_deletefree ((void *)((char *)p + sizeof(std::size_t)));
}

debug_memtrace_new::debug_memtrace_new (const char* file, unsigned long line, const char *func)
{
	this->file = file;
	this->line = line;
	this->func = func;
}

void
debug_memtrace_new::process (void *p, unsigned long s)
{
	if (p) log_new (file, line, func, p, s);
}

debug_memtrace_delete::debug_memtrace_delete (const char* file, unsigned long line, const char *func)
{
	this->file = file;
	this->line = line;
	this->func = func;

	log_delete (file, line, func, NULL);
}

