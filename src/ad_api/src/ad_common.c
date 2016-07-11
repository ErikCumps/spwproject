/** \file
 * The Aggregate Data API: common code.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include <stdlib.h>

#include <ad_common.h>
#include <debug_memtrace.h>

static AD_OOM_hook_t	OOM_hook = NULL;

AD_OOM_hook_t
AD_OOM_set_hook	(AD_OOM_hook_t hook)
{
	AD_OOM_hook_t	old = NULL;

	old = OOM_hook;
	OOM_hook = hook;
	return (old);
}

int
call_OOM_hook (char *file, char *func, unsigned long line, unsigned long size)
{
	if (OOM_hook == NULL) abort ();
	return (OOM_hook (file, func, line, size));
}
