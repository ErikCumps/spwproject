/** \file
 * Slay's Library - internal debugging support.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef SL_INTERNAL_DEBUG_H
#define SL_INTERNAL_DEBUG_H	1

#include "stdafx.h"


#if	_DEBUG

extern void	IDBG_init	(void);
extern void	IDBG_shutdown	(void);
extern void	IDBG_log	(const char *file, unsigned long line, const char *func, const char *fmt, ...);

#define	IDBGLOG0(_fmt)					IDBG_log (__FILE__, __LINE__, __FUNCTION__, _fmt)
#define	IDBGLOG1(_fmt,_a1)				IDBG_log (__FILE__, __LINE__, __FUNCTION__, _fmt, _a1)
#define	IDBGLOG2(_fmt,_a1,_a2)				IDBG_log (__FILE__, __LINE__, __FUNCTION__, _fmt, _a1, _a2)
#define	IDBGLOG3(_fmt,_a1,_a2,_a3)			IDBG_log (__FILE__, __LINE__, __FUNCTION__, _fmt, _a1, _a2, _a3)
#define	IDBGLOG4(_fmt,_a1,_a2,_a3,_a4)			IDBG_log (__FILE__, __LINE__, __FUNCTION__, _fmt, _a1, _a2, _a3, _a4)
#define	IDBGLOG5(_fmt,_a1,_a2,_a3,_a4,_a5)		IDBG_log (__FILE__, __LINE__, __FUNCTION__, _fmt, _a1, _a2, _a3, _a4, _a5)
#define	IDBGLOG6(_fmt,_a1,_a2,_a3,_a4,_a5,_a6)		IDBG_log (__FILE__, __LINE__, __FUNCTION__, _fmt, _a1, _a2, _a3, _a4, _a5, _a6)
#define	IDBGLOG7(_fmt,_a1,_a2,_a3,_a4,_a5,_a6,_a7)	IDBG_log (__FILE__, __LINE__, __FUNCTION__, _fmt, _a1, _a2, _a3, _a4, _a5, _a6, _a7)

#else	/* !_DEBUG */

#define	IDBG_init()
#define	IDBG_shutdown()

#define	IDBGLOG0(_fmt)
#define	IDBGLOG1(_fmt,_a1)
#define	IDBGLOG2(_fmt,_a1,_a2)
#define	IDBGLOG3(_fmt,_a1,_a2,_a3)
#define	IDBGLOG4(_fmt,_a1,_a2,_a3,_a4)
#define	IDBGLOG5(_fmt,_a1,_a2,_a3,_a4,_a5)
#define	IDBGLOG6(_fmt,_a1,_a2,_a3,_a4,_a5,_a6)
#define	IDBGLOG7(_fmt,_a1,_a2,_a3,_a4,_a5,_a6,_a7)

#endif	/* !_DEBUG */

#endif	/* SL_INTERNAL_DEBUG_H */
