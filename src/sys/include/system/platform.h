/** \file
 * Platform independence interface.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SYS_PLATFORM_H
#define	SYS_PLATFORM_H	1

#ifdef	_WIN32

/***** windows *****/
#define	SYS_PF		"WIN32"
#define	SYS_PF_WIN32	1

#include <windows.h>
#include <io.h>

typedef	int	ssize_t;

#define	S_ISREG(mode)	(((mode) & _S_IFMT) == _S_IFREG)

#define	snprintf	_snprintf

#define	ARCH_WINDOWS	1

#elif	linux

/***** linux *****/
#define	SYS_PF		"LINUX"
#define	SYS_PF_LINUX	1

#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>

#define	O_BINARY	0

#define	ARCH_LINUX	1

#else
#error "Unsupported platform!"
#endif

#endif	/* SYS_PLATFORM_H */
