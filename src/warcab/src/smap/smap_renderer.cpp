/** \file
 * The SPWaW war cabinet - strategic map - rendering.
 *
 * Copyright (C) 2012-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "smap_renderer.h"
#include "dbg/dbg.h"

static inline	int	hthm2idx	(int h, int m)
{
	return (h*SMAP_HM_CNT+m);
}

SmapRenderer::SmapRenderer (SMAP_RENDERDATA &renderdata)
{
	int	hidx, midx;
	int	idx;

	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.pm_width = renderdata.width; d.pm_height = renderdata.height;
	
	d.hex_side = renderdata.side;
	d.hex_top = ((d.pm_height - 1) - (d.hex_side - 1)) / 2;

	d.hmove_x = d.pm_width - 1;
	d.hmove_y = 0;

	d.vmove_x = (d.pm_width - 1) / 2;
	d.vmove_y = d.hex_top + ((d.pm_height - 1) / 2) - 1;

	d.pm.border = renderdata.xpms.border;

	d.pm.selection = renderdata.xpms.sel;
	d.pm.selection.setAlphaChannel (renderdata.xpms.selmask);

	d.pm.hexmask[SMAP_HM_C]  = renderdata.xpms.maskC;
	d.pm.hexmask[SMAP_HM_LE] = renderdata.xpms.maskLE;
	d.pm.hexmask[SMAP_HM_RE] = renderdata.xpms.maskRE;
	d.pm.hexmask[SMAP_HM_RO] = renderdata.xpms.maskRO;
	d.pm.hexmask[SMAP_HM_B]  = renderdata.xpms.maskB;
	d.pm.hexmask[SMAP_HM_BR] = renderdata.xpms.maskBR;

	d.pm.transparent = renderdata.xpms.mask;

	d.pm.height[hthm2idx(SMAP_HH_NEG, SMAP_HM_START)] = renderdata.xpms.hneg;
	d.pm.height[hthm2idx(SMAP_HH_000, SMAP_HM_START)] = renderdata.xpms.h000;
	d.pm.height[hthm2idx(SMAP_HH_005, SMAP_HM_START)] = renderdata.xpms.h005;
	d.pm.height[hthm2idx(SMAP_HH_010, SMAP_HM_START)] = renderdata.xpms.h010;
	d.pm.height[hthm2idx(SMAP_HH_015, SMAP_HM_START)] = renderdata.xpms.h015;
	d.pm.height[hthm2idx(SMAP_HH_020, SMAP_HM_START)] = renderdata.xpms.h020;
	d.pm.height[hthm2idx(SMAP_HH_025, SMAP_HM_START)] = renderdata.xpms.h025;
	d.pm.height[hthm2idx(SMAP_HH_030, SMAP_HM_START)] = renderdata.xpms.h030;
	d.pm.height[hthm2idx(SMAP_HH_035, SMAP_HM_START)] = renderdata.xpms.h035;

	for (hidx=SMAP_HH_START; hidx<=SMAP_HH_LAST; hidx++) {
		for (midx=SMAP_HM_START; midx<=SMAP_HM_LAST; midx++) {
			idx = hthm2idx(hidx, midx);
			if (midx != SMAP_HM_START) d.pm.height[idx] = d.pm.height[hthm2idx(hidx, SMAP_HM_START)];
			d.pm.height[idx].setAlphaChannel (d.pm.hexmask[midx]);
		}
	}

	for (midx=SMAP_HM_START; midx<=SMAP_HM_LAST; midx++) {
		d.pm.water[midx] = renderdata.xpms.water;
		d.pm.water[midx].setAlphaChannel (d.pm.hexmask[midx]);
	}

	d.pm.bridge[0] = renderdata.xpms.bridgeEE;
	d.pm.bridge[1] = renderdata.xpms.bridgeSE;
	d.pm.bridge[2] = renderdata.xpms.bridgeSW;
	d.pm.bridge[3] = renderdata.xpms.bridgeWW;
	d.pm.bridge[4] = renderdata.xpms.bridgeNW;
	d.pm.bridge[5] = renderdata.xpms.bridgeNE;

	d.pm.road1[0] = renderdata.xpms.road1EE;
	d.pm.road1[1] = renderdata.xpms.road1SE;
	d.pm.road1[2] = renderdata.xpms.road1SW;
	d.pm.road1[3] = renderdata.xpms.road1WW;
	d.pm.road1[4] = renderdata.xpms.road1NW;
	d.pm.road1[5] = renderdata.xpms.road1NE;

	d.pm.road2[0] = renderdata.xpms.road2EE;
	d.pm.road2[1] = renderdata.xpms.road2SE;
	d.pm.road2[2] = renderdata.xpms.road2SW;
	d.pm.road2[3] = renderdata.xpms.road2WW;
	d.pm.road2[4] = renderdata.xpms.road2NW;
	d.pm.road2[5] = renderdata.xpms.road2NE;

	d.pm.railr[0] = renderdata.xpms.railrEE;
	d.pm.railr[1] = renderdata.xpms.railrSE;
	d.pm.railr[2] = renderdata.xpms.railrSW;
	d.pm.railr[3] = renderdata.xpms.railrWW;
	d.pm.railr[4] = renderdata.xpms.railrNW;
	d.pm.railr[5] = renderdata.xpms.railrNE;

	for (midx=SMAP_HM_START; midx<=SMAP_HM_LAST; midx++) {
		d.pm.inf_red[midx] = renderdata.xpms.red;
		d.pm.inf_red[midx].setAlphaChannel (d.pm.hexmask[midx]);
		d.pm.inf_red[midx].setAlphaChannel (d.pm.transparent);
	}

	for (midx=SMAP_HM_START; midx<=SMAP_HM_LAST; midx++) {
		d.pm.inf_blue[midx] = renderdata.xpms.blue;
		d.pm.inf_blue[midx].setAlphaChannel (d.pm.hexmask[midx]);
		d.pm.inf_blue[midx].setAlphaChannel (d.pm.transparent);
	}

	for (midx=SMAP_HM_START; midx<=SMAP_HM_LAST; midx++) {
		d.pm.inf_neutral[midx] = renderdata.xpms.neutral;
		d.pm.inf_neutral[midx].setAlphaChannel (d.pm.hexmask[midx]);
		d.pm.inf_neutral[midx].setAlphaChannel (d.pm.transparent);
	}

	d.pm.bluedot[ 0] = renderdata.xpms.bluedot1;
	d.pm.bluedot[ 1] = renderdata.xpms.bluedot2;
	d.pm.bluedot[ 2] = renderdata.xpms.bluedot3;
	d.pm.bluedot[ 3] = renderdata.xpms.bluedot4;
	d.pm.bluedot[ 4] = renderdata.xpms.bluedot5;
	d.pm.bluedot[ 5] = renderdata.xpms.bluedot6;
	d.pm.bluedot[ 6] = renderdata.xpms.bluedot7;
	d.pm.bluedot[ 7] = renderdata.xpms.bluedot8;
	d.pm.bluedot[ 8] = renderdata.xpms.bluedot9;
	d.pm.bluedot[ 9] = renderdata.xpms.bluedot10;
	d.pm.bluedot[10] = renderdata.xpms.bluedot11;
	d.pm.bluedot[11] = renderdata.xpms.bluedot12;
	d.pm.bluedot[12] = renderdata.xpms.bluedot13;
	d.pm.bluedot[13] = renderdata.xpms.bluedot14;
	d.pm.bluedot[14] = renderdata.xpms.bluedot15;
	d.pm.bluedot[15] = renderdata.xpms.bluedot16;

	d.pm.reddot[ 0] = renderdata.xpms.reddot1;
	d.pm.reddot[ 1] = renderdata.xpms.reddot2;
	d.pm.reddot[ 2] = renderdata.xpms.reddot3;
	d.pm.reddot[ 3] = renderdata.xpms.reddot4;
	d.pm.reddot[ 4] = renderdata.xpms.reddot5;
	d.pm.reddot[ 5] = renderdata.xpms.reddot6;
	d.pm.reddot[ 6] = renderdata.xpms.reddot7;
	d.pm.reddot[ 7] = renderdata.xpms.reddot8;
	d.pm.reddot[ 8] = renderdata.xpms.reddot9;
	d.pm.reddot[ 9] = renderdata.xpms.reddot10;
	d.pm.reddot[10] = renderdata.xpms.reddot11;
	d.pm.reddot[11] = renderdata.xpms.reddot12;
	d.pm.reddot[12] = renderdata.xpms.reddot13;
	d.pm.reddot[13] = renderdata.xpms.reddot14;
	d.pm.reddot[14] = renderdata.xpms.reddot15;
	d.pm.reddot[15] = renderdata.xpms.reddot16;

	d.pm.vh_blue    = renderdata.xpms.vhexblue;
	d.pm.vh_red     = renderdata.xpms.vhexred;
	d.pm.vh_neutral = renderdata.xpms.vhexneutral;

	d.pm.frontline[0] = renderdata.xpms.frontlineEE;
	d.pm.frontline[1] = renderdata.xpms.frontlineSE;
	d.pm.frontline[2] = renderdata.xpms.frontlineSW;
	d.pm.frontline[3] = renderdata.xpms.frontlineWW;
	d.pm.frontline[4] = renderdata.xpms.frontlineNW;
	d.pm.frontline[5] = renderdata.xpms.frontlineNE;

	layer.hmap = layer.features = layer.vhex = layer.influence = layer.frontline = layer.grid = layer.dots = NULL;

	canvas = frame = map = QRect (0, 0, 0, 0);

	d.rgrid.width = d.rgrid.height = 0;
	d.rgrid.map = NULL;
}

SmapRenderer::~SmapRenderer (void)
{
	DBG_TRACE_DESTRUCT;

	cleanup();
}

void
SmapRenderer::forGrid (int marginx, int marginy, SmapHexGrid &smap)
{
	int	ix, iy;
	int	px, py, rx, ry;

	cleanup();
	if ((smap.width <= 0) || (smap.height <= 0)) return;

	d.hgrid = &smap;

	d.rgrid.width  = d.hgrid->width;
	d.rgrid.height = d.hgrid->height;

	d.rgrid.map = (SMAP_RENDERER_HEX *)malloc (d.rgrid.width * d.rgrid.height * sizeof (SMAP_RENDERER_HEX));
	if (!d.rgrid.map) {
		cleanup();
		return;
	}

	if (marginx < 1) marginx = 1;
	if (marginy < 1) marginy = 1;

	map.setLeft	(marginx);
	map.setTop	(marginy);
	map.setWidth	(d.pm_width + ((d.rgrid.width-1) * d.hmove_x) + d.vmove_x);
	map.setHeight	(d.pm_height + ((d.rgrid.height-1) * d.vmove_y));

	frame.setLeft	(map.left() - 1);
	frame.setTop	(map.top() - 1);
	frame.setWidth	(map.width() + 2);
	frame.setHeight	(map.height() + 2);

	canvas.setLeft	(0);
	canvas.setTop	(0);
	canvas.setWidth	(frame.width() + 2 * marginx);
	canvas.setHeight(frame.height() + 2 * marginy);

	px = map.x(); py = map.y(); rx = px; ry = py;
	for (iy=0; iy<d.rgrid.height; iy++) {
		for (ix=0; ix<d.rgrid.width; ix++) {
			if (ix == 0) { rx = px;	ry = py; }

			int idx = iy*d.rgrid.width+ix;
			d.rgrid.map[idx].posx = px;
			d.rgrid.map[idx].posy = py;

			px += d.hmove_x; py += d.hmove_y;
		}

		if (iy %2) {
			px = rx - d.vmove_x;
		} else {
			px = rx + d.vmove_x;
		}
		py = ry + d.vmove_y;
	}

	layer.hmap	= new QPixmap (map.size());
	layer.features	= new QPixmap (map.size());
	layer.vhex	= new QPixmap (map.size());
	layer.influence	= new QPixmap (map.size());
	layer.frontline = new QPixmap (map.size());
	layer.grid	= new QPixmap (map.size());
	layer.dots	= new QPixmap (map.size());
}

void
SmapRenderer::cleanup (void)
{
	if (layer.hmap) { delete layer.hmap; layer.hmap = NULL; }
	if (layer.features) { delete layer.features; layer.features = NULL; }
	if (layer.vhex) { delete layer.vhex; layer.vhex = NULL; }
	if (layer.influence) { delete layer.influence; layer.influence = NULL; }
	if (layer.frontline) { delete layer.frontline; layer.frontline = NULL; }
	if (layer.grid) { delete layer.grid; layer.grid = NULL; }
	if (layer.dots) { delete layer.dots; layer.dots = NULL; }

	canvas = frame = map = QRect (0, 0, 0, 0);

	if (d.rgrid.map) { free (d.rgrid.map); d.rgrid.map = NULL; }
	d.rgrid.width = d.rgrid.height = 0;

	d.hgrid = NULL;
}

void
SmapRenderer::render (void)
{
	QPainter	*paint = NULL;
	int		ix, iy, ixy, pmidx, i;
	SmapHexPos	pos;
	QPixmap		*p;
	int		idx;

	/* do nothing if there is no SmapHexGrid information */
	if (!d.hgrid) return;

	/* paint hexes */
	layer.hmap->fill (Qt::transparent);
	paint = new QPainter (layer.hmap);
	paint->setRenderHints (QPainter::Antialiasing|QPainter::HighQualityAntialiasing|QPainter::SmoothPixmapTransform, true);
	paint->setBackgroundMode (Qt::TransparentMode);
	for (iy=0; iy<d.hgrid->height; iy++) {
		for (ix=0; ix<d.hgrid->width; ix++) {
			ixy = d.hgrid->grid2idx (pos.set(ix, iy));
			pmidx = hthm2idx (d.hgrid->map[ixy].height, grid2hm (ix, iy));
			
			paint->drawPixmap (
				d.rgrid.map[ixy].posx - map.x(),
				d.rgrid.map[ixy].posy - map.y(),
				d.pm.height[pmidx]);
		}
	}
	delete paint;

	/* paint features */
	layer.features->fill (Qt::transparent);
	paint = new QPainter (layer.features);
	paint->setRenderHints (QPainter::Antialiasing|QPainter::HighQualityAntialiasing|QPainter::SmoothPixmapTransform, true);
	paint->setBackgroundMode (Qt::TransparentMode);

	/* paint water first */
	for (iy=0; iy<d.hgrid->height; iy++) {
		for (ix=0; ix<d.hgrid->width; ix++) {
			idx = d.hgrid->grid2idx (pos.set(ix, iy));

			if (d.hgrid->map[idx].water) {
				paint->drawPixmap (
					d.rgrid.map[idx].posx - map.x(),
					d.rgrid.map[idx].posy - map.y(),
					d.pm.water[grid2hm (ix, iy)]
				);
			}
		}
	}

	/* Now paint other features */
	for (iy=0; iy<d.hgrid->height; iy++) {
		for (ix=0; ix<d.hgrid->width; ix++) {
			idx = d.hgrid->grid2idx (pos.set(ix, iy));

			if (d.hgrid->map[idx].bridge) {
				for (i=0; i<=5; i++) {
					if (d.hgrid->map[idx].conn_bridge & (1<<i)) {
						paint->drawPixmap (
							d.rgrid.map[idx].posx - map.x(),
							d.rgrid.map[idx].posy - map.y(),
							d.pm.bridge[i]
						);
					}
				}

			}
			if (d.hgrid->map[idx].conn_road2) {
				for (i=0; i<=5; i++) {
					if (d.hgrid->map[idx].conn_road2 & (1<<i)) {
						paint->drawPixmap (
							d.rgrid.map[idx].posx - map.x(),
							d.rgrid.map[idx].posy - map.y(),
							d.pm.road2[i]
						);
					}
				}
			}
			if (d.hgrid->map[idx].conn_road1) {
				for (i=0; i<=5; i++) {
					if (d.hgrid->map[idx].conn_road1 & (1<<i)) {
						paint->drawPixmap (
							d.rgrid.map[idx].posx - map.x(),
							d.rgrid.map[idx].posy - map.y(),
							d.pm.road1[i]
						);
					}
				}
			}
			if (d.hgrid->map[idx].conn_railr) {
				for (i=0; i<=5; i++) {
					if (d.hgrid->map[idx].conn_railr & (1<<i)) {
						paint->drawPixmap (
							d.rgrid.map[idx].posx - map.x(),
							d.rgrid.map[idx].posy - map.y(),
							d.pm.railr[i]
						);
					}
				}
			}
		}
	}
	delete paint;

	/* paint victory hexes */
	layer.vhex->fill (Qt::transparent);
	paint = new QPainter (layer.vhex);
	paint->setRenderHints (QPainter::Antialiasing|QPainter::HighQualityAntialiasing|QPainter::SmoothPixmapTransform, true);
	paint->setBackgroundMode (Qt::TransparentMode);
	for (iy=0; iy<d.hgrid->height; iy++) {
		for (ix=0; ix<d.hgrid->width; ix++) {
			idx = d.hgrid->grid2idx (pos.set(ix, iy));
			if (!d.hgrid->map[idx].vic_hex) continue;

			switch (d.hgrid->map[idx].vic_hex_owner) {
				case SMAP_HI_BLUE:
					p = &d.pm.vh_blue;
					break;
				case SMAP_HI_RED:
					p = &d.pm.vh_red;
					break;
				case SMAP_HI_NONE:
				default:
					p = &d.pm.vh_neutral;
					break;
			}

			paint->drawPixmap (
				d.rgrid.map[idx].posx - map.x(),
				d.rgrid.map[idx].posy - map.y(),
				*p
			);
		}
	}
	delete paint;

	/* paint influence */
	layer.influence->fill (Qt::transparent);
	paint = new QPainter (layer.influence);
	paint->setRenderHints (QPainter::Antialiasing|QPainter::HighQualityAntialiasing|QPainter::SmoothPixmapTransform, true);
	paint->setBackgroundMode (Qt::TransparentMode);
	for (iy=0; iy<d.hgrid->height; iy++) {
		for (ix=0; ix<d.hgrid->width; ix++) {
			idx = d.hgrid->grid2idx (pos.set(ix, iy));

			switch (d.hgrid->map[idx].influence) {
				case SMAP_HI_BLUE:
					p = d.pm.inf_blue;
					break;
				case SMAP_HI_RED:
					p = d.pm.inf_red;
					break;
				case SMAP_HI_NONE:
				default:
					p = d.pm.inf_neutral;
					break;
			}
			paint->drawPixmap (d.rgrid.map[idx].posx - map.x(), d.rgrid.map[idx].posy - map.y(), p[grid2hm (ix, iy)]);
		}
	}
	delete paint;

	/* paint grid */
	layer.grid->fill (Qt::transparent);
	paint = new QPainter (layer.grid);
	paint->setRenderHints (QPainter::Antialiasing|QPainter::HighQualityAntialiasing|QPainter::SmoothPixmapTransform, true);
	paint->setBackgroundMode (Qt::TransparentMode);
	for (iy=0; iy<d.hgrid->height; iy++) {
		for (ix=0; ix<d.hgrid->width; ix++) {
			idx = d.hgrid->grid2idx (pos.set(ix, iy));

			paint->drawPixmap (d.rgrid.map[idx].posx - map.x(), d.rgrid.map[idx].posy - map.y(), d.pm.border);
		}
	}
	delete paint;

	/* paint frontlines */
	layer.frontline->fill (Qt::transparent);
	paint = new QPainter (layer.frontline);
	paint->setRenderHints (QPainter::Antialiasing|QPainter::HighQualityAntialiasing|QPainter::SmoothPixmapTransform, true);
	paint->setBackgroundMode (Qt::TransparentMode);
	for (iy=0; iy<d.hgrid->height; iy++) {
		for (ix=0; ix<d.hgrid->width; ix++) {
			idx = d.hgrid->grid2idx (pos.set(ix, iy));
			if (!d.hgrid->map[idx].frontline) continue;

			for (i=0; i<=5; i++) {
				if (d.hgrid->map[idx].frontline & (1<<i)) {
					paint->drawPixmap (d.rgrid.map[idx].posx - map.x(), d.rgrid.map[idx].posy - map.y(), d.pm.frontline[i]);
				}
			}

		}
	}
	delete paint;

	/* paint dots */
	layer.dots->fill (Qt::transparent);
	paint = new QPainter (layer.dots);
	paint->setRenderHints (QPainter::Antialiasing|QPainter::HighQualityAntialiasing|QPainter::SmoothPixmapTransform, true);
	paint->setBackgroundMode (Qt::TransparentMode);
	for (iy=0; iy<d.hgrid->height; iy++) {
		for (ix=0; ix<d.hgrid->width; ix++) {
			int	cnt = 0;

			idx = d.hgrid->grid2idx (pos.set(ix, iy));

			if (d.hgrid->map[idx].influence == SMAP_HI_NONE) continue;

			if ((d.hgrid->map[idx].unit_cnt_blue != 0) && (d.hgrid->map[idx].unit_cnt_red != 0)) {
				switch (d.hgrid->map[idx].influence) {
					case SMAP_HI_BLUE:
						p = d.pm.bluedot;
						cnt = d.hgrid->map[idx].unit_cnt_blue;
						break;
					case SMAP_HI_RED:
						p = d.pm.reddot;
						cnt = d.hgrid->map[idx].unit_cnt_red;
						break;
					case SMAP_HI_NONE:
					default:
						p = NULL;
						cnt = 0;
						break;
				}
			} else if (d.hgrid->map[idx].unit_cnt_blue != 0) {
				p = d.pm.bluedot;
				cnt = d.hgrid->map[idx].unit_cnt_blue;
			} else {
				p = d.pm.reddot;
				cnt = d.hgrid->map[idx].unit_cnt_red;
			}
			if (p && cnt) {
				int n = (cnt >= 16) ? 15 : (cnt-1);
				paint->drawPixmap (d.rgrid.map[idx].posx - map.x(), d.rgrid.map[idx].posy - map.y(), p[n]);
			}
		}
	}
	delete paint;
}

void
SmapRenderer::render_cursor (QPainter &painter, SmapHexPos &cursor)
{
	int	idx;

	idx = d.hgrid->grid2idx (cursor);
	if (idx >= 0) {
		painter.drawPixmap (d.rgrid.map[idx].posx, d.rgrid.map[idx].posy, d.pm.selection);
		painter.drawPixmap (d.rgrid.map[idx].posx, d.rgrid.map[idx].posy, d.pm.border);
	}
}

bool
SmapRenderer::hex2pos (SmapHexPos &pos, int &px, int &py)
{
	int	idx;

	px = py = 0;

	idx = d.hgrid->grid2idx (pos);
	if (idx < 0) return (false);

	px = d.rgrid.map[idx].posx; py = d.rgrid.map[idx].posy;
	return (true);
}

bool
SmapRenderer::pos2hex (const QPoint &pos, int &px, int &py, int &hx, int &hy)
{
	int	x, y;
	bool	complex;

	hx = hy = -1;
	if (!d.hgrid) return (false);

	px = pos.x(); py = pos.y();
	if (!map.contains (px, py)) return (false);

	px -= map.x();
	py -= map.y();

	y = py / d.vmove_y;
	complex = ((py % d.vmove_y) < d.hex_top);

	if (!complex) {
		if (y % 2) { px -= d.vmove_x; }
		x = px / d.hmove_x;
	} else {
		int	c = 0, tx, ty, ta;

		x = px / d.vmove_x;
		c |= ((x % 2) ? 1 : 0);
		c |= ((y % 2) ? 2 : 0);
		tx = (px % d.vmove_x) * d.hex_top;
		ty = (py % d.vmove_y) * d.vmove_x;
		ta = d.hex_top * d.vmove_x;
		switch (c) {
			case 0:	/* (x = even, y = even	*/
				if (ty < (ta-tx)) { x--; y--; }
				break;
			case 1:	/* x = odd, y = even	*/
				if (ty < tx) { y--; }
				break;
			case 2:	/* x = even, y = odd	*/
				if (ty < tx) { x++; y--; }
				x--;
				break;
			case 3:	/* x = odd, y = odd	*/
				if (ty < (ta-tx)) { y--; }
				break;
			default:
				x = y = -1;
				break;
		}
		if (x >= 0) x /= 2;
	}

	if ((x<0) || (x >= d.hgrid->width) || (y<0) || (y >= d.hgrid->height)) return (false);

	hx = x; hy = y;
	return (true);
}

