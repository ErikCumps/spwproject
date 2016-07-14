/** \file
 * Slay's Library - library options handling.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "internal_debug.h"
#include "internal_opts.h"
#include <SL/stdbg.h>
#include <SL/bool.h>



/* --- global variables --- */

SLOPT	SL_OPTIONS;



/* --- forward declarations --- */

static void	statereport	(SL_STDBG_INFO_LEVEL level);



/* --- private variables --- */

/*! Module name */
static const char	*MODULE = "SLOPT";

/*! Initialization status flag */
static SL_BOOL		initialized = SL_false;



/* --- code --- */

/*! Initializes SL options
 *
 * \return	initialization result code
 */
SL_SINIT
SLOPT_init (SLOPT *options)
{
	IDBGLOG0 ("start");

	if (initialized) {
		IDBGLOG0 ("already initialized");
		return (SL_SINIT_REINIT);
	}

	memset (&SL_OPTIONS, 0, sizeof (SL_OPTIONS));
	if (options) {
		memcpy (&SL_OPTIONS, options, sizeof (SL_OPTIONS));
	}

	SL_STDBG_add (statereport, MODULE);

	initialized = SL_true;
	IDBGLOG0 ("done");
	return (SL_SINIT_OK);
}

/*! Shuts down SL options
 */
void
SLOPT_shutdown (void)
{
	IDBGLOG0 ("start");

	if (!initialized) {
		IDBGLOG0 ("not yet initialized");
		return;
	}
	initialized = SL_false;

	SL_STDBG_delete (statereport);

	memset (&SL_OPTIONS, 0, sizeof (SL_OPTIONS));

	IDBGLOG0 ("done");
}

/*! Reports SL options state
 *
 * \param level	state report level
 */
static void
statereport (SL_STDBG_INFO_LEVEL level)
{
	SAYSTATE0 ("### SL options state:\n");

	/* basic information */
	if (level >= SL_STDBG_LEVEL_BAS) {
		SAYSTATE1 ("\tinitialized = %s\n", SL_BOOL_tostr (initialized));
	}

	/* extended information */
	if (level >= SL_STDBG_LEVEL_EXT) {
		SAYSTATE0 ("\tEvent callbacks:\n");
		SAYSTATE1 ("\t\tabort = 0x%8.8x\n", SL_OPTIONS.ecb.abort);
		SAYSTATE1 ("\t\terror = 0x%8.8x\n", SL_OPTIONS.ecb.error);
		SAYSTATE0 ("\tHooks:\n");
		SAYSTATE1 ("\t\toom   = 0x%8.8x\n", SL_OPTIONS.hook.oom);
		SAYSTATE1 ("\t\tpanic = 0x%8.8x\n", SL_OPTIONS.hook.panic);
		SAYSTATE1 ("\t\terror = 0x%8.8x\n", SL_OPTIONS.hook.error);
		SAYSTATE1 ("\t\texit  = 0x%8.8x\n", SL_OPTIONS.hook.exit);
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
	}

	SAYSTATE0 ("\n");
}
