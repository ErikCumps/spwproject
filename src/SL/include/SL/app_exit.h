/** \file
 * Slay's Library - application termination handling.
 *
 * Copyright (C) 2003-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SL_APP_EXIT_H
#define	SL_APP_EXIT_H	1

#include <SL/init_ids.h>

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */



/* --- Data types and structures --- */

/*! Code fixup pointer */
typedef unsigned long SL_EXIT_FIXUP;

/* --- Macro definitions --- */

/*! Exit code for normal termination */
#define	SL_EXIT_NORMAL			  0

/*! Exit code for usage error */
#define	SL_EXIT_USAGE_ERROR		  1

/*! Exit code for fatal error */
#define	SL_EXIT_FATAL_ERROR		  2

/*! Exit code for death by lost error stack */
#define	SL_EXIT_DEATH_LOST_ERROR_STACK	100



/* --- API --- */

extern SL_SINIT	SL_EXIT_init			(void);
extern void	SL_EXIT_shutdown		(void);

extern void	SL_EXIT_exit			(unsigned int ec, char *msg);
extern void	SL_EXIT_exit_and_dump		(unsigned int ec, char *msg, SL_EXIT_FIXUP *fix);
extern void	SL_EXIT_exit_NOW		(unsigned int ec, char *msg);
extern void	SL_EXIT_exit_and_dump_NOW	(unsigned int ec, char *msg, SL_EXIT_FIXUP *fix);

/*! Application termination without reporting */
#define SL_SUDDEN_DEATH(_ec, _msg)		SL_EXIT_exit_NOW (_ec, _msg)

/*! Application termination with coredump but without reporting */
#define	SL_SUDDEN_COREDUMP(_ec, _msg, _fix)	SL_EXIT_exit_and_dump_NOW (_ec, _msg, _fix)

/*! Application termination with reporting */
#define	SL_DIE(_ec, _msg)			SL_EXIT_exit (_ec, _msg)

/*! Application termination with reporting and coredump */
#define	SL_DIE_AND_DUMP(_ec, _msg, _fix)	SL_EXIT_exit_and_dump (_ec, _msg, _fix)



#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif	/* SL_APP_EXIT_H */
