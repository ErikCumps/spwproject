/** \file
 * The SPWaW war cabinet - utility functions - simple MDLD_TREE_ITEM reference tracking class.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"
#include "util_mdltree_reftrack.h"

UtilMdlTreeReftrack::UtilMdlTreeReftrack (void)
{
	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.active = false;
}

UtilMdlTreeReftrack::~UtilMdlTreeReftrack (void)
{
}

bool
UtilMdlTreeReftrack::changed (MDLD_TREE_ITEM *item)
{
	bool	b;

	if (!d.active || (item != d.ptr)) {
		b = true;
	} else {
		b = d.ptr ? d.ptr->seqnum.is_updated (d.seq) : false;
	}

	update(item);

	return (b);
}

void
UtilMdlTreeReftrack::update (MDLD_TREE_ITEM *item)
{
	d.ptr = item;
	d.seq = d.ptr ? d.ptr->seqnum.value() : 0;
	d.active = true;
}
