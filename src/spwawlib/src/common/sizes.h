/** \file
 * The SPWaW Library - savegame sizes and definitions.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_SIZES_H
#define	INTERNAL_SIZES_H	1

#define	UNITCOUNT		1000
#define	FORMCOUNT		400
#define	FORMPCOUNT		100

#define	PLAYER1			0
#define	PLAYER2			1

/* Maximum supported number of units in a single formation.
 * Note: this is an arbitrary number - it could be anything... */
#define	MAXFORMATIONUNITS	128

/* Starting minor formation number for crews */
#define	CREWFSID		50

/* Starting minor formation number for special attached units */
#define	SPAUFSID		60

/* Starting minor formation number for crews of special attached units */
#define	SPAUCREWFSID		110

#endif	/* INTERNAL_SIZES_H */
