/** \file
 * The SPWaW war cabinet - utility functions - simple buffered IO class.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"
#include "util_strbuf.h"

#define	MAXLINELEN	512
#define	BUFFERGROW	4096

UtilStrbuf::UtilStrbuf (char *buf, unsigned long len, bool init, bool nl)
{
	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.grow = false; d.buf = buf; d.size = len; d.eot_nl = nl; d.reserved = (d.eot_nl)?2:1;
	if (!d.buf || !d.size) return;

	if (init) memset (d.buf, 0, d.size);

	d.ptr = strchr (d.buf, 0);
	if (d.ptr) {
		d.left = d.size - (d.ptr - d.buf);
	} else {
		d.left = 0;
	}
}

UtilStrbuf::UtilStrbuf (bool nl)
{
	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.grow = true; d.eot_nl = nl; d.reserved = (d.eot_nl)?2:1;

	d.size = BUFFERGROW;
	SL_SAFE_CALLOC (d.buf, d.size, 1);
	d.ptr = d.buf;
	d.left = d.size;
}

UtilStrbuf::~UtilStrbuf (void)
{
	if (d.grow) SL_SAFE_FREE (d.buf);
}

bool
UtilStrbuf::full (void)
{
	return (d.grow?false:(d.left == 0));
}

void
UtilStrbuf::add (char *string)
{
	if (!string || (string[0] == '\0')) return;

	if (d.grow) {
		growing_add (string, strlen (string));
	} else {
		limited_add (string, strlen (string));
	}
}

void
UtilStrbuf::del (unsigned long cnt)
{
	if (!d.buf || (d.left == 0)) return;

	if ((d.left + cnt) > d.size) {
		clear ();
	} else {
		d.ptr -= cnt; d.left += cnt;
		memset (d.ptr, 0, cnt);
	}
}

void
UtilStrbuf::printf (char *fmt, ...)
{
	va_list		AP;
	char		tmp[MAXLINELEN];

	if (!d.buf || (d.left == 0)) return;

	memset (tmp, 0, sizeof (tmp));
	va_start (AP, fmt);
	_vsnprintf (tmp, sizeof (tmp) - 1, fmt, AP);
	va_end (AP);

	add (tmp);
}

void
UtilStrbuf::clear (void)
{
	if (!d.buf || !d.size) return;

	memset (d.buf, 0, d.size);
	d.ptr = d.buf; d.left = d.size;
}

char *
UtilStrbuf::data (void)
{
	return (d.buf);
}

void
UtilStrbuf::limited_add (char *str, unsigned long len)
{
	unsigned long	min = (d.eot_nl)?2:1;
	unsigned long	max;

	if (!d.buf || (d.left == 0)) return;

	max = (d.left>min)?(d.left-min):0;
	if (len > max) len = max;
	if (len) {
		memcpy (d.ptr, str, len);
		d.ptr += len;
		d.left -= len;
	}
	if (d.left <= min) d.left = 0;
}

void
UtilStrbuf::growing_add (char *str, unsigned long len)
{
	unsigned long	min = (d.eot_nl)?2:1;
	unsigned long	max;
	unsigned long	offset, nsize, nleft;

	max = (d.left>min)?(d.left-min):0;
	if (len > max) {
		offset = d.ptr - d.buf; nsize = d.size; nleft = d.left;
		while (len > max) {
			nsize += BUFFERGROW; nleft += BUFFERGROW;
			max = nleft-min;
		}
		SL_SAFE_REALLOC (d.buf, nsize);
		d.size = nsize;
		d.ptr = d.buf + offset;
		d.left = d.size - offset;
		memset (d.ptr, 0, d.left);
	}

	memcpy (d.ptr, str, len);
	d.ptr += len;
	d.left -= len;

	if (d.left <= min) d.left = 0;
}
