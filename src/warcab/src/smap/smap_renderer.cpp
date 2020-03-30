/** \file
 * The SPWaW war cabinet - strategic map - rendering.
 *
 * Copyright (C) 2012-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "smap_renderer.h"
#include "dbg/dbg.h"

SmapRenderer::SmapRenderer (SMAP_RENDERDATA &renderdata)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.desc = renderdata.desc;

	d.pm_width = renderdata.width; d.pm_height = renderdata.height;
	
	d.hex_side = renderdata.side;
	d.hex_top = ((d.pm_height - 1) - (d.hex_side - 1)) / 2;

	d.hmove_x = d.pm_width - 1;
	d.hmove_y = 0;

	d.vmove_x = (d.pm_width - 1) / 2;
	d.vmove_y = d.hex_top + ((d.pm_height - 1) / 2) - 1;

	d.rd = &renderdata;
	d.hpmc = SMAP_RENDERDATA_hpmc (*d.rd, SPWAW_GAME_TYPE_SPWAW, SPWAW_TUNKNOWN);

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

const char *
SmapRenderer::description (void)
{
	return (d.desc);
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
	if (!d.rgrid.map) goto handle_oom;

	if (marginx < 1) marginx = 1;
	if (marginy < 1) marginy = 1;

	map.setLeft	(marginx);
	map.setTop	(marginy);
	map.setWidth	(d.pm_width + ((d.rgrid.width-1) * d.hmove_x) + d.vmove_x);
	map.setHeight	(d.pm_height + ((d.rgrid.height-1) * d.vmove_y));

	layer.hmap	= new QPixmap (map.size()); if (!layer.hmap) goto handle_oom;
	layer.features	= new QPixmap (map.size()); if (!layer.features) goto handle_oom;
	layer.vhex	= new QPixmap (map.size()); if (!layer.vhex) goto handle_oom;
	layer.influence	= new QPixmap (map.size()); if (!layer.influence) goto handle_oom;
	layer.frontline = new QPixmap (map.size()); if (!layer.frontline) goto handle_oom;
	layer.grid	= new QPixmap (map.size()); if (!layer.grid) goto handle_oom;
	layer.dots	= new QPixmap (map.size()); if (!layer.dots) goto handle_oom;

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


	return;

handle_oom:
	cleanup();
}

void
SmapRenderer::selectHCF (SPWAW_GAME_TYPE gametype, SPWAW_TERRAIN terrain)
{
	d.hpmc = SMAP_RENDERDATA_hpmc (*d.rd, gametype, terrain);
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
			pmidx = SMAP_hthm2idx (*d.hpmc, d.hgrid->map[ixy].height, grid2hm (ix, iy));
			
			paint->drawPixmap (
				d.rgrid.map[ixy].posx - map.x(),
				d.rgrid.map[ixy].posy - map.y(),
				d.hpmc->pixmaps[pmidx]);
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
					d.rd->pmc.water[grid2hm (ix, iy)]
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
							d.rd->pmc.bridge[i]
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
							d.rd->pmc.road2[i]
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
							d.rd->pmc.road1[i]
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
							d.rd->pmc.railr[i]
						);
					}
				}
			}
			if (d.hgrid->map[idx].conn_traml) {
				for (i=0; i<=5; i++) {
					if (d.hgrid->map[idx].conn_traml & (1<<i)) {
						paint->drawPixmap (
							d.rgrid.map[idx].posx - map.x(),
							d.rgrid.map[idx].posy - map.y(),
							d.rd->pmc.traml[i]
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
					p = &d.rd->pmc.vh_blue;
					break;
				case SMAP_HI_RED:
					p = &d.rd->pmc.vh_red;
					break;
				case SMAP_HI_NONE:
				default:
					p = &d.rd->pmc.vh_neutral;
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
					p = d.rd->pmc.inf_blue;
					break;
				case SMAP_HI_RED:
					p = d.rd->pmc.inf_red;
					break;
				case SMAP_HI_NONE:
				default:
					p = d.rd->pmc.inf_neutral;
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

			paint->drawPixmap (d.rgrid.map[idx].posx - map.x(), d.rgrid.map[idx].posy - map.y(), d.rd->pmc.border);
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
					paint->drawPixmap (d.rgrid.map[idx].posx - map.x(), d.rgrid.map[idx].posy - map.y(), d.rd->pmc.frontline[i]);
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
						p = d.rd->pmc.bluedot;
						cnt = d.hgrid->map[idx].unit_cnt_blue;
						break;
					case SMAP_HI_RED:
						p = d.rd->pmc.reddot;
						cnt = d.hgrid->map[idx].unit_cnt_red;
						break;
					case SMAP_HI_NONE:
					default:
						p = NULL;
						cnt = 0;
						break;
				}
			} else if (d.hgrid->map[idx].unit_cnt_blue != 0) {
				p = d.rd->pmc.bluedot;
				cnt = d.hgrid->map[idx].unit_cnt_blue;
			} else {
				p = d.rd->pmc.reddot;
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
		painter.drawPixmap (d.rgrid.map[idx].posx, d.rgrid.map[idx].posy, d.rd->pmc.selection);
		painter.drawPixmap (d.rgrid.map[idx].posx, d.rgrid.map[idx].posy, d.rd->pmc.border);
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

