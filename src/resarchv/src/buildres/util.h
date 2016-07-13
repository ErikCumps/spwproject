/** \file
 * Resource Archive - buildres tool - utility functions.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	UTIL_H
#define	UTIL_H	1

#include <debug_memtrace.h>

#define	UNREFERENCED(P)			do { (P); } while (0)

extern void *	SAFE_malloc	(const char *file, unsigned long line, const char *func, size_t size);
extern void	SAFE_strdup	(const char *file, unsigned long line, const char *func, char *src, char **dst);

#define	TYPEMALLOC(var, type)		do { var = (type *)SAFE_malloc(__FILE__, __LINE__, __FUNCTION__, sizeof (type)); } while (0)
#define	SIZEMALLOC(var, type, size)	do { var = (type *)SAFE_malloc(__FILE__, __LINE__, __FUNCTION__, size); } while (0)

#define	SAFE_MALLOC(s_)		SAFE_malloc (__FILE__, __LINE__, __FUNCTION__, s_)
#define	SAFE_STRDUP(s_, d_)	SAFE_strdup (__FILE__, __LINE__, __FUNCTION__, s_, d_)

#endif	/* UTIL_H */

