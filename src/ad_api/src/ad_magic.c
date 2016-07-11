/** \file
 * The Aggregate Data API: private data integrity validation code.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include <stdlib.h>

#include "ad_magic.h"

void *
check_magic (void *ptr, unsigned int magic)
{
	AD_magic_t	*p;

	if (((p = ptr) != NULL) && (p->magic != magic)) abort ();
	return ((void *)p);
}

void *
check_node_magic (void *ptr, unsigned int magic)
{
	AD_node_magic_t	*p;

	if (((p = ptr) != NULL) && (p->magic != magic)) abort ();
	return ((void *)p);
}
