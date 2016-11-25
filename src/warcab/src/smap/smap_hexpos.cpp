/** \file
 * The SPWaW war cabinet - strategic map - hex position object.
 *
 * Copyright (C) 2012-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "smap_hexpos.h"

SmapHexPos::SmapHexPos (void)
{
	/* The (-1,-1) position is always off-grid */
	set (-1, -1);
}

SmapHexPos::~SmapHexPos (void)
{
	/* Even though destructed, the backing memory of this object could live on,
	 * therefore initialize this to the safe, off-grid, (-1,-1) position.
	 */
	set (-1, -1);
}

SmapHexPos &
SmapHexPos::set (int posx, int posy)
{
	x = posx;
	y = posy;

	idx = -1;
	d.has_idx = false;

	return (*this);
}
