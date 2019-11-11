/** \file
 * The SPWaW Library - common header file.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_COMMON_H
#define	INTERNAL_COMMON_H	1

#include <stddef.h>
#include <limits.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <zlib.h>
#include <errno.h>
#include <malloc.h>

#define	vsnprintf	_vsnprintf
#define	snprintf	_snprintf
#define	alloca		_alloca

#include <debug_memtrace.h>

#endif	/* INTERNAL_COMMON_H */
