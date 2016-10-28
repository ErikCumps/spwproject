/* stdafx.h : include file for standard system include files,
 * or project specific include files that are used frequently,
 * but are changed infrequently
 */

#ifndef	STDAFX_H
#define	STDAFX_H	1

/* We need at least Windows 2000 to run */
#define	MINWINVER	0x0500

#ifndef _WIN32_WINNT
#define	_WIN32_WINNT	MINWINVER
#endif
#if (_WIN32_WINNT < MINWINVER)
#error Only Windows 2000 or newer supported for this build!
#endif

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include <SL.h>
#include <resarchv.h>
#include <spwawlib.h>
#include "gui/gui_qt.h"
#include "plot/plot_qwt.h"

#include <debug_memtrace.h>

#endif	/* STDAFX_H */
