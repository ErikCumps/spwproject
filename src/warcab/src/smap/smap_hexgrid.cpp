/** \file
 * The SPWaW war cabinet - strategic map - hex grid object.
 *
 * Copyright (C) 2012-2021 Erik Cumps <erik.cumps@gmail.com>
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
	/* Don't recreate if the grid dimensions match */
	if ((this->width != width) || (this->height != height)) {
		cleanup();
		if ((width <= 0) || (height <= 0)) return;

		this->width = width; this->height = height;
		map = new SmapHex[this->width*this->height];
	}
}

void
SmapHexGrid::setup (SmapHexGrid &grid)
{
	/* Don't recreate if the grid dimensions match */
	if ((this->width != grid.width) || (this->height != grid.height)) {
		cleanup();
		if ((grid.width <= 0) || (grid.height <= 0)) return;

		this->width = grid.width; this->height = grid.height;

		map = new SmapHex[this->width*this->height];
	}

	for (long i=0; i<(this->width*this->height); i++) {
		map[i] = grid.map[i];
		map[i].clearUnits();
	}
}

void
SmapHexGrid::erase (void)
{
	for (long i=0; i<(this->width*this->height); i++) {
		map[i].erase();
	}
}

void
SmapHexGrid::cleanup (void)
{
	if (map) delete[] map;

	width = height = 0;
	map = NULL;
}