/** \file
 * The SPWaW war cabinet - intel mode definition.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTEL_MODE_H
#define	INTEL_MODE_H	1

#include "util/util_strbuf.h"

/*! Intel mode (selects how much information is available about the opponent) */
typedef enum e_INTEL_MODE {
	INTEL_MODE_FULL = 0,	/*!< full intel available	*/
	INTEL_MODE_LMTD,	/*!< limited intel available	*/
	INTEL_MODE_NONE,	/*!< no intel available		*/
	INTEL_MODE_STARTCODE = INTEL_MODE_FULL,	/*! \internal	*/
	INTEL_MODE_LASTCODE = INTEL_MODE_NONE,	/*!< \internal	*/
} INTEL_MODE;
#define	INTEL_MODE_CNT	(INTEL_MODE_LASTCODE - INTEL_MODE_STARTCODE + 1)

extern const char	*intelmode2str	(INTEL_MODE mode);
extern bool		intelmode2html	(INTEL_MODE mode, UtilStrbuf &str, const char *style);
extern void		intelmode2label (INTEL_MODE mode, QLabel *label);
extern void		intelmode2label (INTEL_MODE mode, bool pflag, QLabel *label);

extern INTEL_MODE	raw2intelmode	(int mode);
extern int		intelmode2raw	(INTEL_MODE mode);

#endif	/* INTEL_MODE_H */
