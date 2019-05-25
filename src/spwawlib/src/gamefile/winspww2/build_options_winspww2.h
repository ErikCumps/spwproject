/** \file
 * The SPWaW Library - winSPWW2 build options.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef BUILD_OPTIONS_WINSPWW2_H
#define BUILD_OPTIONS_WINSPWW2_H	1

/* Set to 1 to substitute the first unit as leader for leaderless formations */
#define	EXP_WINSPWW2_SUBFUAL	1

/* Set to 1 to skip formations without a leader during formation detection */
#define	EXP_WINSPWW2_SKIPNOLDR	0

/* Set to 1 to skip formations without a higher command link during formation detection */
#define	EXP_WINSPWW2_SKIPNOHCMD	1

/* Set to 1 to allow duplicate formation IDs during formation detection */
#define	EXP_WINSPWW2_ALLOWDUPF	1

/* Set to 1 to filter out duplicate formations during candidate formation verification */
#define	EXP_WINSPWW2_FILTERDUPF	1

/* Set to 1 to filter out big formation list gaps during candidate formation verification */
#define	EXP_WINSPWW2_FILTERGAPF	1

/* Set to 1 to verify candidate units based on unit validity */
#define	EXP_WINSPWW2_VALIDITY	1

/* Set to 1 to assume unverified units to be SPAU */
#define	EXP_WINSPWW2_AUTOSPAU	1

#endif	/* BUILD_OPTIONS_WINSPWW2_H */

