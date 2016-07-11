/** \file
 * debug_memtrace: C++ local delete implementations.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

/* Include this file ONCE in a SINGLE source file of your project! */

#ifndef	DEBUG_MEMTRACE_DELETE_H
#define	DEBUG_MEMTRACE_DELETE_H	1

#ifdef __cplusplus
#ifdef	DEBUG_MEMTRACE_CPP

#undef delete

void
operator delete (void *p) throw()
{
	debug_memtrace_deletefree (p);
}

void
operator delete (void *p, const std::nothrow_t&) throw()
{
	debug_memtrace_deletefree (p);
}

void
operator delete[] (void *p) throw()
{
	debug_memtrace_deletefree (p);
}

void
operator delete[] (void *p, const std::nothrow_t&) throw()
{
	debug_memtrace_deletefree (p);
}

#endif	/* DEBUG_MEMTRACE_CPP */
#endif	/* __cplusplus */

#endif	/* DEBUG_MEMTRACE_DELETE_H */
