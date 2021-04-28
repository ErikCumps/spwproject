/** \file
 * The SPWaW war cabinet - strategic map - hex object.
 *
 * Copyright (C) 2012-2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "smap_hex.h"

SmapHex::SmapHex (void)
{
	erase();
}

SmapHex::~SmapHex (void)
{
}

void
SmapHex::setPos (SmapHexPos &pos)
{
	this->pos = pos;
	d.ready = true;
}

void
SmapHex::setHeight	(int raw_height)
{
	if (!d.ready) return;

	height = raw2ht (raw_height);
	actheight = raw_height;
}

void
SmapHex::setWater	(bool has_water)
{
	if (!d.ready) return;

	water = has_water;
}

void
SmapHex::setBridge	(bool has_bridge)
{
	if (!d.ready) return;

	bridge = has_bridge;
}

void
SmapHex::setRoadConn (int road1, int road2, int railr, int traml)
{
	if (!d.ready) return;

	conn_road1 = road1;
	conn_road2 = road2;
	conn_railr = railr;
	conn_traml = traml;
	conn_bridge = 0;
}

void
SmapHex:: setVicHex (SPWAW_VHSTATUS owner, INTEL_MODE mode)
{
	if (!d.ready) return;

	vic_hex = true;
	switch (owner) {
		case SPWAW_VHP1:
			vic_hex_owner = SMAP_HI_BLUE;
			break;
		case SPWAW_VHP2:
			vic_hex_owner = (mode != INTEL_MODE_NONE) ? SMAP_HI_RED : SMAP_HI_CONTESTED;
			break;
		case SPWAW_VHN:
		default:
			vic_hex_owner = (mode != INTEL_MODE_NONE) ? SMAP_HI_NONE : SMAP_HI_CONTESTED;
			break;
	}
}

void
SmapHex::addUnit (SMAP_HI influence, bool kia)
{
	if (!d.ready) return;

	switch (influence) {
		case SMAP_HI_BLUE:
			unit_cnt_blue++;
			if (!kia) all_KIA_blue = false;
			break;
		case SMAP_HI_RED:
			unit_cnt_red++;
			if (!kia) all_KIA_red = false;
			break;
		default:
			break;
	}
}

void
SmapHex::erase (void)
{
	pos.set (-1, -1);
	height = SMAP_HH_000;
	actheight = 0;
	water = false;
	bridge = false;
	conn_road1 = conn_road2 = conn_railr = conn_traml = conn_bridge = 0;
	vic_hex = false;
	vic_hex_owner = SMAP_HI_NONE;
	unit_cnt_blue = unit_cnt_red = 0;
	all_KIA_blue = all_KIA_red = true;
	influence_blue_cnt = influence_red_cnt = 0;
	influence_blue = influence_red = 0.0;
	influence = SMAP_HI_NONE;
	frontline = 0;

	d.ready = false;
}

void
SmapHex::clearUnits (void)
{
	unit_cnt_blue = unit_cnt_red = 0;
	all_KIA_blue = all_KIA_red = true;
}

bool
SmapHex::hasBridge (void)
{
	return (bridge);
}

bool
SmapHex::hasRoad (SMAP_HRD dir)
{
	int conns =	(conn_road1 & SMAP_HRD_180mask[dir]) |
			(conn_road2 & SMAP_HRD_180mask[dir]) |
			(conn_railr & SMAP_HRD_180mask[dir]) |
			(conn_traml & SMAP_HRD_180mask[dir]);

	return (conns != 0);
}
