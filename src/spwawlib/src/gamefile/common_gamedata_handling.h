/** \file
 * The SPWaW Library - gamefile handling - common gamedata handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef COMMON_GAMEDATA_HANDLING_H
#define COMMON_GAMEDATA_HANDLING_H	1

typedef struct s_COMMON_HANDLING_OPTIONS {
	bool	SUBFUAL;	/* Substitute the first unit as leader for leaderless formations		*/
	bool	SKIPNOLDR;	/* Skip formations without a leader during formation detection			*/
	bool	SKIPNOHCMD;	/* Skip formations without a higher command link during formation detection	*/
	bool	ALLOWDUPF;	/* Allow duplicate formation IDs during formation detection			*/
	bool	FILTERDUPF;	/* Filter out duplicate formations during candidate formation verification	*/
	bool	FILTERGAPF;	/* Filter out big formation list gaps during candidate formation verification	*/
	bool	VALIDITY;	/* Verify candidate units based on unit validity				*/
	bool	AUTOSPAU;	/* Assume unverified units to be SPAU						*/
} COMMON_HANDLING_OPTIONS;

#endif	/* COMMON_GAMEDATA_HANDLING_H */

