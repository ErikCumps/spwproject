/** \file
 * The SPWaW war cabinet - utility functions.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	UTIL_COMMON_H
#define	UTIL_COMMON_H	1

#include <util/util_strbuf.h>
#include <util/util_spwdlt.h>
#include <util/util_strhash.h>
#include <util/util_numhash.h>
#include <util/util_seqnum.h>
#include <util/util_uht_list_job.h>

extern void	UTIL_fmt_longspan	(SPWAW_PERIOD *span, char *buf, int len);
extern void	UTIL_fmt_longspan	(SPWAW_PERIOD *span, UtilStrbuf *buf);
extern void	UTIL_fmt_shortspan	(SPWAW_PERIOD *span, char *buf, int len);
extern void	UTIL_fmt_shortspan	(SPWAW_PERIOD *span, UtilStrbuf *buf);
extern void	UTIL_fmt_fullspan	(SPWAW_PERIOD *span, char *buf, int len);
extern void	UTIL_fmt_fullspan	(SPWAW_PERIOD *span, UtilStrbuf *buf);

/*! Simple macro to calculate the length of a (statically initialized) array */
#define	ARRAYCOUNT(arr_) (sizeof(arr_)/sizeof(arr_[0]))

#endif	/* UTIL_COMMON_H */
