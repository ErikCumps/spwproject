/** \file
 * Slay's Library - boolean data type.
 *
 * Copyright (C) 2003-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <SL/system.h>
#include <SL/bool.h>



/* --- private variables --- */

/*! Boolean value string presentations */
static const char *value_names[2] = { "false", "true" };



/* --- API --- */

/*! Converts boolean value to string presentation (C API)
 *
 * \param abool	boolean value
 * \return	pointer to const string with value name
 */
const char *
SL_BOOL_tostr (SL_BOOL abool)
{
	return value_names[abool];
}

/*! Toggles boolean value (C API)
 *
 * \param abool	pointer to boolean value
 */
void
SL_BOOL_toggle (SL_BOOL *abool)
{
	ASSERTR (abool != NULL);

	*abool = (*abool == SL_true) ? SL_false : SL_true;
}

