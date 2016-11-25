/** \file
 * The SPWaW war cabinet - strategic map - hex grid object.
 *
 * Copyright (C) 2012-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "smap_hexgrid.h"
#include "dbg/dbg.h"

SmapHexGrid::SmapHexGrid (void)
{
	DBG_TRACE_CONSTRUCT;

	width = height = 0;
	map = NULL;
}

SmapHexGrid::~SmapHexGrid (void)
{
	DBG_TRACE_DESTRUCT;

	cleanup();
}

void
SmapHexGrid::setup (int width, int height)
{
	cleanup();
	if ((width <= 0) || (height <= 0)) return;

	this->width = width; this->height = height;
	map = new SmapHex[this->width*this->height];
}

void
SmapHexGrid::cleanup (void)
{
	if (map) delete[] map;

	width = height = 0;
	map = NULL;
}