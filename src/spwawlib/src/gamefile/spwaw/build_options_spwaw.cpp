/** \file
 * The SPWaW Library - SPWaW build options.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gamefile/spwaw/build_options_spwaw.h"

COMMON_HANDLING_OPTIONS	spwaw_handling_options = {
	EXP_SPWAW_SUBFUAL,
	EXP_SPWAW_SKIPNONAME,
	EXP_SPWAW_SKIPNOLDR,
	EXP_SPWAW_SKIPNOHCMD,
	EXP_SPWAW_ALLOWDUPF,
	EXP_SPWAW_FILTERDUPF,
	EXP_SPWAW_FILTERGAPF,
	EXP_SPWAW_VALIDITY,
	EXP_SPWAW_AUTOSPAU,
	EXP_SPWAW_AUTOCMPCR,
};
