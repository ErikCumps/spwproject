/** \file
 * Resource Archive - buildres tool - utility functions.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include <stdlib.h>
#include <string.h>
#include "util.h"

#ifdef	DEBUG_MEMTRACE

#define	safe_malloc(fi_, li_, fu_, s_)		debug_memtrace_malloc	(fi_, li_, fu_, s_)
#define	safe_strdup(fi_, li_, fu_, p_)		debug_memtrace_strdup	(fi_, li_, fu_, p_)

#else	/* !DEBUG_MEMTRACE */

#define	safe_malloc(fi_, li_, fu_, s_)		malloc	(s_)
#define	safe_strdup(fi_, li_, fu_, p_)		strdup	(p_)

#endif	/* DEBUG_MEMTRACE */

void *
SAFE_malloc (const char *file, unsigned long line, const char *func, size_t size)
{
	void	*p;

	/* prevent compiler warnings */ file = file; line = line; func = func;

	p = safe_malloc (file, line, func, size); if (p == NULL) abort();
	memset (p, 0, size);
	return (p);
}

void
SAFE_strdup (const char *file, unsigned long line, const char *func, char *src, char **dst)
{
	char	*p;

	/* prevent compiler warnings */ file = file; line = line; func = func;

	if (dst == NULL) abort();

	p = safe_strdup (file, line, func, src); if (p == NULL) abort();
	*dst = p;
}

