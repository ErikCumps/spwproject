/** \file
 * The SPWaW Library - utility code: error handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	ERROR_H
#define	ERROR_H	1

/* NOTE: these convenience macros expect a local variable rc of type SPWAW_ERROR! */

#include <spwawlib_api.h>

extern void error (const char *file, unsigned long line, const char *func, char *fmt, ...);

#define	ERROR0(m_)		do { error (__FILE__, __LINE__, __FUNCTION__, m_); } while (0)
#define	ERROR1(m_,a1_)		do { error (__FILE__, __LINE__, __FUNCTION__, m_, a1_); } while (0)
#define	ERROR2(m_,a1_,a2_)	do { error (__FILE__, __LINE__, __FUNCTION__, m_, a1_, a2_); } while (0)

/*! Returns true case of error */
#define	HASERROR		(rc != SPWERR_OK)

/*! Returns in case of error */
#define	ROE(m_)								\
do {									\
	if (rc != SPWERR_OK) {						\
		ERROR2 ("%s failed (%s)", m_, SPWAW_errstr (rc));	\
		return (rc);						\
	}								\
} while (0)

/*! Sets error and return */
#define	RWE(e_, m_)							\
do {									\
	ERROR1 ("%s", m_);						\
	return (e_);							\
} while (0)

/*! Goes to label in case of error */
#define	ERRORGOTO(m_, l_)						\
do {									\
	if (rc != SPWERR_OK) {						\
		ERROR2 ("%s failed (%s)", m_, SPWAW_errstr (rc));	\
		goto l_;						\
	}								\
} while (0)

/*! Sets error and goes to label */
#define	FAILGOTO(e_, m_, l_)						\
do {									\
	ERROR1 ("%s", m_);						\
	rc = e_; goto l_;						\
} while (0)


/*! Check for SPWAWLIB initialization and return if error */
#define	CSPWINIT							\
do {									\
	if (!spwaw_initialized)	{					\
		RWE (SPWERR_NOTINIT, "library not yet initialized");	\
	}								\
} while (0)

/*! Verify argument is not NULL and return with error status if it is */
#define	CNULLARG(a_)							\
do {									\
	if (a_ == NULL) {						\
		RWE (SPWERR_NULLARG,					\
			"unexpected NULL value for " #a_ " argument");	\
	}								\
} while (0)

/*! Verify argument is not NULL and return with specific value if it is */
#define CWVNULL(a_, v_)							\
do {									\
	if (a_ == NULL) {						\
		ERROR0 ("unexpected NULL value for " #a_ " argument");	\
		return (v_);						\
	}								\
} while (0)

/*! Verify argument is not NULL and return if it is */
#define CWRNULL(a_)							\
do {									\
	if (a_ == NULL) {						\
		ERROR0 ("unexpected NULL value for " #a_ " argument");	\
		return;							\
	}								\
} while (0)

/*! Check alloced pointer for out of memory condition */
#define	COOM(p_, m_)							\
do {									\
	if (p_ == NULL) {						\
		ERROR1 ("out of memory allocating %s", m_);		\
		return (SPWERR_OUTOFMEM);				\
	}								\
} while (0)

/*! Check alloced pointer for out of memory condition */
#define	COOMRET(p_, m_, r_)						\
do {									\
	if (p_ == NULL) {						\
		ERROR1 ("out of memory allocating %s", m_);		\
		return (r_);						\
	}								\
} while (0)

/*! Check alloced pointer for out of memory condition */
#define	COOMGOTO(p_, m_, l_)						\
do {									\
	if (p_ == NULL) {						\
		rc = SPWERR_OUTOFMEM;					\
		ERROR1 ("out of memory allocating %s", m_);		\
		goto l_;						\
	}								\
} while (0)

#endif	/* ERROR_H */
