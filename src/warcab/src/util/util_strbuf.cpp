/** \file
 * The SPWaW war cabinet - utility functions - simple buffered IO class.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"
#include "util_strbuf.h"

#define	MAXLINELEN	512

UtilStrbuf::UtilStrbuf (char *buf, unsigned long len, bool init, bool nl)
{
	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.buf = buf; d.size = len; d.eot_nl = nl;
	if (!d.buf || !d.size) return;

	if (init) memset (d.buf, 0, d.size);

	d.ptr = strchr (d.buf, 0);
	if (d.ptr) {
		d.left = d.size - (d.ptr - d.buf);
	} else {
		d.left = 0;
	}
}

UtilStrbuf::~UtilStrbuf (void)
{
}

bool
UtilStrbuf::full (void)
{
	return (d.left == 0);
}

void
UtilStrbuf::update (void)
{
	unsigned long	min = (d.eot_nl)?2:1;
	if (!d.buf) return;

	d.ptr = strchr (d.buf, 0);
	if (d.ptr) {
		d.left = d.size - (d.ptr - d.buf);
	}
	if (d.left <= min) d.left = 0;
}

void
UtilStrbuf::add (char *string)
{
	unsigned long	min = (d.eot_nl)?2:1;
	unsigned long	max;
	unsigned long	len;

	if (!d.buf || (d.left == 0)) return;

	max = (d.left>min)?(d.left-min):0;
	if ((len = strlen (string)) > max) len = max;
	if (len) {
		memcpy (d.ptr, string, len);
		d.ptr += len;
		d.left -= len;
	}
	if (d.left <= min) d.left = 0;
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
