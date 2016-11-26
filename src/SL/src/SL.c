/** \file
 * Slay's Library - main implementation file.
 *
 * Copyright (C) 2006-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <SL.h>
#include "internal_debug.h"
#include "internal_opts.h"
#include "stdbg_private.h"
#include "basic_output.h"



/* --- forward declarations --- */
static void	SL_app_shutdown	(void);




/* --- private macros  --- */

/*! Convenience macros */
#define	ROE(c_)		if (c_ != SL_INIT_OK) return (c_)
#define	CHECK(c_, rc_)	if (c_ != SL_INIT_OK) return (rc_)



/* --- private variables --- */

/*! Initialization reference count */
static LONG	refcnt = 0;



/* --- code --- */

/*! Initializes SL components
 *
 * \return	initialization result code
 *
 * \todo Implement argument passing?
 * \todo Handle initial trace log if TRACE enabled
 */
SL_INIT
SL_init (SL_APP_INFO *info, SLOPT *opt)
{
	int		rc;

	/* Initialize basic library subsystems */
	rc = SL_lib_init ();		ROE (rc);

	/* Register our shutdown routine */
	atexit (SL_app_shutdown);

	/* Initialize critical subsystems */
	rc = SL_APP_init (info);	CHECK (rc, SL_INIT_APP);

	/* Initialize options */
	rc = SLOPT_init (opt);		CHECK (rc, SL_INIT_SLOPTS);

	#if	TRACE
		/* LOG_init ("./INITIAL_TRACE_LOG", &trace_level); */
	#endif	/* TRACE */

	return (SL_INIT_OK);
}

/*! Initializes SL components (library support only)
 *
 * \return	initialization result code
 *
 * \todo Implement argument passing?
 * \todo Handle initial trace log if TRACE enabled
 */
SL_INIT
SL_lib_init (void)
{
	/* Register our shutdown routine */
	atexit (SL_lib_shutdown);

	if (InterlockedIncrement (&refcnt) == 1)
	{
		int	rc;

		/* Initialize internal debugging */
		IDBG_init ();

		/* Initialize critical subsystems */
		rc = SLMSG_init ();		CHECK (rc, SL_INIT_SLMSG);
		rc = SL_STDBG_init ();		CHECK (rc, SL_INIT_STDBG);
		rc = SLBO_init ();		CHECK (rc, SL_INIT_SLBO);
		rc = SL_THREAD_init ();		CHECK (rc, SL_INIT_THREAD);

		/* Initialize other subsystems */
		rc = SL_EXIT_init ();		CHECK (rc, SL_INIT_EXIT);
		rc = SL_ERROR_init ();		CHECK (rc, SL_INIT_ERROR);
	}

	return (SL_INIT_OK);
}

/*! Shuts down SL library (application support only)
 */
static void
SL_app_shutdown (void)
{
	SLOPT_shutdown ();
	SL_APP_shutdown ();
}

/*! Shuts down SL library (library support only)
 */
void
SL_lib_shutdown (void)
{
	if (InterlockedDecrement (&refcnt) == 0)
	{
		/* Shut down subsystems */
		SL_ERROR_shutdown ();
		SL_EXIT_shutdown ();
		SL_THREAD_shutdown ();
		SLBO_shutdown ();
		SL_STDBG_shutdown ();
		SLMSG_shutdown ();

		/* Shut down internal debugging */
		IDBG_shutdown ();
	}
}

/*! Shuts down SL library
 */
void
SL_shutdown (void)
{
	SLOPT_shutdown ();
	SL_APP_shutdown ();
	SL_lib_shutdown ();
}
