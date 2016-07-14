/* stdafx.h: include file for standard system include files,
 * or project specific include files that are used frequently,
 * but are changed infrequently
 */

#ifndef	STDAFX_H
#define	STDAFX_H	1

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>
#include <time.h>

#define	snprintf	_snprintf
#define	vsnprintf	_vsnprintf

#include <debug_memtrace.h>

#define	UNREFERENCED(P)	do { (P); } while (0)

#endif	/* STDAFX_H */