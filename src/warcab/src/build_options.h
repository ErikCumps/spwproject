/** \file
 * The SPWaW war cabinet - build options.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef BUILD_OPTIONS_H
#define BUILD_OPTIONS_H	1

/*! Set to 1 to enable debug tracing */
#define	DEBUG_TRACE		0

/*! Set to 1 to allow snapshot loading */
#define	ALLOW_SNAPSHOTS_LOAD	0

/*! Set to 1 to allow attempts to ignore soft errors */
#define	ALLOW_ERROR_IGNORE	0

/*! Set to 1 to allow attempts to ignore aborts */
#define	ALLOW_ABORT_IGNORE	0

/*! Set to 1 to allow experimental features
 * Warning: this may break the build and/or warcab functionality!
 */
#define	EXPERIMENTAL		0

#endif	/* BUILD_OPTIONS_H */

