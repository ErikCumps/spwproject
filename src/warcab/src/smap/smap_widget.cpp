/** \file
 * The SPWaW war cabinet - strategic map - widget.
 *
 * Copyright (C) 2012-2021 Erik Cumps <erik.cumps@gmail.com>
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
#define	NUMBER_SIZE	(BASE_SIZE * 5 / 3)

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
	UTC_CONTACT,
	UTC_LAST = UTC_CONTACT	/*!< \internal */
} UTCID;

/*! A unit table model column definition */
typedef struct s_UTCDEF {
	UTCID		id;
	MDLT_COLDEF	def;
} UTCDEF;

/*! Unit table model column definitions */
static UTCDEF	unit_table_coldef[] = {
	{ UTC_UID,	{ "#",			MDLT_DATA_STR,	BASE_SIZE	} },
	{ UTC_UNIT,	{ "Unit",		MDLT_DATA_STR,	(BASE_SIZE * 3)	} },
	{ UTC_READY,	{ "Ready",		MDLT_DATA_PERC,	(BASE_SIZE * 3)	} },
	{ UTC_DMG,	{ "Damage",		MDLT_DATA_INT,	NUMBER_SIZE	} },
	{ UTC_MEN,	{ "Men",		MDLT_DATA_INT,	NUMBER_SIZE	} },
	{ UTC_MOR,	{ "Morale",		MDLT_DATA_INT,	NUMBER_SIZE	} },
	{ UTC_SUP,	{ "Suppression",	MDLT_DATA_INT,	NUMBER_SIZE	} },
	{ UTC_STATUS,	{ "Status",		MDLT_DATA_STR,	(BASE_SIZE * 3)	} },
	{ UTC_SEEN,	{ "Spotted",		MDLT_DATA_STR,	(BASE_SIZE * 2)	} },
	{ UTC_ENTR,	{ "Entrenchment",	MDLT_DATA_STR,	(BASE_SIZE * 2)	} },
	{ UTC_CONTACT,	{ "Contact",		MDLT_DATA_STR,	(BASE_SIZE * 2)	} },
};

/*! Convenience function to create a MDLT_DEF for the unit table model */
static MDLT_DEF	*
build_MDLT_DEF (void)
{
	MDLT_DEF	*def = NULL;

	SL_SAFE_CALLOC (def, 1, sizeof (*def)); // FIXME: def could be NULL in case of OOM!
	def->col_cnt = ARRAYCOUNT(unit_table_coldef);

	SL_SAFE_CALLOC (def->col_lst, def->col_cnt, sizeof (*(def->col_lst))); // FIXME: def->col_lst could be NULL in case of OOM!
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

	SMAP_RENDERDATA_create (d.rdlist[ZOOM_1X], "zoom1x", 11);
	RENDERNEW (d.renderlist[ZOOM_1X], SmapRenderer (d.rdlist[ZOOM_1X]), ERR_GUI_SMAP_INIT_FAILED, "zoom1x");

	SMAP_RENDERDATA_create (d.rdlist[ZOOM_2X], "zoom2x", 21);
	RENDERNEW (d.renderlist[ZOOM_2X], SmapRenderer (d.rdlist[ZOOM_2X]), ERR_GUI_SMAP_INIT_FAILED, "zoom2x");

	d.comp_cfg.grid		= false;
	d.comp_cfg.influence	= false;
	d.comp_cfg.vhex		= false;
	d.comp_cfg.frontline	= false;

	reset_cursor();
	
	setrender (ZOOM_1X);

	SET_GUICLS_NOERR;
}

SmapWidget::~SmapWidget (void)
{
	DBG_TRACE_DESTRUCT;

	clear();

	// QT deletes child widgets
	delete d.renderlist[ZOOM_1X];
	delete d.renderlist[ZOOM_2X];
	SMAP_RENDERDATA_destroy (d.rdlist[ZOOM_1X]);
	SMAP_RENDERDATA_destroy (d.rdlist[ZOOM_2X]);

	free_MDLT_DEF (d.model_def);
}

QSize
SmapWidget::sizeHint() const
{
	return (d.rptr->canvas.size());
}

void
SmapWidget::load (SPWAW_BTURN *turn, SMAP_HPMC_TYPE type)
{
	clear();
	if (!turn) return;

	d.comp_cfg.hcf = type;

	SL_SAFE_STRDUP (d.battle.location, turn->battle->location);
	d.battle.date = turn->tdate;
	d.battle.gametype = turn->snap->gametype;
	d.battle.terrain = turn->snap->game.battle.data.terrain;
	d.battle.max_height = SMAP_gametype2maxheight (d.battle.gametype);

	load_info_full(turn);
	load_info_lmtd(turn);
	load_info_none(turn);

	apply_intelmode(true);
	applycursor();
}

void
SmapWidget::clear()
{
	d.renderlist[ZOOM_1X]->cleanup();
	d.renderlist[ZOOM_2X]->cleanup();

	clear_grid_info (d.info_full);
	clear_grid_info (d.info_lmtd);
	clear_grid_info (d.info_none);

	SL_SAFE_FREE (d.battle.location);
}

void
SmapWidget::reset_cursor (void)
{
	d.cursor.set (-1, -1);

	d.model->clear();
	d.model->commit();
}
	
void
SmapWidget::enable_grid (bool enable, bool paint)
{
	if (d.comp_cfg.grid != enable) {
		d.comp_cfg.grid = enable;
		if (paint) repaint();
	}
}

void
SmapWidget::enable_victoryhexes (bool enable, bool paint)
{
	if (d.comp_cfg.vhex != enable) {
		d.comp_cfg.vhex = enable;
		if (paint) repaint();
	}
}

void
SmapWidget::enable_influence (bool enable, bool paint)
{
	if (d.comp_cfg.influence != enable) {
		d.comp_cfg.influence = enable;
		if (paint) repaint();
	}
}

void
SmapWidget::enable_frontline (bool enable, bool paint)
{
	if (d.comp_cfg.frontline != enable) {
		d.comp_cfg.frontline = enable;
		if (paint) repaint();
	}
}

void
SmapWidget::select_hcf (SMAP_HPMC_TYPE type, bool paint)
{
	if (d.comp_cfg.hcf != type) {
		d.comp_cfg.hcf = type;
		d.renderlist[ZOOM_1X]->selectHCF (d.comp_cfg.hcf, d.battle.gametype, d.battle.terrain);
		d.renderlist[ZOOM_1X]->render();
		d.renderlist[ZOOM_2X]->selectHCF (d.comp_cfg.hcf, d.battle.gametype, d.battle.terrain);
		d.renderlist[ZOOM_2X]->render();
		if (paint) repaint();
	}
}

void
SmapWidget::set_intel_mode (INTEL_MODE mode, bool paint)
{
	d.intel_mode = mode;

	apply_intelmode (paint);
	applycursor();
}

void
SmapWidget::set_zoom (ZOOMLEVEL zoom, bool paint)
{
	if (d.zoom != zoom) {
		setrender (zoom);
		if (paint) repaint();
	}
}

void
SmapWidget::trigger_repaint (void)
{
	repaint();
}

void
SmapWidget::askscroll (void)
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
	QPainter	paint (this);
	COMPOSITOR_CFG	cfg = d.comp_cfg;

	/* Composite the image */
	composite (paint, cfg);

	/* Render the cursor */
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
	idx = d.info->grid.grid2idx (pos.set(x, y));

	menu = new QMenu (this);

	QString	s;
	menu->addSeparator ();
	s.sprintf ("Cursor position: (%d,%d)", px, py);
	menu->addAction (QString(s));

	menu->addSeparator ();
	s.sprintf ("Hex position: (%d, %d)", x, y);
	menu->addAction (QString(s));
	s.sprintf ("Hex height: %d meter (bin #%d)", d.info->grid.map[idx].actheight, d.info->grid.map[idx].height);
	menu->addAction (QString(s));
	if (d.info->grid.map[idx].water) {
		s.sprintf ("* has water");
		menu->addAction (QString(s));
	}
	if (d.info->grid.map[idx].bridge) {
		s.sprintf ("* has bridge");
		menu->addAction (QString(s));
	}
	if (d.info->grid.map[idx].conn_road1) {
		s.sprintf ("* has primary road");
		menu->addAction (QString(s));
	}
	if (d.info->grid.map[idx].conn_road2) {
		s.sprintf ("* has secondary road");
		menu->addAction (QString(s));
	}
	if (d.info->grid.map[idx].conn_railr) {
		s.sprintf ("* has railroad");
		menu->addAction (QString(s));
	}
	if (d.info->grid.map[idx].conn_traml) {
		s.sprintf ("* has tramline");
		menu->addAction (QString(s));
	}

	menu->addSeparator ();
	if (d.info->grid.map[idx].vic_hex) {
		switch (d.info->grid.map[idx].vic_hex_owner) {
			case SMAP_HI_BLUE:
				s.sprintf ("Victory hex owner: blue");
				break;
			case SMAP_HI_RED:
				s.sprintf ("Victory hex owner: red");
				break;
			case SMAP_HI_NONE:
			default:
				s.sprintf ("Victory hex owner: neutral");
				break;
		}
		menu->addAction (QString(s));
	}

	menu->addSeparator ();
	s.sprintf ("Unit count: %d blue unit(s)", d.info->grid.map[idx].unit_cnt_blue);
	menu->addAction (QString(s));
	s.sprintf ("Unit count: %d red unit(s)", d.info->grid.map[idx].unit_cnt_red);
	menu->addAction (QString(s));

	menu->addSeparator ();
	switch (d.info->grid.map[idx].influence) {
		case SMAP_HI_BLUE:
			s.sprintf ("Influence: blue");
			break;
		case SMAP_HI_RED:
			s.sprintf ("Influence: red");
			break;
		case SMAP_HI_NONE:
		default:
			s.sprintf ("Influence: contested");
			break;
	}
	menu->addAction (QString(s));
	s.sprintf ("Influence values: %.6f/%.6f (blue/red)", d.info->grid.map[idx].influence_blue, d.info->grid.map[idx].influence_red);
	menu->addAction (QString(s));
	s.sprintf ("Influence counts: %d/%d (blue/red)", d.info->grid.map[idx].influence_blue_cnt, d.info->grid.map[idx].influence_red_cnt);
	menu->addAction (QString(s));

	menu->exec(event->globalPos());

	delete menu;
}

void
SmapWidget::clear_grid_info (GRID_INFO &info)
{
	if (info.b_cnt) {
		delete[] info.b_info; info.b_info = NULL;
		info.b_cnt = 0;
	}
	if (info.r_cnt) {
		delete[] info.r_info; info.r_info = NULL;
		info.r_cnt = 0;
	}
	info.grid.cleanup();
}

void
SmapWidget::apply_intelmode (bool paint)
{
	switch (d.intel_mode) {
		case INTEL_MODE_FULL:
		default:
			d.info = &d.info_full;
			break;
		case INTEL_MODE_LMTD:
			d.info = &d.info_lmtd;
			break;
		case INTEL_MODE_NONE:
			d.info = &d.info_none;
			break;
	}

	d.renderlist[ZOOM_1X]->forGrid (MARGIN_X, MARGIN_Y, d.info->grid);
	d.renderlist[ZOOM_1X]->selectHCF (d.comp_cfg.hcf, d.battle.gametype, d.battle.terrain);
	d.renderlist[ZOOM_1X]->render();

	d.renderlist[ZOOM_2X]->forGrid (MARGIN_X, MARGIN_Y, d.info->grid);
	d.renderlist[ZOOM_2X]->selectHCF (d.comp_cfg.hcf, d.battle.gametype, d.battle.terrain);
	d.renderlist[ZOOM_2X]->render();

	resize (d.rptr->canvas.size());
	setMinimumSize (d.rptr->canvas.size());
	setMaximumSize (d.rptr->canvas.size());
	if (paint) repaint();
}

void
SmapWidget::load_info_full (SPWAW_BTURN *turn)
{
	SPWAW_SNAP_MAP_DATA	*hdata;
	SPWAW_SNAP_VHEX		*vdata;
	SPWAW_SNAP_OOB_U	*udata;
	SmapHexPos		pos;
	SmapHex			*hex;
	int			x, y, idx;
	int			i;
	bool			kia;

	GRID_INFO *info = &d.info_full;

	info->grid.setup (turn->snap->game.map.width, turn->snap->game.map.height);

	hdata = turn->snap->game.map.data;
	for (x=0; x<info->grid.width; x++) {
		for (y=0; y<info->grid.height; y++) {
			pos.set(x, y);
			hex = info->grid.grid2hex (pos); hex->setPos (pos);

			idx = y*info->grid.width+x;
			hex->setHeight	(hdata[idx].h);
			hex->setWater	(hdata[idx].water);
			hex->setBridge	(hdata[idx].bridge);
			hex->setRoadConn(hdata[idx].conn_road1, hdata[idx].conn_road2, hdata[idx].conn_rail, hdata[idx].conn_tram);
		}
	}

	vdata = turn->snap->game.battle.data.vhex;
	for (i=0; i<SPWAW_VHEXCNT; i++) {
		hex = info->grid.grid2hex (pos.set(vdata[i].x, vdata[i].y));

		hex->setVicHex (vdata[i].status, INTEL_MODE_FULL);
	}

	udata = &turn->snap->OOBp1.battle.units;
	if (udata->cnt) {
		info->b_info = new UNIT_INFO[udata->cnt];
		info->b_cnt = 0;
		for (i=0; i<udata->cnt; i++) {
			if ((udata->list[i].data.posx < 0) || (udata->list[i].data.posy < 0)) continue;
			kia = (!udata->list[i].data.alive || (udata->list[i].data.aband == SPWAW_ASTAY));
			hex = info->grid.grid2hex (info->b_info[info->b_cnt].pos.set (udata->list[i].data.posx, udata->list[i].data.posy));
			hex->addUnit (SMAP_HI_BLUE);
			if (!kia) hex->all_KIA_blue = false;
			info->b_info[info->b_cnt].uid		= udata->list[i].strings.uid;
			info->b_info[info->b_cnt].unit		= udata->list[i].data.dname; // FIXME: designation, actually
			info->b_info[info->b_cnt].ready		= udata->list[i].attr.gen.ready;
			info->b_info[info->b_cnt].damage	= udata->list[i].data.damage;
			info->b_info[info->b_cnt].hcnt		= udata->list[i].data.hcnt_left;
			info->b_info[info->b_cnt].morale	= udata->list[i].data.mor;
			info->b_info[info->b_cnt].suppression	= udata->list[i].data.sup;
			info->b_info[info->b_cnt].status	= udata->list[i].strings.status;
			info->b_info[info->b_cnt].spotted	= udata->list[i].data.spotted ? "spotted" : "hidden";
			info->b_info[info->b_cnt].entrenchment	= udata->list[i].strings.entr;
			info->b_info[info->b_cnt].contact	= udata->list[i].strings.contact;
			info->b_info[info->b_cnt].KIA		= kia;
			info->b_cnt++;
		}
		/* Clean up if no units found */
		if (!info->b_cnt) { delete[] info->b_info; info->b_info = NULL; }
	}

	udata = &turn->snap->OOBp2.battle.units;
	if (udata->cnt) {
		info->r_info = new UNIT_INFO[udata->cnt];
		info->r_cnt = 0;
		for (i=0; i<udata->cnt; i++) {
			if ((udata->list[i].data.posx < 0) || (udata->list[i].data.posy < 0)) continue;
			kia = (!udata->list[i].data.alive || (udata->list[i].data.aband == SPWAW_ASTAY));
			hex = info->grid.grid2hex (info->r_info[info->r_cnt].pos.set (udata->list[i].data.posx, udata->list[i].data.posy));
			hex->addUnit (SMAP_HI_RED);
			if (!kia) hex->all_KIA_red = false;
			info->r_info[info->r_cnt].uid		= udata->list[i].strings.uid;
			info->r_info[info->r_cnt].unit		= udata->list[i].data.dname; // FIXME: designation, actually
			info->r_info[info->r_cnt].ready		= udata->list[i].attr.gen.ready;
			info->r_info[info->r_cnt].damage	= udata->list[i].data.damage;
			info->r_info[info->r_cnt].hcnt		= udata->list[i].data.hcnt_left;
			info->r_info[info->r_cnt].morale	= udata->list[i].data.mor;
			info->r_info[info->r_cnt].suppression	= udata->list[i].data.sup;
			info->r_info[info->r_cnt].status	= udata->list[i].strings.status;
			info->r_info[info->r_cnt].spotted	= udata->list[i].data.spotted ? "spotted" : "hidden";
			info->r_info[info->r_cnt].entrenchment	= udata->list[i].strings.entr;
			info->r_info[info->r_cnt].contact	= udata->list[i].strings.contact;
			info->r_info[info->r_cnt].KIA		= kia;
			info->r_cnt++;
		}
		/* Clean up if no units found */
		if (!info->r_cnt) { delete[] info->r_info; info->r_info = NULL; }
	}

	calc_bridgecon(d.info_full);
	calc_influence(d.info_full);
	calc_frontline(d.info_full);
}

void
SmapWidget::load_info_lmtd (SPWAW_BTURN *turn)
{
	SPWAW_SNAP_VHEX		*vdata;
	SPWAW_SNAP_OOB_U	*udata;
	SmapHexPos		pos, npos;
	SmapHex			*hex, *nhex;
	int			i;
	int			x, y;
	bool			kia;

	GRID_INFO *info = &d.info_lmtd;

	info->grid.setup (d.info_full.grid);
	info->grid.dotted = true;

	vdata = turn->snap->game.battle.data.vhex;
	for (i=0; i<SPWAW_VHEXCNT; i++) {
		hex = info->grid.grid2hex (pos.set(vdata[i].x, vdata[i].y));
		hex->setVicHex (vdata[i].status, INTEL_MODE_LMTD);
	}

	for (y=0; y<info->grid.height; y++) {
		for (x=0; x<info->grid.width; x++) {
			hex = info->grid.grid2hex (x, y);
			if (!hex->frontline) continue;
			if (hex->influence != SMAP_HI_RED) continue;

			hex->influence = SMAP_HI_CONTESTED;
			if (hex->frontline & SMAP_HRD_mask[SMAP_HRD_EE]) {
				pos = hex->pos;
				if (info->grid.hexEE (pos, npos)) {
					nhex = info->grid.grid2hex (npos);
					nhex->influence = SMAP_HI_CONTESTED;
				}
				if (info->grid.hexWW (pos, npos)) {
					nhex = info->grid.grid2hex (npos);
					nhex->influence = SMAP_HI_CONTESTED;
				}
			}
			if (hex->frontline & SMAP_HRD_mask[SMAP_HRD_SE]) {
				pos = hex->pos;
				if (info->grid.hexSE (pos, npos)) {
					nhex = info->grid.grid2hex (npos);
					nhex->influence = SMAP_HI_CONTESTED;
				}
				if (info->grid.hexNW (pos, npos)) {
					nhex = info->grid.grid2hex (npos);
					nhex->influence = SMAP_HI_CONTESTED;
				}
			}
			if (hex->frontline & SMAP_HRD_mask[SMAP_HRD_SW]) {
				pos = hex->pos;
				if (info->grid.hexSW (pos, npos)) {
					nhex = info->grid.grid2hex (npos);
					nhex->influence = SMAP_HI_CONTESTED;
				}
				if (info->grid.hexNE (pos, npos)) {
					nhex = info->grid.grid2hex (npos);
					nhex->influence = SMAP_HI_CONTESTED;
				}
			}
			if (hex->frontline & SMAP_HRD_mask[SMAP_HRD_WW]) {
				pos = hex->pos;
				if (info->grid.hexWW (pos, npos)) {
					nhex = info->grid.grid2hex (npos);
					nhex->influence = SMAP_HI_CONTESTED;
				}
				if (info->grid.hexEE (pos, npos)) {
					nhex = info->grid.grid2hex (npos);
					nhex->influence = SMAP_HI_CONTESTED;
				}
			}
			if (hex->frontline & SMAP_HRD_mask[SMAP_HRD_NW]) {
				pos = hex->pos;
				if (info->grid.hexNW (pos, npos)) {
					nhex = info->grid.grid2hex (npos);
					nhex->influence = SMAP_HI_CONTESTED;
				}
				if (info->grid.hexSE (pos, npos)) {
					nhex = info->grid.grid2hex (npos);
					nhex->influence = SMAP_HI_CONTESTED;
				}
			}
			if (hex->frontline & SMAP_HRD_mask[SMAP_HRD_NE]) {
				pos = hex->pos;
				if (info->grid.hexNE (pos, npos)) {
					nhex = info->grid.grid2hex (npos);
					nhex->influence = SMAP_HI_CONTESTED;
				}
				if (info->grid.hexSW (pos, npos)) {
					nhex = info->grid.grid2hex (npos);
					nhex->influence = SMAP_HI_CONTESTED;
				}
			}
		}
	}
	calc_frontline(d.info_lmtd);

	udata = &turn->snap->OOBp1.battle.units;
	if (udata->cnt) {
		info->b_info = new UNIT_INFO[udata->cnt];
		info->b_cnt = 0;
		for (i=0; i<udata->cnt; i++) {
			if ((udata->list[i].data.posx < 0) || (udata->list[i].data.posy < 0)) continue;
			kia = (!udata->list[i].data.alive || (udata->list[i].data.aband == SPWAW_ASTAY));
			hex = info->grid.grid2hex (info->b_info[info->b_cnt].pos.set (udata->list[i].data.posx, udata->list[i].data.posy));
			hex->addUnit (SMAP_HI_BLUE);
			if (!kia) hex->all_KIA_blue = false;
			info->b_info[info->b_cnt].uid		= udata->list[i].strings.uid;
			info->b_info[info->b_cnt].unit		= udata->list[i].data.dname; // FIXME: designation, actually
			info->b_info[info->b_cnt].ready		= udata->list[i].attr.gen.ready;
			info->b_info[info->b_cnt].damage	= udata->list[i].data.damage;
			info->b_info[info->b_cnt].hcnt		= udata->list[i].data.hcnt_left;
			info->b_info[info->b_cnt].morale	= udata->list[i].data.mor;
			info->b_info[info->b_cnt].suppression	= udata->list[i].data.sup;
			info->b_info[info->b_cnt].status	= udata->list[i].strings.status;
			info->b_info[info->b_cnt].spotted	= udata->list[i].data.spotted ? "spotted" : "hidden";
			info->b_info[info->b_cnt].entrenchment	= udata->list[i].strings.entr;
			info->b_info[info->b_cnt].contact	= udata->list[i].strings.contact;
			info->b_info[info->b_cnt].KIA		= kia;
			info->b_cnt++;
		}
		/* Clean up if no units found */
		if (!info->b_cnt) { delete[] info->b_info; info->b_info = NULL; }
	}

	udata = &turn->snap->OOBp2.battle.units;
	if (udata->cnt) {
		info->r_info = new UNIT_INFO[udata->cnt];
		info->r_cnt = 0;
		for (i=0; i<udata->cnt; i++) {
			if ((udata->list[i].data.posx < 0) || (udata->list[i].data.posy < 0)) continue;
			if (udata->list[i].data.alive && (udata->list[i].data.aband != SPWAW_ASTAY) && !udata->list[i].data.spotted) continue;
			kia = (!udata->list[i].data.alive || (udata->list[i].data.aband == SPWAW_ASTAY));
			hex = info->grid.grid2hex (info->r_info[info->r_cnt].pos.set (udata->list[i].data.posx, udata->list[i].data.posy));
			hex->addUnit (SMAP_HI_RED);
			if (!kia) hex->all_KIA_red = false;
			info->r_info[info->r_cnt].uid		= udata->list[i].strings.uid;
			info->r_info[info->r_cnt].unit		= udata->list[i].data.dname; // FIXME: designation, actually
			info->r_info[info->r_cnt].ready		= udata->list[i].attr.gen.ready;
			info->r_info[info->r_cnt].damage	= udata->list[i].data.damage;
			info->r_info[info->r_cnt].hcnt		= udata->list[i].data.hcnt_left;
			info->r_info[info->r_cnt].morale	= udata->list[i].data.mor;
			info->r_info[info->r_cnt].suppression	= udata->list[i].data.sup;
			info->r_info[info->r_cnt].status	= udata->list[i].strings.status;
			info->r_info[info->r_cnt].spotted	= udata->list[i].data.spotted ? "spotted" : "hidden";
			info->r_info[info->r_cnt].entrenchment	= udata->list[i].strings.entr;
			info->r_info[info->r_cnt].contact	= udata->list[i].strings.contact;
			info->r_info[info->r_cnt].KIA		= kia;
			info->r_cnt++;
		}
		/* Clean up if no units found */
		if (!info->r_cnt) { delete[] info->r_info; info->r_info = NULL; }
	}
}

void
SmapWidget::load_info_none (SPWAW_BTURN *turn)
{
	SPWAW_SNAP_VHEX		*vdata;
	SPWAW_SNAP_OOB_U	*udata;
	SmapHexPos		pos;
	SmapHex			*hex;
	int			i;
	int			x, y;
	bool			kia;

	GRID_INFO *info = &d.info_none;

	info->grid.setup (d.info_full.grid);
	info->grid.dotted = true;

	vdata = turn->snap->game.battle.data.vhex;
	for (i=0; i<SPWAW_VHEXCNT; i++) {
		hex = info->grid.grid2hex (pos.set(vdata[i].x, vdata[i].y));

		hex->setVicHex (vdata[i].status, INTEL_MODE_NONE);
	}

	for (y=0; y<info->grid.height; y++) {
		for (x=0; x<info->grid.width; x++) {
			SmapHex *hex = info->grid.grid2hex (x, y);
			if (hex->influence == SMAP_HI_BLUE) continue;
			if (hex->influence == SMAP_HI_RED) hex->influence = SMAP_HI_CONTESTED;
		}
	}
	calc_frontline(d.info_none);

	udata = &turn->snap->OOBp1.battle.units;
	if (udata->cnt) {
		info->b_info = new UNIT_INFO[udata->cnt];
		info->b_cnt = 0;
		for (i=0; i<udata->cnt; i++) {
			if ((udata->list[i].data.posx < 0) || (udata->list[i].data.posy < 0)) continue;
			hex = info->grid.grid2hex (info->b_info[info->b_cnt].pos.set (udata->list[i].data.posx, udata->list[i].data.posy));
			kia = (!udata->list[i].data.alive || (udata->list[i].data.aband == SPWAW_ASTAY));
			hex->addUnit (SMAP_HI_BLUE);
			if (!kia) hex->all_KIA_blue = false;
			info->b_info[info->b_cnt].uid		= udata->list[i].strings.uid;
			info->b_info[info->b_cnt].unit		= udata->list[i].data.dname; // FIXME: designation, actually
			info->b_info[info->b_cnt].ready		= udata->list[i].attr.gen.ready;
			info->b_info[info->b_cnt].damage	= udata->list[i].data.damage;
			info->b_info[info->b_cnt].hcnt		= udata->list[i].data.hcnt_left;
			info->b_info[info->b_cnt].morale	= udata->list[i].data.mor;
			info->b_info[info->b_cnt].suppression	= udata->list[i].data.sup;
			info->b_info[info->b_cnt].status	= udata->list[i].strings.status;
			info->b_info[info->b_cnt].spotted	= udata->list[i].data.spotted ? "spotted" : "hidden";
			info->b_info[info->b_cnt].entrenchment	= udata->list[i].strings.entr;
			info->b_info[info->b_cnt].contact	= udata->list[i].strings.contact;
			info->b_info[info->b_cnt].KIA		= kia;
			info->b_cnt++;
		}
		/* Clean up if no units found */
		if (!info->b_cnt) { delete[] info->b_info; info->b_info = NULL; }
	}

	udata = &turn->snap->OOBp2.battle.units;
	if (udata->cnt) {
		info->r_info = new UNIT_INFO[udata->cnt];
		info->r_cnt = 0;
		for (i=0; i<udata->cnt; i++) {
			if ((udata->list[i].data.posx < 0) || (udata->list[i].data.posy < 0)) continue;
			if (udata->list[i].data.alive && (udata->list[i].data.aband != SPWAW_ASTAY) && !udata->list[i].data.spotted) continue;
			kia = (!udata->list[i].data.alive || (udata->list[i].data.aband == SPWAW_ASTAY));
			hex = info->grid.grid2hex (info->r_info[info->r_cnt].pos.set (udata->list[i].data.posx, udata->list[i].data.posy));
			hex->addUnit (SMAP_HI_RED);
			if (!kia) hex->all_KIA_red = false;
			info->r_info[info->r_cnt].uid		= udata->list[i].strings.uid;
			info->r_info[info->r_cnt].unit		= udata->list[i].data.dname; // FIXME: designation, actually
			info->r_info[info->r_cnt].ready		= udata->list[i].attr.gen.ready;
			info->r_info[info->r_cnt].damage	= udata->list[i].data.damage;
			info->r_info[info->r_cnt].hcnt		= udata->list[i].data.hcnt_left;
			info->r_info[info->r_cnt].morale	= udata->list[i].data.mor;
			info->r_info[info->r_cnt].suppression	= udata->list[i].data.sup;
			info->r_info[info->r_cnt].status	= udata->list[i].strings.status;
			info->r_info[info->r_cnt].spotted	= udata->list[i].data.spotted ? "spotted" : "hidden";
			info->r_info[info->r_cnt].entrenchment	= udata->list[i].strings.entr;
			info->r_info[info->r_cnt].contact	= udata->list[i].strings.contact;
			info->r_info[info->r_cnt].KIA		= kia;
			info->r_cnt++;
		}
		/* Clean up if no units found */
		if (!info->r_cnt) { delete[] info->r_info; info->r_info = NULL; }
	}
}

void
SmapWidget::calc_bridgecon (GRID_INFO &info)
{
	SmapHexPos	pos;
	SmapHex		*hex, *nhex;
	SmapHexPos	npos;
	int		ix, iy;

	for (iy=0; iy<info.grid.height; iy++) {
		for (ix=0; ix<info.grid.width; ix++) {
			hex = info.grid.grid2hex (ix, iy);
			if (!hex->bridge) continue;

			hex->conn_bridge = 0;
			if (info.grid.hexEE (hex->pos, npos)) {
				nhex = info.grid.grid2hex (npos);
				if (nhex->hasBridge() || nhex->hasRoad (SMAP_HRD_EE))
					hex->conn_bridge |= SMAP_HRD_mask[SMAP_HRD_EE];
			}
			if (info.grid.hexSE (hex->pos, npos)) {
				nhex = info.grid.grid2hex (npos);
				if (nhex->hasBridge() || nhex->hasRoad (SMAP_HRD_SE))
					hex->conn_bridge |= SMAP_HRD_mask[SMAP_HRD_SE];
			}
			if (info.grid.hexSW (hex->pos, npos)) {
				nhex = info.grid.grid2hex (npos);
				if (nhex->hasBridge() || nhex->hasRoad (SMAP_HRD_SW))
					hex->conn_bridge |= SMAP_HRD_mask[SMAP_HRD_SW];
			}
			if (info.grid.hexWW (hex->pos, npos)) {
				nhex = info.grid.grid2hex (npos);
				if (nhex->hasBridge() || nhex->hasRoad (SMAP_HRD_WW))
					hex->conn_bridge |= SMAP_HRD_mask[SMAP_HRD_WW];
			}
			if (info.grid.hexNW (hex->pos, npos)) {
				nhex = info.grid.grid2hex (npos);
				if (nhex->hasBridge() || nhex->hasRoad (SMAP_HRD_NW))
					hex->conn_bridge |= SMAP_HRD_mask[SMAP_HRD_NW];
			}
			if (info.grid.hexNE (hex->pos, npos)) {
				nhex = info.grid.grid2hex (npos);
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
height_weight (SMAP_HH tgt_height, SMAP_HH src_height, SMAP_HH max_height, double distance)
{
#if	!HEIGHTWEIGHT
	return (1.0);
#else	/* HEIGHTWEIGHT */
	int range = max_height - SMAP_HH_START; if (!range) return (1.0);
	static double scale = 0.75;

	int delta = src_height - tgt_height;
	return (1.0 + (((double)delta / (double)range * scale) / sqrt(distance)));
#endif	/* HEIGHTWEIGHT */
}

//#define	DISTANCE_THRESHOLD	0.0
#define	DISTANCE_THRESHOLD	1000.0
#define	DISTANCE_SCALEFACTOR	1.0
#define	LOCAL_INFLUENCE		10.0

void
SmapWidget::calc_influence (GRID_INFO &info)
{
	SmapHexPos	pos;
	int		ix, iy, idx, i;
	int		mcx, mcy, rcx, rcy;
	double		dv, iv, mbd, mrd;

	for (iy=0; iy<info.grid.height; iy++) {
		for (ix=0; ix<info.grid.width; ix++) {
			idx = info.grid.grid2idx (pos.set(ix, iy));

			info.grid.grid2coord (info.grid.map[idx].pos, mcx, mcy);

			info.grid.map[idx].influence_blue = 0.0; info.grid.map[idx].influence_blue_cnt = 0; mbd = 0.0;
			for (i=0; i<info.b_cnt; i++) {
				info.grid.grid2coord (info.b_info[i].pos, rcx, rcy);
				if ((rcx == mcx) && (rcy == mcy)) {
					dv = 0.0;
					iv = LOCAL_INFLUENCE;
				} else {
					dv = distance (mcx, mcy, rcx, rcy);
					iv = LOCAL_INFLUENCE / dv / dv;
					iv *= height_weight (info.grid.map[idx].height, info.grid.grid2hex (info.b_info[i].pos)->height, d.battle.max_height, dv);
				}
				iv *= info.b_info[i].ready;
				if (dv < DISTANCE_THRESHOLD) {
					info.grid.map[idx].influence_blue += iv;
					info.grid.map[idx].influence_blue_cnt++;
				} else {
					iv *= DISTANCE_SCALEFACTOR;
					if (iv > mbd) mbd = iv;
				}
			}
			if (!info.grid.map[idx].influence_blue_cnt) {
				info.grid.map[idx].influence_blue_cnt = -1;
				info.grid.map[idx].influence_blue = mbd;
			}
			if (info.grid.map[idx].influence_blue < mbd) {
				info.grid.map[idx].influence_blue = mbd;
			}

			info.grid.map[idx].influence_red = 0.0; info.grid.map[idx].influence_red_cnt = 0; mrd = 0.0;
			for (i=0; i<info.r_cnt; i++) {
				info.grid.grid2coord (info.r_info[i].pos, rcx, rcy);
				if ((rcx == mcx) && (rcy == mcy)) {
					dv = 0.0;
					iv = LOCAL_INFLUENCE;
				} else {
					dv = distance (mcx, mcy, rcx, rcy);
					iv = LOCAL_INFLUENCE / dv / dv;
					iv *= height_weight (info.grid.map[idx].height, info.grid.grid2hex (info.r_info[i].pos)->height, d.battle.max_height, dv);
				}
				iv *= info.r_info[i].ready;
				if (dv < DISTANCE_THRESHOLD) {
					info.grid.map[idx].influence_red += iv;
					info.grid.map[idx].influence_red_cnt++;
				} else {
					iv *= DISTANCE_SCALEFACTOR;
					if (iv > mrd) mrd = iv;
				}
			}
			if (!info.grid.map[idx].influence_red_cnt) {
				info.grid.map[idx].influence_red_cnt = -1;
				info.grid.map[idx].influence_red = mrd;
			}
			if (info.grid.map[idx].influence_red < mrd) {
				info.grid.map[idx].influence_red = mrd;
			}

			if ((info.grid.map[idx].influence_blue_cnt > 0) && (info.grid.map[idx].influence_red_cnt > 0)) {
				if (info.grid.map[idx].influence_blue > info.grid.map[idx].influence_red) {
					info.grid.map[idx].influence = SMAP_HI_BLUE;
				} else if (info.grid.map[idx].influence_blue < info.grid.map[idx].influence_red) {
					info.grid.map[idx].influence = SMAP_HI_RED;
				} else if (info.grid.map[idx].influence_blue_cnt > info.grid.map[idx].influence_red_cnt) {
					info.grid.map[idx].influence = SMAP_HI_BLUE;
				} else if (info.grid.map[idx].influence_blue_cnt < info.grid.map[idx].influence_red_cnt) {
					info.grid.map[idx].influence = SMAP_HI_RED;
				} else {
					info.grid.map[idx].influence = SMAP_HI_BLUE;
				}
			} else if (info.grid.map[idx].influence_blue_cnt > 0) {
				if (info.grid.map[idx].influence_blue > info.grid.map[idx].influence_red) {
					info.grid.map[idx].influence = SMAP_HI_BLUE;
				} else {
					info.grid.map[idx].influence = SMAP_HI_RED;
				}
			} else if (info.grid.map[idx].influence_red_cnt > 0) {
				if (info.grid.map[idx].influence_red > info.grid.map[idx].influence_blue) {
					info.grid.map[idx].influence = SMAP_HI_RED;
				} else {
					info.grid.map[idx].influence = SMAP_HI_BLUE;
				}
			} else if(info.grid.map[idx].influence_blue >= info.grid.map[idx].influence_red) {
				info.grid.map[idx].influence = SMAP_HI_BLUE;
			} else {
				info.grid.map[idx].influence = SMAP_HI_RED;
			}
		}
	}
}

void
SmapWidget::calc_frontline (GRID_INFO &info)
{
	SmapHexPos	pos;
	SmapHex		*hex, *nhex;
	SmapHexPos	npos;
	int		ix, iy;

	for (iy=0; iy<info.grid.height; iy++) {
		for (ix=0; ix<info.grid.width; ix++) {
			hex = info.grid.grid2hex (ix, iy);

			hex->frontline = 0;
			if (info.grid.hexEE (hex->pos, npos)) {
				nhex = info.grid.grid2hex (npos);
				if (hex->influence != nhex->influence) hex->frontline |= SMAP_HRD_mask[SMAP_HRD_EE];
			}
			if (info.grid.hexSE (hex->pos, npos)) {
				nhex = info.grid.grid2hex (npos);
				if (hex->influence != nhex->influence) hex->frontline |= SMAP_HRD_mask[SMAP_HRD_SE];
			}
			if (info.grid.hexSW (hex->pos, npos)) {
				nhex = info.grid.grid2hex (npos);
				if (hex->influence != nhex->influence) hex->frontline |= SMAP_HRD_mask[SMAP_HRD_SW];
			}
			if (info.grid.hexWW (hex->pos, npos)) {
				nhex = info.grid.grid2hex (npos);
				if (hex->influence != nhex->influence) hex->frontline |= SMAP_HRD_mask[SMAP_HRD_WW];
			}
			if (info.grid.hexNW (hex->pos, npos)) {
				nhex = info.grid.grid2hex (npos);
				if (hex->influence != nhex->influence) hex->frontline |= SMAP_HRD_mask[SMAP_HRD_NW];
			}
			if (info.grid.hexNE (hex->pos, npos)) {
				nhex = info.grid.grid2hex (npos);
				if (hex->influence != nhex->influence) hex->frontline |= SMAP_HRD_mask[SMAP_HRD_NE];
			}
		}
	}
}

#define	DATA(dst_,src_,lim_)	\
	do { if (d.intel_mode <= lim_) dst_ = src_; else dst_.clear(); } while (0)

void
SmapWidget::applycursor (void)
{
	int		idx;
	int		rows;
	MDLT_DATA	data;
	int		i;

	idx = d.info->grid.grid2idx (d.cursor);
	if (idx < 0) return;

	rows = d.info->grid.map[idx].unit_cnt_blue + d.info->grid.map[idx].unit_cnt_red;

	MDLT_alloc_data (data, rows, d.model->columnCount());

	idx = 0;
	for (i=0; i<d.info->b_cnt; i++) {
		if (d.info->b_info[i].pos != d.cursor) continue;
		data.row[idx].bg = *(RES_color(RID_RGB_PLAYER_BG));
		data.row[idx].data[UTC_UID]	= d.info->b_info[i].uid;
		data.row[idx].data[UTC_UNIT]	= d.info->b_info[i].unit;
		data.row[idx].data[UTC_READY]	= d.info->b_info[i].ready;
		data.row[idx].data[UTC_DMG]	= d.info->b_info[i].damage;
		data.row[idx].data[UTC_MEN]	= d.info->b_info[i].hcnt;
		data.row[idx].data[UTC_MOR]	= d.info->b_info[i].morale;
		data.row[idx].data[UTC_SUP]	= d.info->b_info[i].suppression;
		data.row[idx].data[UTC_STATUS]	= d.info->b_info[i].status;
		data.row[idx].data[UTC_SEEN]	= d.info->b_info[i].spotted;
		data.row[idx].data[UTC_ENTR]	= d.info->b_info[i].entrenchment;
		data.row[idx].data[UTC_CONTACT]	= d.info->b_info[i].contact;
		idx++;
	}
	for (i=0; i<d.info->r_cnt; i++) {
		if (d.info->r_info[i].pos != d.cursor) continue;
		data.row[idx].bg = *(RES_color(RID_RGB_OPPONENT_BG));
		DATA (data.row[idx].data[UTC_UID],	d.info->r_info[i].uid,		INTEL_MODE_LMTD);
		DATA (data.row[idx].data[UTC_UNIT],	d.info->r_info[i].unit,		INTEL_MODE_NONE);
		DATA (data.row[idx].data[UTC_READY],	d.info->r_info[i].ready,	INTEL_MODE_FULL);
		DATA (data.row[idx].data[UTC_DMG],	d.info->r_info[i].damage,	INTEL_MODE_FULL);
		DATA (data.row[idx].data[UTC_MEN],	d.info->r_info[i].hcnt,		INTEL_MODE_FULL);
		DATA (data.row[idx].data[UTC_MOR],	d.info->r_info[i].morale,	INTEL_MODE_FULL);
		DATA (data.row[idx].data[UTC_SUP],	d.info->r_info[i].suppression,	INTEL_MODE_FULL);
		DATA (data.row[idx].data[UTC_STATUS],	d.info->r_info[i].status,	INTEL_MODE_NONE);
		DATA (data.row[idx].data[UTC_SEEN],	d.info->r_info[i].spotted,	INTEL_MODE_NONE);
		DATA (data.row[idx].data[UTC_ENTR],	d.info->r_info[i].entrenchment,	INTEL_MODE_LMTD);
		DATA (data.row[idx].data[UTC_CONTACT],	d.info->r_info[i].contact,	INTEL_MODE_FULL);

		idx++;
	}

	d.model->load (&data);
	MDLT_free_data (data);

	d.model->commit();
}

void
SmapWidget::setcursor (SmapHexPos &pos)
{
	int		idx;

	idx = d.info->grid.grid2idx (pos);
	if (idx < 0) return;

	d.cursor = pos;

	applycursor();
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

void
SmapWidget::composite (QPainter &paint, COMPOSITOR_CFG &cfg)
{
	QBrush		bb;
	QPen		bp;

	/* Try to use anti-aliasing */
	paint.setRenderHints (QPainter::Antialiasing|QPainter::HighQualityAntialiasing|QPainter::SmoothPixmapTransform, true);

	/* Paint the frame first */
	bb.setColor (Qt::darkGreen);
	bb.setStyle (Qt::SolidPattern);
	bp.setColor (bb.color().darker(200));
	bp.setWidth (1);

	paint.setBrush (bb);
	paint.setPen (bp);

	paint.drawRect (d.rptr->frame);

	/* Next, paint the hexes */
	paint.drawPixmap (d.rptr->map.x(), d.rptr->map.y(), *(d.rptr->layer.hmap));

	/* And paint the features */
	paint.drawPixmap (d.rptr->map.x(), d.rptr->map.y(), *(d.rptr->layer.features));

	/* Paint the victory hexes, if enabled */
	if (cfg.vhex) {
		paint.drawPixmap (d.rptr->map.x(), d.rptr->map.y(), *(d.rptr->layer.vhex));
	}

	/* Paint the influence, if enabled */
	if (cfg.influence) {
		paint.drawPixmap (d.rptr->map.x(), d.rptr->map.y(), *(d.rptr->layer.influence));
	}

	/* Paint the grid, if enabled */
	if (cfg.grid) {
		paint.drawPixmap (d.rptr->map.x(), d.rptr->map.y(), *(d.rptr->layer.grid));
	}

	/* Paint the frontline, if enabled */
	if (cfg.frontline) {
		paint.drawPixmap (d.rptr->map.x(), d.rptr->map.y(), *(d.rptr->layer.frontline));
	}

	/* Finally, paint the dots */
	paint.drawPixmap (d.rptr->map.x(), d.rptr->map.y(), *(d.rptr->layer.dots));
}

/* Constructs a base filename for a stratmap image to be saved */
static void
smap_save_basename (SmapWidget::BATTLE_INFO &info, SmapRenderer *rptr, SmapWidget::COMPOSITOR_CFG &cfg, char *dst, unsigned int len)
{
	const char	fmt[] = "stratmap_%04d-%02d-%02d_%s_%02dh%02d_turn%02d_%s_g%d_i%d_v%d_f%d";

	if (!rptr || !dst || !len) return;
	memset (dst, len, 0);

	QString location (info.location);
	location.remove (QRegExp("[\\/:\"*?<>|]+"));

	/* Determine the filename of the image to be saved */
	snprintf (dst, len - 1, fmt,
		info.date.date.year, info.date.date.month, info.date.date.day,
		qPrintable(location),
		info.date.date.hour, info.date.date.minute,
		info.date.turn,
		rptr->description(),
		cfg.grid, cfg.influence, cfg.vhex, cfg.frontline);
}

/*! Saves the stratmap pixmap as an image in the right location.
 * Returns the filename if the save succeeded.
 */
static QString
smap_save_image (QPixmap &pixmap, char *basename, const char *ext)
{
	QString		filename;
	QString		pathname;

	/* Save the pixmap in the snapshots directory */
	filename.sprintf ("%s.%s", basename, ext);
	pathname.sprintf ("%s/%s", CFG_snp_path(), qPrintable(filename));

	if (!pixmap.save (pathname, ext)) filename.clear();
	return (filename);
}

/*! Reports the save result */
static void
smap_save_report_result (QString filename)
{
	MSGBOX_TYPE	type;
	char		msg[256];

	/* Report success or failure */
	memset (msg, sizeof(msg), 0);
	if (!filename.isEmpty()) {
		type = MSGBOX_INFO;
		snprintf (msg, sizeof(msg) - 1,
			"Stratmap image saved as:\n%s", qPrintable(filename));
	} else {
		type = MSGBOX_WARNING;
		snprintf (msg, sizeof(msg) - 1,
			"Failed to save stratmap image!");
	}
	GUI_msgbox (type, "Stratmap image", msg);
}

void
SmapWidget::save_smap (void)
{
	static const char *ext = "png";

	BATTLE_INFO	info = d.battle;
	SmapRenderer	*rptr = d.rptr;
	COMPOSITOR_CFG	cfg = d.comp_cfg;
	char		base[MAX_PATH+1];

	GuiProgress	progress ("Saving stratmap image...", 0);
	progress.setRange (0, 4);

	/* Determine the base filenames of the stratmap image */
	smap_save_basename (info, rptr, cfg, base, sizeof(base));

	progress.inc();

	/* Create an empty pixmap to receive the stratmap image */
	QPixmap	*pm = new QPixmap (rptr->canvas.size());

	progress.inc();

	/* Composite the stratmap image */
	QPainter *paint = new QPainter (pm);
	composite (*paint, cfg);
	delete paint;

	progress.inc();

	/* Now, save the image */
	QString filename = smap_save_image (*pm, base, "png");
	delete pm;

	progress.inc();
	progress.done();

	/* Finally, report the result */
	smap_save_report_result (filename);
}
