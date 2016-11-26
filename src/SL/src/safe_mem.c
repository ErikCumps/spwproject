/** \file
 * Slay's Library - safe memory management handling.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "internal_debug.h"
#include <SL/system.h>
#include <SL/safe_mem.h>
#include "internal_opts.h"

#ifdef	DEBUG_MEMTRACE

#define	safe_calloc(fi_, li_, fu_, n_, s_)	debug_memtrace_calloc	(fi_, li_, fu_, n_, s_)
#define	safe_malloc(fi_, li_, fu_, s_)		debug_memtrace_malloc	(fi_, li_, fu_, s_)
#define	safe_realloc(fi_, li_, fu_, p_, s_)	debug_memtrace_realloc	(fi_, li_, fu_, p_, s_)
#define	safe_free(fi_, li_, fu_, p_)		debug_memtrace_free	(fi_, li_, fu_, p_)
#define	safe_strdup(fi_, li_, fu_, p_)		debug_memtrace_strdup	(fi_, li_, fu_, p_)

#else	/* !DEBUG_MEMTRACE */

#define	safe_calloc(fi_, li_, fu_, n_, s_)	calloc	(n_, s_)
#define	safe_malloc(fi_, li_, fu_, s_)		malloc	(s_)
#define	safe_realloc(fi_, li_, fu_, p_, s_)	realloc	(p_, s_)
#define	safe_free(fi_, li_, fu_, p_)		free	(p_)
#define	safe_strdup(fi_, li_, fu_, p_)		strdup	(p_)

#endif	/* DEBUG_MEMTRACE */

void
SL_SAFE_calloc (const char *file, unsigned long line, const char *func, void **p, size_t n, size_t s)
{
	void	*ptr = NULL;

	CAPNULLARG (p);
	*p = NULL;

	ptr = safe_calloc (file, line, func, n, s);
	if (ptr == NULL) {
		IDBGLOG5 ("%s:%lu %s: OOM: calloc (%lu * %lu bytes) failed!", file, line, func, n, s);

		if (SL_OPTIONS.hook.oom != NULL) SL_OPTIONS.hook.oom (file, line, func, (n*s));

		/* does not return */
		SL_system_abort (file, line, func, "OOM: calloc failed");
	}

	*p = ptr;
}

void
SL_SAFE_malloc (const char *file, unsigned long line, const char *func, void **p, size_t s)
{
	void	*ptr = NULL;

	CAPNULLARG (p);
	*p = NULL;

	ptr = safe_malloc (file, line, func, s);
	if (ptr == NULL) {
		IDBGLOG4 ("%s:%lu %s: OOM: malloc (%lu bytes) failed!", file, line, func, s);

		if (SL_OPTIONS.hook.oom != NULL) SL_OPTIONS.hook.oom (file, line, func, s);

		/* does not return */
		SL_system_abort (file, line, func, "OOM: malloc failed");
	}

	*p = ptr;
}

void
SL_SAFE_realloc (const char *file, unsigned long line, const char *func, void **p, size_t s)
{
	void	*ptr = NULL;

	CAPNULLARG (p); CAPNULLARG (*p);

	ptr = safe_realloc (file, line, func, *p, s);
	if (ptr == NULL) {
		IDBGLOG4 ("%s:%lu %s: OOM: realloc (%lu bytes) failed!", file, line, func, s);

		if (SL_OPTIONS.hook.oom != NULL) SL_OPTIONS.hook.oom (file, line, func, s);

		/* does not return */
		SL_system_abort (file, line, func, "OOM: realloc failed");
	}

	*p = ptr;
}

void
SL_SAFE_free (const char *file, unsigned long line, const char *func, void **p)
{
	CAPNULLARG (p);

#ifndef	DEBUG_MEMTRACE
	/* prevent compiler warnings */
	UNREFERENCED_PARAMETER (file);
	UNREFERENCED_PARAMETER (line);
	UNREFERENCED_PARAMETER (func);
#endif	/* !DEBUG_MEMTRACE */

	if (*p != NULL) {
		safe_free (file, line, func, *p);
		*p = NULL;
	} else {
		IDBGLOG3 ("%s:%lu %s: attempted to free NULL pointer", file, line, func);
	}
}

void
SL_SAFE_strdup (const char *file, unsigned long line, const char *func, void **p, const char *s)
{
	void		*ptr = NULL;
	unsigned long	len;

	CAPNULLARG (p);
	*p = NULL;

	if (!s) return;
	len = strlen(s) + 1;

	ptr = safe_strdup (file, line, func, s);
	if (ptr == NULL) {
		IDBGLOG4 ("%s:%lu %s: OOM: strdup (%lu bytes) failed!", file, line, func, len);

		if (SL_OPTIONS.hook.oom != NULL) SL_OPTIONS.hook.oom (file, line, func, len);

		/* does not return */
		SL_system_abort (file, line, func, "OOM: strdup failed");
	}

	*p = ptr;
}
