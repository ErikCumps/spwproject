/** \file
 * Slay's Library - debug support: code location.
 *
 * Copyright (C) 2006-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */


#include "stdafx.h"
#include "internal_debug.h"
#include <SL/system.h>
#include <SL/safe_mem.h>
#include <SL/codeloc.h>



/* --- forward declarations --- */

/* --- private macros --- */

/* --- private data types --- */

/* --- private variables --- */



/* --- code --- */

/*! Fills code location structure
 * \param loc    Pointer to code location structure
 * \param file   File name
 * \param line   File line number
 * \param func   Function name
 */
void
SL_CODELOC_set (SL_CODELOC *loc, const char *file, unsigned long line, const char *func)
{
	ASSERT (loc != NULL);

	memset (loc, 0, sizeof (SL_CODELOC));
	if (file) memcpy (loc->file, file, sizeof (loc->file) - 1);
	if (func) memcpy (loc->func, func, sizeof (loc->func) - 1);
	loc->line = line;
}

/*! Copies code location structure
 * \param dst    Pointer to destination code location structure
 * \param src    Pointer to source code location structure
 */
void
SL_CODELOC_copy (SL_CODELOC *dst, SL_CODELOC *src)
{
	ASSERT (dst != NULL);

	memset (dst, 0, sizeof (SL_CODELOC));
	if (src) memcpy (dst, src, sizeof (SL_CODELOC));
}

/*! Creates code location
 * \param file   File name
 * \param line   File line number
 * \param func   Function name
 */
SL_CODELOC *
SL_HERE_I_AM_core (const char *file, unsigned long line, const char *func)
{
	SL_CODELOC	*loc;

	SL_SAFE_MALLOC (loc, sizeof (SL_CODELOC));
	SL_CODELOC_set (loc, file, line, func);

	return (loc);
}

/*! Frees code location
 * \param here   Pointer to code location
 */
void
SL_FREE_CODELOC_core (SL_CODELOC **here)
{
	if (here != NULL) SL_SAFE_FREE (*here);
}

