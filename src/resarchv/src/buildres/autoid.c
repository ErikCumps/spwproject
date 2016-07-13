/** \file
 * Resource Archive - buildres tool - auto-id handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include <stdlib.h>

#include <system/independence.h>
#include "idhash.h"
#include "autoid.h"

#define	AUTOID_START	(1)

static unsigned long	AUTOID_latest = AUTOID_START;

unsigned long
autoid (char *name)
{
	unsigned long	id;

	if (name == NULL) abort();

	id = AUTOID_latest;
	while (ih_find (id) != NULL) id++;

	AUTOID_latest = id + 1;
	return (id);
}

