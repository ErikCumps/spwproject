/** \file
 * Slay's Library - lowlevel system support.
 *
 * Copyright (C) 2001-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <SL/system.h>
#include <SL/callbacks.h>
#include "internal_opts.h"


/* --- API --- */

_CRTIMP void __cdecl _assert(const char *, const char *, unsigned);

/*! Invokes system assert()
 */
void
SL_system_assert (const char *file, unsigned long line, const char *func, const char *exp)
{
	UNREFERENCED (func);

	_assert (exp, file, line);

	// If we get here it's because the user requested to ignore the abort condition!
	abort ();
}

/*! Invokes system abort()
 */
void
SL_system_abort (const char *file, unsigned long line, const char *func, const char *msg)
{
	SL_EVT_RC	rc;

	if (!SL_OPTIONS.ecb.abort) {
		rc = SL_EVT_HANDLE;
	} else {
		rc = SL_OPTIONS.ecb.abort (file, line, func, msg);
		if (rc == SL_EVT_CONTINUE) return;
	}

	if (rc == SL_EVT_HANDLE) {
		SL_system_assert (file, line, func, msg);
	}
}

/*! Returns process handle
 */
HANDLE
SL_system_process (void)
{
	return (GetCurrentProcess());
}

/*! Returns thread id of caller thread
 */
DWORD
SL_system_threadid (void)
{
	return (GetCurrentThreadId());
}

/*! Peforms coredump and exits
 */
void
SL_system_coredump (void)
{
	ABORT ("coredump request detected");
	ExitProcess (42);
}
