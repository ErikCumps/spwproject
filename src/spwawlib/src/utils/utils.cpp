/** \file
 * The SPWaW Library - utility code.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "common/internal.h"

void *
safe_malloc_core (const char *file, unsigned long line, const char *func, unsigned int size, const char *name)
{
	void	*ptr;

	/* prevent compiler warnings */
	UNREFERENCED_PARAMETER (file); UNREFERENCED_PARAMETER (line); UNREFERENCED_PARAMETER (func);

	if ((ptr = MEMTRACE_MALLOC (file, line, func, size)) == NULL) {
		ERROR1 ("out of memory allocating %s data", name);
	} else {
		memset (ptr, 0, size);
	}
	return (ptr);
}

void
safe_free_core (const char *file, unsigned long line, const char *func, void **ptr)
{
	if (!ptr || !*ptr) return;

	/* prevent compiler warnings */
	UNREFERENCED_PARAMETER (file); UNREFERENCED_PARAMETER (line); UNREFERENCED_PARAMETER (func);

	MEMTRACE_FREE (file, line, func, *ptr);
	*ptr = NULL;
}

void *
safe_realloc_core (const char *file, unsigned long line, const char *func, void *ptr, unsigned int size)
{
	void	*np;

	/* prevent compiler warnings */
	UNREFERENCED_PARAMETER (file); UNREFERENCED_PARAMETER (line); UNREFERENCED_PARAMETER (func);

	np = MEMTRACE_REALLOC (file, line, func, ptr, size);
	if (np == NULL) ERROR1 ("out of memory reallocating %u bytes", size);

	return (np);
}

void
fill_ptr_core (void *ptr, unsigned int size, int val)
{
	if (!size) return;

	memset ((char *)ptr, val, size);
}

void
aszstrcpy_core (char *src, char *dst, unsigned int size)
{
	fill_ptr_core (dst, size, 0);

	if (size <= 1) return;

	memcpy (dst, src, size - 1);
}

char *
aszstrstab_core (char *src, unsigned int size, STRTAB *stab)
{
	char	*buf = NULL, *p;

	if (!src || !size || !stab) return (NULL);

	buf = safe_nmalloc (char, size+1);
	if (!buf) return (NULL);

	memcpy (buf, src, size); p = STRTAB_add (stab, buf); free (buf);

	return (p);
}

void *
mmapfile (int fd, HANDLE *maph)
{
	HANDLE	hfd;
	void	*map;

	if (!maph) return (NULL);
	*maph = NULL;

	hfd = (void *)_get_osfhandle (fd);
	if (hfd == (void *)-1) return (NULL);

	*maph = CreateFileMapping (hfd, NULL, PAGE_READONLY, 0, 0, NULL);
	if (*maph == NULL) return (NULL);

	map = MapViewOfFile (*maph, FILE_MAP_READ, 0, 0, 0);
	if (map == NULL) {
		CloseHandle (*maph); *maph = NULL;
		return (NULL);
	}

	return (map);
}

void
unmmapfile (void **map, HANDLE *maph)
{
	if (!map || !maph) return;

	if (*map)  { UnmapViewOfFile (*map);	*map  = NULL; }
	if (*maph) { CloseHandle (*maph);	*maph = NULL; }
}

#define	MAXLINELEN	512

UtilStrbuf::UtilStrbuf (char *buf, unsigned long len, bool init, bool nl)
{
	clear_ptr (this);

	this->buf    = buf;
	this->size   = len;
	this->eot_nl = nl;

	if (!this->buf || !this->size) {
		this->buf  = NULL;
		this->size = 0;
		return;
	}

	if (init) memset (this->buf, 0, this->size);

	this->ptr = strchr (this->buf, 0);
	if (this->ptr) {
		this->left = this->size - (this->ptr - this->buf);
	} else {
		this->left = 0;
	}
}

UtilStrbuf::~UtilStrbuf (void)
{
}

bool
UtilStrbuf::full (void)
{
	return (left == 0);
}

void
UtilStrbuf::update (void)
{
	unsigned long	min = (eot_nl)?2:1;
	if (!buf) return;

	ptr = strchr (buf, 0);
	if (ptr) {
		left = size - (ptr - buf);
	}
	if (left <= min) left = 0;
}

void
UtilStrbuf::add (char *string)
{
	unsigned long	min = (eot_nl)?2:1;
	unsigned long	max;
	unsigned long	len;

	if (!buf || (left == 0)) return;

	max = (left>min)?(left-min):0;
	if ((len = strlen (string)) > max) len = max;
	if (len) {
		memcpy (ptr, string, len);
		ptr += len;
		left -= len;
	}
	if (left <= min) left = 0;
}

void
UtilStrbuf::printf (char *fmt, ...)
{
	va_list		AP;
	char		tmp[MAXLINELEN];

	if (!buf || (left == 0)) return;

	memset (tmp, 0, sizeof (tmp));
	va_start (AP, fmt);
	_vsnprintf (tmp, sizeof (tmp) - 1, fmt, AP);
	va_end (AP);

	add (tmp);
}
