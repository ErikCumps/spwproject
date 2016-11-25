/** \file
 * The SPWaW war cabinet - strategic map - widget.
 *
 * Copyright (C) 2012-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "smap_widget.h"
#include "res/res.h"

/*! Strategic map: left/right margin */
#define	MARGIN_X	10

/*! Strategic map: top/bottom margin */
#define	MARGIN_Y	10

/*! Convenience macro to create a new renderer */
#define	RENDERNEW(var_,what_,err_,msg_)										\
	do {													\
		if (!(var_ = new what_))									\
			SET_GUICLS_ERROR (err_, "failed to create <" #msg_ "> renderer");			\
	} while (0)

/*! Base size for unit table model column width */
#define	BASE_SIZE	40

/*!< Base size for unit table model column width for numbers */
#define	NUMBER_SIZE	(BASE_SIZE * 4 / 3)

/*! Unit table model column IDs */
typedef enum e_UTCID {
	UTC_UID = 0,
	UTC_UNIT,
	UTC_READY,
	UTC_DMG,
	UTC_MEN,
	UTC_SUP,
	UTC_MOR,
	UTC_STATUS,
	UTC_SEEN,
	UTC_ENTR,
	UTC_LAST = UTC_ENTR	/*!< \internal */
} UTCID;

/*! A unit table model column definition */
typedef struct s_UTCDEF {
	UTCID		id;
	MDLT_COLDEF	def;
} UTCDEF;

/*! Unit table model column definitions */
static UTCDEF	unit_table_coldef[] = {
	{ UTC_UID,	{ "#",			MDLT_DATA_STR,	BASE_SIZE*3	} },
	{ UTC_UNIT,	{ "Unit",		MDLT_DATA_STR,	BASE_SIZE*3	} },
	{ UTC_READY,	{ "Ready",		MDLT_DATA_PERC,	BASE_SIZE*3	} },
	{ UTC_DMG,	{ "Damage",		MDLT_DATA_INT,	NUMBER_SIZE	} },
	{ UTC_MEN,	{ "Men",		MDLT_DATA_INT,	NUMBER_SIZE	} },
	{ UTC_MOR,	{ "Morale",		MDLT_DATA_INT,	NUMBER_SIZE	} },
	{ UTC_SUP,	{ "Suppression",	MDLT_DATA_INT,	NUMBER_SIZE	} },
	{ UTC_STATUS,	{ "Status",		MDLT_DATA_STR,	BASE_SIZE*2	} },
	{ UTC_SEEN,	{ "Spotted",		MDLT_DATA_STR,	BASE_SIZE*2	} },
	{ UTC_ENTR,	{ "Entrenchment",	MDLT_DATA_STR,	BASE_SIZE*2	} },
};

/*! Convenience function to create a MDLT_DEF for the unit table model */
static MDLT_DEF	*
build_MDLT_DEF (void)
{
	MDLT_DEF	*def = NULL;

	SL_SAFE_CALLOC (def, 1, sizeof (*def));
	def->col_cnt = ARRAYCOUNT(unit_table_coldef);

	SL_SAFE_CALLOC (def->col_lst, def->col_cnt, sizeof (*(def->col_lst)));
	for (int i=UTC_UID; i<=UTC_LAST; i++) {
		for (int j=0; j<def->col_cnt; j++) {
			if (unit_table_coldef[j].id == i) {
				def->col_lst[i] = unit_table_coldef[j].def;
				break;
			}
		}
	}

	return (def);
}

/*! Convenience function to create a MDLT_DEF for the unit table model */
void
free_MDLT_DEF (MDLT_DEF *def)
{
	if (def->col_lst) SL_SAFE_FREE (def->col_lst);
	SL_SAFE_FREE (def);
}

SmapWidget::SmapWidget (ModelTable *model, QWidget *P)
	: QFrame (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.model = model;

	d.model_def = build_MDLT_DEF();
	d.model->setup (d.model_def);
	
	setAttribute (Qt::WA_StaticContents);
	setPalette (*RES_color (RID_SMAP_BG));
	setAutoFillBackground (true);

	setMouseTracking (true);
	setFocusPolicy (Qt::WheelFocus);
	
	RENDERNEW (d.renderlist[ZOOM_1X], SmapRenderer (renderdata_11x11), ERR_GUI_SMAP_INIT_FAILED, "zoom1x");
	RENDERNEW (d.renderlist[ZOOM_2X], SmapRenderer (renderdata_21x21), ERR_GUI_SMAP_INIT_FAILED, "zoom2x");

	d.o_grid = false; 
	d.o_influence = false;
	d.o_vhex = false;
	d.o_frontline = false;

	reset_cursor();
	
	setrender (ZOOM_1X);

	SET_GUICLS_NOERR;
}

SmapWidget::~SmapWidget (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.renderlist[ZOOM_1X];
	delete d.renderlist[ZOOM_2X];
	delete[] d.b_info;
	delete[] d.r_info;

	free_MDLT_DEF (d.model_def);
}

QSize
SmapWidget::sizeHint() const
{
	return (d.rptr->canvas.size());
}

void
SmapWidget::load (SPWAW_SNAPSHOT *snap)
{
	SPWAW_SNAP_MAP_DATA	*hdata;
	SPWAW_SNAP_VHEX		*vdata;
	SPWAW_SNAP_OOB_U	*udata;
	SmapHexPos		pos;
	SmapHex			*hex;
	SmapHexPos		npos;
	int			x, y, idx;
	int			i;

	clear();

	d.grid.setup (snap->game.map.width, snap->game.map.height);

	d.renderlist[ZOOM_1X]->forGrid (MARGIN_X, MARGIN_Y, d.grid);
	d.renderlist[ZOOM_2X]->forGrid (MARGIN_X, MARGIN_Y, d.grid);

	hdata = snap->game.map.data;
	for (x=0; x<snap->game.map.width; x++) {
		for (y=0; y<snap->game.map.height; y++) {
			pos.set(x, y);
			hex = d.grid.grid2hex (pos); hex->setPos (pos);

			idx = y*snap->game.map.width+x;
			hex->setHeight	(hdata[idx].h);
			hex->setWater	(hdata[idx].water);
			hex->setBridge	(hdata[idx].bridge);
			hex->setRoadConn(hdata[idx].conn_road1, hdata[idx].conn_road2, hdata[idx].conn_rail);
		}
	}

	vdata = snap->game.battle.data.vhex;
	for (i=0; i<SPWAW_VHEXCNT; i++) {
		hex = d.grid.grid2hex (pos.set(vdata[i].x, vdata[i].y));

		hex->setVicHex (vdata[i].status);
	}

	udata = &snap->OOBp1.battle.units;
	if (udata->cnt) {
		d.b_info = new UNIT_INFO[udata->cnt];
		d.b_cnt = 0;
		for (i=0; i<udata->cnt; i++) {
			if ((udata->list[i].data.posx < 0) || (udata->list[i].data.posy < 0)) continue;
			hex = d.grid.grid2hex (d.b_info[d.b_cnt].pos.set (udata->list[i].data.posx, udata->list[i].data.posy));
			hex->addUnit (SMAP_HI_BLUE);
			d.b_info[d.b_cnt].uid		= udata->list[i].strings.uid;
			d.b_info[d.b_cnt].unit		= udata->list[i].data.type;
			d.b_info[d.b_cnt].ready		= udata->list[i].attr.gen.ready;
			d.b_info[d.b_cnt].damage	= udata->list[i].data.damage;
			d.b_info[d.b_cnt].hcnt		= udata->list[i].data.hcnt_left;
			d.b_info[d.b_cnt].morale	= udata->list[i].data.mor;
			d.b_info[d.b_cnt].suppression	= udata->list[i].data.sup;
			d.b_info[d.b_cnt].status	= udata->list[i].strings.status;
			d.b_info[d.b_cnt].spotted	= udata->list[i].data.spotted ? "spotted" : "hidden";
			d.b_info[d.b_cnt].entrenchment	= udata->list[i].strings.entr;
			d.b_cnt++;
		}
	}

	udata = &snap->OOBp2.battle.units;
	if (udata->cnt) {
		d.r_info = new UNIT_INFO[udata->cnt];
		d.r_cnt = 0;
		for (i=0; i<udata->cnt; i++) {
			if ((udata->list[i].data.posx < 0) || (udata->list[i].data.posy < 0)) continue;
			hex = d.grid.grid2hex (d.r_info[d.r_cnt].pos.set (udata->list[i].data.posx, udata->list[i].data.posy));
			hex->addUnit (SMAP_HI_RED);
			d.r_info[d.r_cnt].uid		= udata->list[i].strings.uid;
			d.r_info[d.r_cnt].unit		= udata->list[i].data.type;
			d.r_info[d.r_cnt].ready		= udata->list[i].attr.gen.ready;
			d.r_info[d.r_cnt].damage	= udata->list[i].data.damage;
			d.r_info[d.r_cnt].hcnt		= udata->list[i].data.hcnt_left;
			d.r_info[d.r_cnt].morale	= udata->list[i].data.mor;
			d.r_info[d.r_cnt].suppression	= udata->list[i].data.sup;
			d.r_info[d.r_cnt].status	= udata->list[i].strings.status;
			d.r_info[d.r_cnt].spotted	= udata->list[i].data.spotted ? "spotted" : "hidden";
			d.r_info[d.r_cnt].entrenchment	= udata->list[i].strings.entr;
			d.r_cnt++;
		}
	}

	calc_bridgecon();
	calc_influence();
	calc_frontline();
	
	d.renderlist[ZOOM_1X]->render();
	d.renderlist[ZOOM_2X]->render();

	resize (d.rptr->canvas.size());
	setMinimumSize (d.rptr->canvas.size());
	setMaximumSize (d.rptr->canvas.size());
	repaint();
}

void
SmapWidget::clear()
{
	if (d.b_cnt) {
		delete[] d.b_info; d.b_info = NULL;
		d.b_cnt = 0;
	}
	if (d.r_cnt) {
		delete[] d.r_info; d.r_info = NULL;
		d.r_cnt = 0;
	}
	d.renderlist[ZOOM_1X]->cleanup();
	d.renderlist[ZOOM_2X]->cleanup();
	d.grid.cleanup();
}

void
SmapWidget::reset_cursor (void)
{
	d.cursor.set (-1, -1);

	d.model->clear();
	d.model->commit();
}
	
void
SmapWidget::enable_grid (bool enable)
{
	if (d.o_grid != enable) {
		d.o_grid = enable;
		repaint();
	}
}

void
SmapWidget::enable_victoryhexes (bool enable)
{
	if (d.o_vhex != enable) {
		d.o_vhex = enable;
		repaint();
	}
}

void
SmapWidget::enable_influence (bool enable)
{
	if (d.o_influence != enable) {
		d.o_influence = enable;
		repaint();
	}
}

void
SmapWidget::enable_frontline (bool enable)
{
	if (d.o_frontline != enable) {
		d.o_frontline = enable;
		repaint();
	}
}

void
SmapWidget::set_zoom (ZOOMLEVEL zoom)
{
	if (d.zoom != zoom) {
		setrender (zoom);
		repaint();
	}
}

void
SmapWidget::askscroll(void)
{
	int	px, py;

	if (d.rptr->hex2pos (d.cursor, px, py))
	{
		emit wouldscroll (px, py);
	} else {
		emit wouldscroll (0, 0);
	}
}

void
SmapWidget::paintEvent (QPaintEvent *)
{
	QBrush		bb;
	QPen		bp;
		
	QPainter paint (this);

	paint.setRenderHints (QPainter::Antialiasing|QPainter::HighQualityAntialiasing|QPainter::SmoothPixmapTransform, true);

	/* paint frame */
	bb.setColor (Qt::darkGreen);
	bb.setStyle (Qt::SolidPattern);
	bp.setColor (bb.color().darker(200));
	bp.setWidth (1);

	paint.setBrush (bb);
	paint.setPen (bp);

	paint.drawRect (d.rptr->frame);

	/* paint hexes */
	paint.drawPixmap (d.rptr->map.x(), d.rptr->map.y(), *(d.rptr->layer.hmap));

	/* paint features */
	paint.drawPixmap (d.rptr->map.x(), d.rptr->map.y(), *(d.rptr->layer.features));

	/* paint victory hexes, if enabled */
	if (d.o_vhex) {
		paint.drawPixmap (d.rptr->map.x(), d.rptr->map.y(), *(d.rptr->layer.vhex));
	}

	/* paint influence, if enabled */
	if (d.o_influence) {
		paint.drawPixmap (d.rptr->map.x(), d.rptr->map.y(), *(d.rptr->layer.influence));
	}

	/* paint grid, if enabled */
	if (d.o_grid) {
		paint.drawPixmap (d.rptr->map.x(), d.rptr->map.y(), *(d.rptr->layer.grid));
	}

	/* paint frontline, if enabled */
	if (d.o_frontline) {
		paint.drawPixmap (d.rptr->map.x(), d.rptr->map.y(), *(d.rptr->layer.frontline));
	}

	/* paint dots */
	paint.drawPixmap (d.rptr->map.x(), d.rptr->map.y(), *(d.rptr->layer.dots));

	/* render cursor */
	d.rptr->render_cursor (paint, d.cursor);
}

void
SmapWidget::mouseReleaseEvent (QMouseEvent *e)
{
	int		px, py, x, y;
	SmapHexPos	pos;
	Qt::MouseButton	button;

	if (!e) return;
	if (!d.rptr->pos2hex (e->pos(), px, py, x, y)) return;
	d.hpx = px; d.hpy = py;

	button = e->button();
	switch (button) {
		case Qt::LeftButton:
			setcursor (pos.set(x,y));
			break;
		case Qt::RightButton:
			setcursor (pos.set(x,y));
			break;
		default:
			break;
	}

	repaint();
}

void
SmapWidget::contextMenuEvent (QContextMenuEvent *event)
{
	SmapHexPos	pos;
	int		idx, px, py, x, y;
	QMenu		*menu = NULL;

	if (!event) return;
	if (!d.rptr->pos2hex (event->pos(), px, py, x, y)) return;
	d.hpx = px; d.hpy = py;
	idx = d.grid.grid2idx (pos.set(x, y));

	menu = new QMenu (this);

	QString	s;
	menu->addSeparator ();
	s.sprintf ("Cursor position: (%d,%d)", px, py);
	menu->addAction (QString(s));
	menu->addSeparator ();
	s.sprintf ("Hex position: (%d, %d)", x, y);
	menu->addAction (QString(s));
	s.sprintf ("Hex height  : %d meter", d.grid.map[idx].actheight);
	menu->addAction (QString(s));
	menu->addSeparator ();
	s.sprintf ("Unit count: %d blue unit(s)", d.grid.map[idx].unit_cnt_blue);
	menu->addAction (QString(s));
	s.sprintf ("Unit count: %d red unit(s)", d.grid.map[idx].unit_cnt_red);
	menu->addAction (QString(s));
	menu->addSeparator ();
	s.sprintf ("Influence       : %d", d.grid.map[idx].influence);
	menu->addAction (QString(s));
	s.sprintf ("Influence values: %.6f/%.6f", d.grid.map[idx].influence_blue, d.grid.map[idx].influence_red);
	menu->addAction (QString(s));
	s.sprintf ("Influence counts: %d/%d", d.grid.map[idx].influence_blue_cnt, d.grid.map[idx].influence_red_cnt);
	menu->addAction (QString(s));
			
	menu->exec(event->globalPos());

	delete menu;
} 

void
SmapWidget::calc_bridgecon (void)
{
	SmapHexPos	pos;
	SmapHex		*hex, *nhex;
	SmapHexPos	npos;
	int		ix, iy;

	for (iy=0; iy<d.grid.height; iy++) {
		for (ix=0; ix<d.grid.width; ix++) {
			hex = d.grid.grid2hex (ix, iy);
			if (!hex->bridge) continue;

			hex->conn_bridge = 0;
			if (d.grid.hexEE (hex->pos, npos)) {
				nhex = d.grid.grid2hex (npos);
				if (nhex->hasBridge() || nhex->hasRoad (SMAP_HRD_EE))
					hex->conn_bridge |= SMAP_HRD_mask[SMAP_HRD_EE];
			}
			if (d.grid.hexSE (hex->pos, npos)) {
				nhex = d.grid.grid2hex (npos);
				if (nhex->hasBridge() || nhex->hasRoad (SMAP_HRD_SE))
					hex->conn_bridge |= SMAP_HRD_mask[SMAP_HRD_SE];
			}
			if (d.grid.hexSW (hex->pos, npos)) {
				nhex = d.grid.grid2hex (npos);
				if (nhex->hasBridge() || nhex->hasRoad (SMAP_HRD_SW))
					hex->conn_bridge |= SMAP_HRD_mask[SMAP_HRD_SW];
			}
			if (d.grid.hexWW (hex->pos, npos)) {
				nhex = d.grid.grid2hex (npos);
				if (nhex->hasBridge() || nhex->hasRoad (SMAP_HRD_WW))
					hex->conn_bridge |= SMAP_HRD_mask[SMAP_HRD_WW];
			}
			if (d.grid.hexNW (hex->pos, npos)) {
				nhex = d.grid.grid2hex (npos);
				if (nhex->hasBridge() || nhex->hasRoad (SMAP_HRD_NW))
					hex->conn_bridge |= SMAP_HRD_mask[SMAP_HRD_NW];
			}
			if (d.grid.hexNE (hex->pos, npos)) {
				nhex = d.grid.grid2hex (npos);
				if (nhex->hasBridge() || nhex->hasRoad (SMAP_HRD_NE))
					hex->conn_bridge |= SMAP_HRD_mask[SMAP_HRD_NE];
			}
		}
	}
}

static double
distance (int c1x, int c1y, int c2x, int c2y)
{
	int	dx, dy;
	double	d = 0.0;

	dx = c2x - c1x; dy = c2y - c1y;
	d = (double)(dx*dx + dy*dy);

	return (sqrt(d));
}

#define	HEIGHTWEIGHT	1

static inline double
height_weight (SMAP_HH tgt_height, SMAP_HH src_height)
{
#if	!HEIGHTWEIGHT
	return (1.0);
#else	/* HEIGHTWEIGHT */
	static int range = SMAP_HH_LAST - SMAP_HH_START;
	static double scale = 0.75;

	int delta = src_height - tgt_height;
	return (1.0 + ((double)delta / (double)range * scale));
#endif	/* HEIGHTWEIGHT */
}

//#define	DISTANCE_THRESHOLD	0.0
#define	DISTANCE_THRESHOLD	1000.0
#define	DISTANCE_SCALEFACTOR	1.0
#define	LOCAL_INFLUENCE		10.0

void
SmapWidget::calc_influence (void)
{
	SmapHexPos	pos;
	int		ix, iy, idx, i;
	int		mcx, mcy, rcx, rcy;
	double		dv, iv, mbd, mrd;

	for (iy=0; iy<d.grid.height; iy++) {
		for (ix=0; ix<d.grid.width; ix++) {
			idx = d.grid.grid2idx (pos.set(ix, iy));

			d.grid.grid2coord (d.grid.map[idx].pos, mcx, mcy);

			d.grid.map[idx].influence_blue = 0.0; d.grid.map[idx].influence_blue_cnt = 0; mbd = 0.0;
			for (i=0; i<d.b_cnt; i++) {
				d.grid.grid2coord (d.b_info[i].pos, rcx, rcy);
				if ((rcx == mcx) && (rcy == mcy)) {
					dv = 0.0;
					iv = LOCAL_INFLUENCE;
				} else {
					dv = distance (mcx, mcy, rcx, rcy);
					iv = LOCAL_INFLUENCE / dv / dv;
				}
				iv *= d.b_info[i].ready;
				iv *= height_weight (d.grid.map[idx].height, d.grid.grid2hex (d.b_info[i].pos)->height);
				if (dv < DISTANCE_THRESHOLD) {
					d.grid.map[idx].influence_blue += iv;
					d.grid.map[idx].influence_blue_cnt++;
				} else {
					iv *= DISTANCE_SCALEFACTOR;
					if (iv > mbd) mbd = iv;
				}
			}
			if (!d.grid.map[idx].influence_blue_cnt) {
				d.grid.map[idx].influence_blue_cnt = -1;
				d.grid.map[idx].influence_blue = mbd;
			}
			if (d.grid.map[idx].influence_blue < mbd) {
				d.grid.map[idx].influence_blue = mbd;
			}

			d.grid.map[idx].influence_red = 0.0; d.grid.map[idx].influence_red_cnt = 0; mrd = 0.0;
			for (i=0; i<d.r_cnt; i++) {
				d.grid.grid2coord (d.r_info[i].pos, rcx, rcy);
				if ((rcx == mcx) && (rcy == mcy)) {
					dv = 0.0;
					iv = LOCAL_INFLUENCE;
				} else {
					dv = distance (mcx, mcy, rcx, rcy);
					iv = LOCAL_INFLUENCE / dv / dv;
				}
				iv *= d.r_info[i].ready;
				iv *= height_weight (d.grid.map[idx].height, d.grid.grid2hex (d.r_info[i].pos)->height);
				if (dv < DISTANCE_THRESHOLD) {
					d.grid.map[idx].influence_red += iv;
					d.grid.map[idx].influence_red_cnt++;
				} else {
					iv *= DISTANCE_SCALEFACTOR;
					if (iv > mrd) mrd = iv;
				}
			}
			if (!d.grid.map[idx].influence_red_cnt) {
				d.grid.map[idx].influence_red_cnt = -1;
				d.grid.map[idx].influence_red = mrd;
			}
			if (d.grid.map[idx].influence_red < mrd) {
				d.grid.map[idx].influence_red = mrd;
			}

			if ((d.grid.map[idx].influence_blue_cnt > 0) && (d.grid.map[idx].influence_red_cnt > 0)) {
				if (d.grid.map[idx].influence_blue > d.grid.map[idx].influence_red) {
					d.grid.map[idx].influence = SMAP_HI_BLUE;
				} else if (d.grid.map[idx].influence_blue < d.grid.map[idx].influence_red) {
					d.grid.map[idx].influence = SMAP_HI_RED;
				} else if (d.grid.map[idx].influence_blue_cnt > d.grid.map[idx].influence_red_cnt) {
					d.grid.map[idx].influence = SMAP_HI_BLUE;
				} else if (d.grid.map[idx].influence_blue_cnt < d.grid.map[idx].influence_red_cnt) {
					d.grid.map[idx].influence = SMAP_HI_RED;
				} else {
					d.grid.map[idx].influence = SMAP_HI_BLUE;
				}
			} else if (d.grid.map[idx].influence_blue_cnt > 0) {
				if (d.grid.map[idx].influence_blue > d.grid.map[idx].influence_red) {
					d.grid.map[idx].influence = SMAP_HI_BLUE;
				} else {
					d.grid.map[idx].influence = SMAP_HI_RED;
				}
			} else if (d.grid.map[idx].influence_red_cnt > 0) {
				if (d.grid.map[idx].influence_red > d.grid.map[idx].influence_blue) {
					d.grid.map[idx].influence = SMAP_HI_RED;
				} else {
					d.grid.map[idx].influence = SMAP_HI_BLUE;
				}
			} else if(d.grid.map[idx].influence_blue >= d.grid.map[idx].influence_red) {
				d.grid.map[idx].influence = SMAP_HI_BLUE;
			} else {
				d.grid.map[idx].influence = SMAP_HI_RED;
			}
		}
	}
}

void
SmapWidget::calc_frontline (void)
{
	SmapHexPos	pos;
	SmapHex	*hex, *nhex;
	SmapHexPos	npos;
	int	ix, iy;

	for (iy=0; iy<d.grid.height; iy++) {
		for (ix=0; ix<d.grid.width; ix++) {
			hex = d.grid.grid2hex (ix, iy);

			hex->frontline = 0;
			if (d.grid.hexEE (hex->pos, npos)) {
				nhex = d.grid.grid2hex (npos);
				if (hex->influence != nhex->influence) hex->frontline |= SMAP_HRD_mask[SMAP_HRD_EE];
			}
			if (d.grid.hexSE (hex->pos, npos)) {
				nhex = d.grid.grid2hex (npos);
				if (hex->influence != nhex->influence) hex->frontline |= SMAP_HRD_mask[SMAP_HRD_SE];
			}
			if (d.grid.hexSW (hex->pos, npos)) {
				nhex = d.grid.grid2hex (npos);
				if (hex->influence != nhex->influence) hex->frontline |= SMAP_HRD_mask[SMAP_HRD_SW];
			}
			if (d.grid.hexWW (hex->pos, npos)) {
				nhex = d.grid.grid2hex (npos);
				if (hex->influence != nhex->influence) hex->frontline |= SMAP_HRD_mask[SMAP_HRD_WW];
			}
			if (d.grid.hexNW (hex->pos, npos)) {
				nhex = d.grid.grid2hex (npos);
				if (hex->influence != nhex->influence) hex->frontline |= SMAP_HRD_mask[SMAP_HRD_NW];
			}
			if (d.grid.hexNE (hex->pos, npos)) {
				nhex = d.grid.grid2hex (npos);
				if (hex->influence != nhex->influence) hex->frontline |= SMAP_HRD_mask[SMAP_HRD_NE];
			}
		}
	}
}

void
SmapWidget::setcursor (SmapHexPos &pos)
{
	int		idx;
	int		rows;
	MDLT_DATA	data;
	int		i;

	idx = d.grid.grid2idx (pos);
	if (idx < 0) return;

	d.cursor = pos;

	rows = d.grid.map[idx].unit_cnt_blue + d.grid.map[idx].unit_cnt_red;

	MDLT_alloc_data (data, rows, d.model->columnCount());

	idx = 0;
	for (i=0; i<d.b_cnt; i++) {
		if (d.b_info[i].pos != d.cursor) continue;
		data.row[idx].bg = *(RES_color(RID_RGB_PLAYER_BG));
		data.row[idx].data[UTC_UID]	= d.b_info[i].uid;
		data.row[idx].data[UTC_UNIT]	= d.b_info[i].unit;
		data.row[idx].data[UTC_READY]	= d.b_info[i].ready;
		data.row[idx].data[UTC_DMG]	= d.b_info[i].damage;
		data.row[idx].data[UTC_MEN]	= d.b_info[i].hcnt;
		data.row[idx].data[UTC_MOR]	= d.b_info[i].morale;
		data.row[idx].data[UTC_SUP]	= d.b_info[i].suppression;
		data.row[idx].data[UTC_STATUS]	= d.b_info[i].status;
		data.row[idx].data[UTC_SEEN]	= d.b_info[i].spotted;
		data.row[idx].data[UTC_ENTR]	= d.b_info[i].entrenchment;
		idx++;
	}
	for (i=0; i<d.r_cnt; i++) {
		if (d.r_info[i].pos != d.cursor) continue;
		data.row[idx].bg = *(RES_color(RID_RGB_OPPONENT_BG));
		data.row[idx].data[UTC_UID]	= d.r_info[i].uid;
		data.row[idx].data[UTC_UNIT]	= d.r_info[i].unit;
		data.row[idx].data[UTC_READY]	= d.r_info[i].ready;
		data.row[idx].data[UTC_DMG]	= d.r_info[i].damage;
		data.row[idx].data[UTC_MEN]	= d.r_info[i].hcnt;
		data.row[idx].data[UTC_MOR]	= d.r_info[i].morale;
		data.row[idx].data[UTC_SUP]	= d.r_info[i].suppression;
		data.row[idx].data[UTC_STATUS]	= d.r_info[i].status;
		data.row[idx].data[UTC_SEEN]	= d.r_info[i].spotted;
		data.row[idx].data[UTC_ENTR]	= d.r_info[i].entrenchment;
		idx++;
	}

	d.model->load (&data);
	MDLT_free_data (data);

	d.model->commit();
}

void
SmapWidget::setrender (ZOOMLEVEL zoom)
{
	d.zoom = zoom;
	d.rptr = d.renderlist[d.zoom];

	QSize mapsize = d.rptr->canvas.size();
	mapsize.rheight() += 200;

	setMinimumSize (sizeHint());
	setMaximumSize (sizeHint());
}
