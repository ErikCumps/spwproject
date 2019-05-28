/** \file
 * The SPWaW Library - SPWaW build options.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef BUILD_OPTIONS_SPWAW_H
#define BUILD_OPTIONS_SPWAW_H	1

/* Set to 1 to substitute the first unit as leader for leaderless formations */
#define	EXP_SPWAW_SUBFUAL	0

/* Set to 1 to skip formations without a leader during formation detection */
#define	EXP_SPWAW_SKIPNOLDR	1

/* Set to 1 to skip formations without a higher command link during formation detection */
#define	EXP_SPWAW_SKIPNOHCMD	0

/* Set to 1 to allow duplicate formation IDs during formation detection */
#define	EXP_SPWAW_ALLOWDUPF	1

/* Set to 1 to filter out duplicate formations during candidate formation verification */
#define	EXP_SPWAW_FILTERDUPF	0

/* Set to 1 to filter out big formation list gaps during candidate formation verification */
#define	EXP_SPWAW_FILTERGAPF	0

/* Set to 1 to verify candidate units based on unit validity */
#define	EXP_SPWAW_VALIDITY	0

/* Set to 1 to assume unverified units to be SPAU */
#define	EXP_SPWAW_AUTOSPAU	0

#include "gamefile/common_gamedata_handling.h"

extern COMMON_HANDLING_OPTIONS	spwaw_handling_options;

#endif	/* BUILD_OPTIONS_SPWAW_H */

