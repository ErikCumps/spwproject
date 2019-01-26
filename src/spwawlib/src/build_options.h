/** \file
 * The SPWaW Library - build options.
 *
 * Copyright (C) 2007-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef BUILD_OPTIONS_H
#define BUILD_OPTIONS_H	1

/* Set to 1 to enable IO logging */
#define	IOLOGGING	0

/* Set to 1 to enable (un)pack logging */
#define	PACKLOGGING	0

/* Set to 1 to enable unit/formation detection logging */
#define	UFDLOGGING	1

/* Set to 1 to enable unit/formation detection tracing */
#define	UFDTRACING	1



			/* winSPWW2 */	/* SPWAW */
#define	EXP_SUBFUAL	1		//0	// winSPWW2 formations without a leader get one assigned
#define	EXP_SKIPNOLDR	0		//1	// winSPWW2 formations that are completely destroyed have no leader
#define	EXP_SKIPNOHCMD	1		//0	// winSPWW2 formations must have a higher command link
#define	EXP_ALLOWDUPF	1		//1
#define	EXP_FILTERDUPF	1		//0
#define	EXP_FILTERGAPF	1		//0

#endif	/* BUILD_OPTIONS_H */

