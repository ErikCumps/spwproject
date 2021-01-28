/** \file
 * The SPWaW Library - winSPMBT build options.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gamefile/winspmbt/build_options_winspmbt.h"

COMMON_HANDLING_OPTIONS	winspmbt_handling_options = {
	EXP_WINSPMBT_SUBFUAL,
	EXP_WINSPMBT_SKIPNONAME,
	EXP_WINSPMBT_SKIPNOLDR,
	EXP_WINSPMBT_SKIPNOHCMD,
	EXP_WINSPMBT_ALLOWDUPF,
	EXP_WINSPMBT_FILTERDUPF,
	EXP_WINSPMBT_FILTERGAPF,
	EXP_WINSPMBT_VALIDITY,
	EXP_WINSPMBT_AUTOSPAU,
	EXP_WINSPMBT_AUTOCMPCR,
};

