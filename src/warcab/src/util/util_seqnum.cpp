/** \file
 * The SPWaW war cabinet - utility functions - simple sequence number class.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"
#include "util_seqnum.h"

#define	MAXLINELEN	512

UtilSeqnum::UtilSeqnum (void)
{
	/* Initialize */
	seq = 0;
}

UtilSeqnum::~UtilSeqnum (void)
{
}

void
UtilSeqnum::update (void)
{
	seq++;
}

bool
UtilSeqnum::is_updated (unsigned int &ref)
{
	unsigned int	s = seq;
	bool		b = (ref != s);

	ref = s;
	return (b);
}

unsigned int
UtilSeqnum::value (void)
{
	return (seq);
}