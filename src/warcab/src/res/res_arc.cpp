/** \file
 * The SPWaW war cabinet - resource archive handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"

static void	statereport	(SL_STDBG_INFO_LEVEL level);

/*! Module name */
static const char	*MODULE = "RES_ARC";

static SL_BOOL		initialized = SL_false;
static arc_t		*arc = NULL;

SL_ERROR
RES_ARC_init (char *filename)
{
	if (initialized) RETURN_OK;

	CAPNULLARG(filename);

	arc = arc_load (filename, NULL);
	if (!arc) RETURN_ERR_FUNCTION_EX1 (ERR_ARC_OPEN_FAILED, "resource archive file \"%s\"", filename);

	SL_STDBG_add (statereport, MODULE);

	initialized = SL_true;
	RETURN_OK;
}

void
RES_ARC_shutdown (void)
{
	if (!initialized) return;

	SL_STDBG_delete (statereport);

	if (arc) { arc_close (arc); arc = NULL; }
	initialized = SL_false;
}

SL_ERROR
RES_ARC_item (int id, arc_item_t **item)
{
	CAPNULLARG (item);

	*item = arc_byid (arc, id);
	if (!*item) RETURN_ERR_FUNCTION_EX1 (ERR_ARC_ITEM_NOTFOUND, "item id %d", id);

	RETURN_OK;
}

static void
statereport (SL_STDBG_INFO_LEVEL level)
{
	SAYSTATE0 ("### Resource library state:\n");

	/* basic information */
	if (level >= SL_STDBG_LEVEL_BAS) {
		SAYSTATE1 ("\tinitialized = %s\n", SL_BOOL_tostr (initialized));
	}

	/* extended information */
	if (level >= SL_STDBG_LEVEL_EXT) {
		SAYSTATE1 ("\tarc = 0x%8.8x\n", arc);
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
	}

	SAYSTATE0 ("\n");
}
