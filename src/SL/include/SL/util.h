/** \file
 * Slay's Library - utility functions.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SL_UTIL_H
#define	SL_UTIL_H	1

#include <time.h>
#include <stdarg.h>

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */



/* --- API --- */

extern void	SL_UTIL_timestamp	(time_t *tm, char *buf, unsigned int len);
extern void	SL_UTIL_va_copy		(va_list *dst, va_list *src);


#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif	/* SL_UTIL_H */
