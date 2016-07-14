/** \file
 * Slay's Library - lowlevel system support.
 *
 * Copyright (C) 2001-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef SL_SYSTEM_H
#define SL_SYSTEM_H	1

#include <windows.h>

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */



/* --- Macros --- */

/* Discover max filename length */
/*! \def MAPP_FN_MAX
 *	 Maximum filename length (calculated at compile time)
 */
#ifdef	PATH_MAX
#	define	MAPP_FN_MAX	PATH_MAX
#else
#	define	MAPP_FN_MAX	255
#endif

/*! Maximum line length */
#define	MAX_SLLEN	8192



/* --- API --- */

extern void	SL_system_assert	(const char *file, unsigned long line, const char *func, const char *exp);
extern void	SL_system_abort		(const char *file, unsigned long line, const char *func, const char *msg);
extern HANDLE	SL_system_process	(void);
extern DWORD	SL_system_threadid	(void);
extern void	SL_system_coredump	(void);

#define	ABORT(_msg)								\
	do {									\
		SL_system_abort (__FILE__, __LINE__, __FUNCTION__, _msg);	\
	} while (0)



#ifdef	_DEBUG

/*! Debug version of ASSERT:
 * invokes system assert()
 *
 * \param _e	expression to test
 */
#define	ASSERT(_e)		(void)((_e)||(SL_system_assert(__FILE__, __LINE__, __FUNCTION__, #_e),0))

/*! Debug version of ASSERTR:
 * invokes system assert()
 *
 * \param _e	expression to test
 */
#define	ASSERTR(_e)		(void)((_e)||(SL_system_assert(__FILE__, __LINE__, __FUNCTION__, #_e),0))

/*! Debug version of ASSERTRV:
 * invokes system assert()
 *
 * \param _e	expression to test
 * \param _r	return value (unused)
 */
#define	ASSERTRV(_e, _r)	(void)((_e)||(SL_system_assert(__FILE__, __LINE__, __FUNCTION__, #_e),0))

#else	/* !_DEBUG */

/*! Non-debug version of ASSERT:
 * abort()s if expression is not zero
 *
 * \param _e	expression to test
 */
#define	ASSERT(_e)		(void)((_e)||(SL_system_assert(__FILE__, __LINE__, __FUNCTION__, #_e),0))

/*! Non-debug version of ASSERTR:
 * returns if expression is not zero
 *
 * \param _e	expression to test
 */
#define	ASSERTR(_e)				\
	do {					\
		if (!(_e)) return;		\
	} while (0)

/*! Non-debug version of ASSERTRV:
 * returns with specified return value if expression is not zero
 *
 * \param _e	expression to test
 * \param _r	value to return for non-zero expression
 */
#define	ASSERTRV(_e, _r)			\
	do {					\
		if (!(_e)) return (_r);		\
	} while (0)

#endif	/* !_DEBUG */


#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif /* SL_SYSTEM_H */
