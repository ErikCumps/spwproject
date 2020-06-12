/** \file
 * The SPWaW war cabinet - strategic map - hex grid object.
 *
 * Copyright (C) 2012-2020 Erik Cumps <erik.cumps@gmail.com>
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
SmapHexGrid::setup (SmapHexGrid &grid)
{
	cleanup();
	if ((grid.width <= 0) || (grid.height <= 0)) return;

	this->width = grid.width; this->height = grid.height;

	unsigned long cnt = this->width*this->height;
	map = new SmapHex[cnt];

	for (unsigned long i=0; i<cnt; i++) {
		map[i] = grid.map[i];
		map[i].unit_cnt_blue = 0;
		map[i].all_KIA_blue = true;
		map[i].unit_cnt_red = 0;
		map[i].all_KIA_red = true;
	}
}

void
SmapHexGrid::cleanup (void)
{
	if (map) delete[] map;

	width = height = 0;
	map = NULL;
}