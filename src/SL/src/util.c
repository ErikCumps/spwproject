/** \file
 * Slay's Library - utility functions.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <SL/system.h>
#include <SL/util.h>



/* --- private variables --- */

/*! Boolean value string presentations */
static const char *value_names[2] = { "false", "true" };



/* --- API --- */

/*! Formats the given date and time
 * \param tm		pointer to time value
 * \param buf		pointer to storage for timestamp
 * \param len		size of storage for timestamp
 *
 * \note routine adapts timestamp format to available storage size
 */
void
SL_UTIL_timestamp (time_t *tm, char *buf, unsigned int len)
{
        struct tm	ltm;

	if (!buf || !len) return;
	memset (buf, 0, len);

	if (!tm) return;

	// TODO: protect localtime with lock
	memset (&ltm, 0, sizeof (ltm));
	memcpy (&ltm, localtime (tm), sizeof (ltm));

	if (len >= 20) {
	        strftime (buf, len - 1, "%Y/%m/%d %H:%M:%S", &ltm);
	} else if (len >= 18) {
		strftime (buf, len - 1, "%y/%m/%d %H:%M:%S", &ltm);
	} else if (len >= 15) {
		strftime (buf, len - 1, "%m/%d %H:%M:%S", &ltm);
	} else if (len >= 12) {
		strftime (buf, len - 1, "%d %H:%M:%S", &ltm);
	} else if (len >= 9) {
		strftime (buf, len - 1, "%H:%M:%S", &ltm);
	} else if (len >= 6) {
		strftime (buf, len - 1, "%M:%S", &ltm);
	} else if (len >= 3) {
		strftime (buf, len - 1, "%S", &ltm);
	}
}

/*! Safely copies va_list
 * \param dst   Destination
 * \param src   Source va_list
 */
void
SL_UTIL_va_copy (va_list *dst, va_list *src)
{
	ASSERT (dst != NULL);
	ASSERT (src != NULL);

#ifdef	__va_copy
	__va_copy (*dst, *src);
#else
	*dst = *src;
#endif	/* __va_copy */
}
