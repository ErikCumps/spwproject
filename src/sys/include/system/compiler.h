/** \file
 * Compiler independence interface.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SYS_COMPILER_H
#define	SYS_COMPILER_H	1

/* Compiler specifics */
#ifdef	__GNUC__

/***** GCC *****/
#define	SYS_CC		"GCC"
#define	SYS_CC_GCC	1

#define	PACKED		__attribute__ ((packed))
#define	INLINE		inline

#elif	_MSC_VER

/***** Microsoft C *****/
#define	SYS_CC		"MSVC.NET"
#define	SYS_CC_MSC	2

#define	PACKED		/* MS C compiler does not support variable attributes */
#define	INLINE		__inline

#else
#error "Unsupported compiler!"
#endif

#endif	/* SYS_COMPILER_H */
