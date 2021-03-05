/** \file
 * The SPWaW Library - winSPMBT build options.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef BUILD_OPTIONS_WINSPMBT_H
#define BUILD_OPTIONS_WINSPMBT_H	1

/* Set to 1 to substitute the first unit as leader for leaderless formations */
#define	EXP_WINSPMBT_SUBFUAL	1

/* Set to 1 to skip formations without a valid name during formation detection */
#define	EXP_WINSPMBT_SKIPNONAME	1

/* Set to 1 to skip formations without a leader during formation detection */
#define	EXP_WINSPMBT_SKIPNOLDR	0

/* Set to 1 to skip formations without a higher command link during formation detection */
#define	EXP_WINSPMBT_SKIPNOHCMD	1

/* Set to 1 to allow duplicate formation IDs during formation detection */
#define	EXP_WINSPMBT_ALLOWDUPF	1

/* Set to 1 to filter out duplicate formations during candidate formation verification */
#define	EXP_WINSPMBT_FILTERDUPF	1

/* Set to 1 to filter out big formation list gaps during candidate formation verification */
#define	EXP_WINSPMBT_FILTERGAPF	1

/* Set to 1 to verify candidate units based on unit validity */
#define	EXP_WINSPMBT_VALIDITY	1

/* Set to 1 to assume unverified units to be SPAU */
#define	EXP_WINSPMBT_AUTOSPAU	1

/* Set to 1 to assume unverified units in core formations to be core for campaign battles */
#define	EXP_WINSPMBT_AUTOCMPCR	1

#include "gamefile/common_gamedata_handling.h"

extern COMMON_HANDLING_OPTIONS	winspmbt_handling_options;

#endif	/* BUILD_OPTIONS_WINSPMBT_H */

